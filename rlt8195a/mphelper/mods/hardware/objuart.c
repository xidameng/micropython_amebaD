/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 * Copyright (c) 2015 Daniel Campora
 * Copyright (c) 2016 Chester Tseng
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "py/mpstate.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "py/stream.h"
#include "py/ringbuf.h"

#include "py/mperrno.h"
#include "exception.h"

#include "bufhelper.h"
#include "objuart.h"
#include "rtc_api.h"

extern ringbuf_t input_buf;
extern int interrupt_char;

void term_irq(uart_obj_t *self, SerialIrq event) {
    serial_t *sobj = &(self->obj);
    int c = serial_getc(sobj);
    if (c == interrupt_char) {
        mp_keyboard_interrupt();
    } else {
        ringbuf_put(&input_buf, c);
    }
}

void uart_rx_irq_handler(uart_obj_t *self, SerialIrq event) {
    if (self->isr_handler != mp_const_none) {
        gc_lock();
        nlr_buf_t nlr;
        if (nlr_push(&nlr) == 0) {
            mp_call_function_1(self->isr_handler, self->unit);
            nlr_pop();
        } else {
            mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
        }
        gc_unlock();
    }
}

bool uart_rx_wait(uint32_t timeout_us) {
    mp_uint_t start = xTaskGetTickCount();
    for (;;) {
        if (input_buf.iget != input_buf.iput) {
            return true;
        }
        if (xTaskGetTickCount() - start >= timeout_us) {
            return false;
        }
    }
}

STATIC mp_obj_t uart_recv(mp_obj_t self_in, void *buf_in, mp_uint_t size, int *errcode) {
    uart_obj_t *self = self_in;

    if (size == 0) {
        return 0;
    }

    // wait for first char to become available
    if (!uart_rx_wait(self->timeout * 1000)) {
        *errcode = MP_EAGAIN;
        return MP_STREAM_ERROR;
    }

    uint8_t *buf = buf_in;

    for (;;) {
        *buf++ = ringbuf_get(&input_buf);       
        if (--size == 0 || !uart_rx_wait(self->timeout_char * 1000)) {
            return buf - (uint8_t *)buf_in;
        }
    }
}

STATIC mp_obj_t uart_send(mp_obj_t self_in, const void *buf_in, mp_uint_t size, int *errcode) {
    uart_obj_t *self = self_in;
    const uint8_t *buf = buf_in;

    for (size_t i=0; i<size; i++) {
        serial_putc(&(self->obj), buf[i]);
    }

    return size;
}

STATIC void uart_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    uart_obj_t *self = self_in;
    mp_printf(print, "UART(%d, baudrate=%u, bits=%d, stop=%d, parity=%d, TX=%q, Rx=%q)", self->unit, \
            self->baudrate, self->bits, self->stop, self->parity, self->tx->name, self->rx->name);
}

STATIC mp_obj_t uart_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {

    // TODO: alloc a new tulpe ???
    // TODO: CTS/RTS pin and flow control needed!
    mp_obj_t tuple[2];
    tuple[0] = &pin_PA_7;
    tuple[1] = &pin_PA_6;

    const mp_arg_t uart_init_args[] = {
        { MP_QSTR_unit,                          MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_baudrate,     MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = UART_DEFAULT_BAUD_RATE} },
        { MP_QSTR_bits,         MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 8} },
        { MP_QSTR_stop,         MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 1} },
        { MP_QSTR_parity,       MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = ParityNone} },
        { MP_QSTR_timeout,      MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_timeout_char, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_pins,         MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = mp_obj_new_tuple(2, tuple)} },
    };
    // parse args
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(uart_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), uart_init_args, args);

    mp_obj_t *pins;

    mp_obj_get_array_fixed_n(args[7].u_obj, 2, &pins);

    pin_obj_t *tx = pin_find(pins[0]);
    pin_obj_t *rx = pin_find(pins[1]);

    if (pin_obj_find_af(tx, PIN_FN_UART, args[0].u_int, PIN_TYPE_UART_TX) < 0) 
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid TX pin"));

    if (pin_obj_find_af(rx, PIN_FN_UART, args[0].u_int, PIN_TYPE_UART_RX) < 0) 
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid RX pin"));

    uart_obj_t *self    = m_new_obj(uart_obj_t);
    self->base.type     = &uart_type;
    self->unit          = args[0].u_int;
    self->is_inited     = false;
    self->baudrate      = MIN(MAX(args[1].u_int, UART_MIN_BAUD_RATE), UART_MAX_BAUD_RATE);
    self->bits          = args[2].u_int;
    self->stop          = args[3].u_int;
    self->parity        = args[4].u_int;
    self->timeout       = args[5].u_int;
    self->timeout_char  = args[6].u_int;
    self->tx            = tx;
    self->rx            = rx;

    return (mp_obj_t)self;
}

