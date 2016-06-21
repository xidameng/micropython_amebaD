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

#include "exception.h"

#include "bufhelper.h"
#include "objuart.h"

#define UART0_TX                             PA_7
#define UART0_RX                             PA_6

#define UART1_TX                             PB_5
#define UART1_RX                             PB_4

#define UART2_TX                             PA_1
#define UART2_RX                             PA_4

serial_t uart_channel0;     //(RX, TX) = (PA_6, PA_7) 
serial_t uart_channel1;     //(RX, TX) = (PB_5, PB_4)
serial_t uart_channel2;     //(RX, TX) = (PA_4, PA_1)

STATIC mp_obj_t mp_uart_clear(mp_obj_t self_in) {
    uart_obj_t *self = (uart_obj_t *)&self_in;
    serial_clear((serial_t *)self->obj);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(uart_clear_obj, mp_uart_clear);

STATIC mp_obj_t uart_send(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    STATIC const mp_arg_t uart_send_args[] = {
        { MP_QSTR_buf,     MP_ARG_REQUIRED | MP_ARG_OBJ,  },
    };

    uart_obj_t *self = pos_args[0];
    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(uart_send_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(args), uart_send_args, args);

    // get the buffer to send from
    mp_buffer_info_t bufinfo;
    uint8_t data[1];
    pyb_buf_get_for_send(args[0].u_obj, &bufinfo, data);

    // send the data
    size_t i = 0;
    char *ptr = 0;
    ptr = bufinfo.buf;
    for (i = 0; i < bufinfo.len; i++) {
        serial_putc((serial_t *)self->obj, ptr[i]);
    }

    // return the number of bytes written
    return mp_obj_new_int(bufinfo.len);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(uart_send_obj, 1, uart_send);

STATIC mp_obj_t uart_recv(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    STATIC const mp_arg_t uart_recv_args[] = {
        { MP_QSTR_nbytes,  MP_ARG_REQUIRED | MP_ARG_OBJ, },
    };

    uart_obj_t *self = pos_args[0];
    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(uart_recv_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(args), uart_recv_args, args);

    vstr_t vstr;

    // get the buffer to receive into
    pyb_buf_get_for_recv(args[0].u_obj, &vstr);

    size_t i = 0;

    // receive the data
    for (i = 0; i < vstr.len; i++) {
        vstr.buf[i] = serial_getc((serial_t *)self->obj);
    }

    // return the received data
    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(uart_recv_obj, 1, uart_recv);



STATIC void uart_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    uart_obj_t *self = self_in;
    mp_printf(print, "UART(%d, baudrate=%u, bits=%d, stop=%d, parity=%d)", self->id, self->baudrate, self->bits, self->stop, self->parity);
}

STATIC mp_obj_t uart_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {
    const mp_arg_t uart_init_args[] = {
        { MP_QSTR_id,                          MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_baudrate,  MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = UART_DEFAULT_BAUD_RATE} },
        { MP_QSTR_bits,      MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 8} },
        { MP_QSTR_stop,      MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 1} },
        { MP_QSTR_parity,    MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = ParityNone} },
    };
    // parse args
    mp_map_t kw_args;
    mp_uint_t PinTX;
    mp_uint_t PinRX;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(uart_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), uart_init_args, args);

    if ((args[0].u_int < 0) || (args[0].u_int > 2)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, mpexception_os_resource_not_avaliable));
    }   

    uart_obj_t *self = m_new_obj(uart_obj_t);
    self->base.type = &uart_type;
    self->id        = args[0].u_int;
    self->baudrate  = MIN(MAX(args[1].u_int, UART_MIN_BAUD_RATE), UART_MAX_BAUD_RATE);
    self->bits      = args[2].u_int;
    self->stop      = args[3].u_int;
    self->parity    = args[4].u_int;
    
    switch(self->id) {
        case 0:
            self->obj = (void *)&uart_channel0;
            PinTX = UART0_TX;
            PinRX = UART0_RX;
            break;
        case 1:
            self->obj = (void *)&uart_channel1;
            PinTX = UART1_TX;
            PinRX = UART1_RX;
            break;
        case 2:
            self->obj = (void *)&uart_channel2;
            PinTX = UART2_TX;
            PinRX = UART2_RX;
            break;
        default:
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, mpexception_os_resource_not_avaliable));
            break;
    }

    // TODO: should check alternative pins 
    serial_init((serial_t *)self->obj, PinTX, PinRX);

    serial_baud((serial_t *)self->obj, self->baudrate);

    serial_format((serial_t *)self->obj, self->bits, self->parity, self->stop);

    return (mp_obj_t)self;
}


STATIC const mp_map_elem_t uart_locals_dict_table[] = {
    // instance methods
    { MP_OBJ_NEW_QSTR(MP_QSTR_clear),               (mp_obj_t)&uart_clear_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_send),                (mp_obj_t)&uart_send_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_recv),                (mp_obj_t)&uart_recv_obj },
    // class constants
};
STATIC MP_DEFINE_CONST_DICT(uart_locals_dict, uart_locals_dict_table);

const mp_obj_type_t uart_type = {
    { &mp_type_type },
    .name        = MP_QSTR_UART,
    .print       = uart_print,
    .make_new    = uart_make_new,
    .locals_dict = (mp_obj_t)&uart_locals_dict,
};