STATIC mp_obj_t uart_init0(mp_obj_t self_in) {
    uart_obj_t *self = self_in;
    if (self->is_inited ==  true) 
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "UART object has init"));
    serial_init(&(self->obj), self->tx->id, self->rx->id);
    serial_baud(&(self->obj), self->baudrate);
    serial_format(&(self->obj), self->bits, self->parity, self->stop);
    self->is_inited = true;
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(uart_init_obj, uart_init0);

STATIC mp_obj_t uart_deinit0(mp_obj_t self_in) {
    uart_obj_t *self = self_in;
    if (self->is_inited == false)
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "UART object has not init"));
    serial_free(&(self->obj));
    self->is_inited = false;
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(uart_deinit_obj, uart_deinit0);

STATIC mp_obj_t uart_dupterm_notify(mp_obj_t self_in, mp_obj_t enable_in) {
    uart_obj_t *self = self_in;
    bool enable = mp_obj_is_true(enable_in);
    serial_irq_handler(&(self->obj), term_irq, (uint32_t)&(self->obj));
    serial_irq_set(&(self->obj), RxIrq, enable);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(uart_dupterm_notify_obj, uart_dupterm_notify);

STATIC mp_obj_t uart_callback(mp_obj_t self_in, mp_obj_t func_in) {
    uart_obj_t *self = self_in;
    if (func_in == mp_const_none) {
        serial_irq_handler(&(self->obj), NULL, (uint32_t)&(self->obj));
        serial_irq_set(&(self->obj), RxIrq, false);
    } else {
        serial_irq_handler(&(self->obj), uart_rx_irq_handler, (uint32_t)&(self->obj));
        serial_irq_set(&(self->obj), RxIrq, true);
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(uart_callback_obj, uart_callback);

STATIC const mp_map_elem_t uart_locals_dict_table[] = {
    // instance methods
    { MP_OBJ_NEW_QSTR(MP_QSTR_init),            (mp_obj_t)&uart_init_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_deinit),          (mp_obj_t)&uart_deinit_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_write),           (mp_obj_t)&mp_stream_write_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_read),            (mp_obj_t)&mp_stream_read_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_readall),         (mp_obj_t)(&mp_stream_readall_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_readline),        (mp_obj_t)(&mp_stream_unbuffered_readline_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_readinto),        (mp_obj_t)(&mp_stream_readinto_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dupterm_notify),  (mp_obj_t)(&uart_dupterm_notify_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_callback),        (mp_obj_t)(&uart_callback_obj) },

    // class constants
    { MP_OBJ_NEW_QSTR(MP_QSTR_ParityNone),      MP_OBJ_NEW_SMALL_INT(ParityNone) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_ParityOdd),       MP_OBJ_NEW_SMALL_INT(ParityOdd) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_ParityEven),      MP_OBJ_NEW_SMALL_INT(ParityEven) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_ParityForced1),   MP_OBJ_NEW_SMALL_INT(ParityForced1) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_ParityForced0),   MP_OBJ_NEW_SMALL_INT(ParityForced0) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_FlowControlNone),     MP_OBJ_NEW_SMALL_INT(FlowControlNone) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_FlowControlRTS),      MP_OBJ_NEW_SMALL_INT(FlowControlRTS) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_FlowControlCTS),      MP_OBJ_NEW_SMALL_INT(FlowControlCTS) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_FlowControlRTSCTS),   MP_OBJ_NEW_SMALL_INT(FlowControlRTSCTS) },
};
STATIC MP_DEFINE_CONST_DICT(uart_locals_dict, uart_locals_dict_table);

STATIC const mp_stream_p_t uart_stream_p = {
    .read    = uart_recv,
    .write   = uart_send,
    .is_text = false,
};

const mp_obj_type_t uart_type = {
    { &mp_type_type },
    .name        = MP_QSTR_UART,
    .print       = uart_print,
    .make_new    = uart_make_new,
    .getiter     = mp_identity,
    .iternext    = mp_stream_unbuffered_iter,
    .protocol    = &uart_stream_p,
    .locals_dict = (mp_obj_t)&uart_locals_dict,
};
