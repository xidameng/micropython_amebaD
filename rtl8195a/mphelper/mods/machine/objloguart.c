/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
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

/*****************************************************************************
 *                              Header includes
 * ***************************************************************************/
#include "objloguart.h"

// TODO(Chester) This array should place in objuart.c
STATIC const char *_parity_name[] = {"None", "1", "0"};

STATIC log_uart_obj_t log_uart_obj = {
    .base.type      = &log_uart_type,
    .params = {
        .baudrate  = LOG_UART_DEFAULT_BAUDRATE,
        .data_bits = LOG_UART_DEFAULT_DATA_BITS,
        .parity    = LOG_UART_DEFAULT_PARITY,
        .stop_bits = LOG_UART_DEFAULT_STOP_BITS,
    },
    .tx = {
        .timeout_ms = LOG_UART_DEFAULT_TX_TIMEOUT,
    },
    .rx = {
        .timeout_ms = LOG_UART_DEFAULT_RX_TIMEOUT,
    },
    .irq_enabled = true,
    .irq_handler = mp_const_none,
};

/*
 *  OBJLOG_UART Interrupt Service Handler
 */
void mp_obj_log_uart_irq_handler (log_uart_obj_t *self, LOG_UART_INT_ID event) {

    /*
     *  At last read one char from register to prevent from interrupt pending
     */

    if (event == IIR_RX_RDY || event == IIR_CHAR_TIMEOUT) {

        char chr = log_uart_getc(&(self->obj));

        if (self->irq_handler != mp_const_none) {
            /*
             * Don't lock gc (gc_lock) here because we need to create a new queue for mallo, if locked
             * gc_alloc would be wrong
             *
             */
            nlr_buf_t nlr;
            if (nlr_push(&nlr) == 0) {
                mp_call_function_2(self->irq_handler, MP_OBJ_FROM_PTR(self),
                        mp_obj_new_bytes(&chr, 1));
                nlr_pop();
            } else {
                self->irq_handler = mp_const_none;
                mp_printf(&mp_plat_print, "Uncaught exception in callback handler");
                if (nlr.ret_val != MP_OBJ_NULL) {
                    mp_obj_print_exception(&mp_plat_print, nlr.ret_val);
                }
            }
        }
    }
}

void loguart_init0(void) {
    // Do nothing here
}

STATIC void log_uart_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    log_uart_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "LOGUART(baudrate=%u, bits=%u, parity=%s, stop=%u, tx_timeout=%u, rx_timeout=%u)",
        self->params.baudrate, self->params.data_bits,
        _parity_name[self->params.parity], self->params.stop_bits,
        self->tx.timeout_ms, self->rx.timeout_ms);
}

STATIC mp_obj_t log_uart_make_new (const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 0, 0, false);
    return (mp_obj_t)&log_uart_obj;
}

STATIC mp_obj_t log_uart_init0(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    enum { ARG_baudrate, ARG_bits, ARG_parity, ARG_stop, ARG_tx_timeout, ARG_rx_timeout };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_baudrate,     MP_ARG_INT,  {.u_int = 0} },
        { MP_QSTR_bits,         MP_ARG_INT,  {.u_int = 0} },
        { MP_QSTR_parity,       MP_ARG_OBJ,  {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_stop,         MP_ARG_INT,  {.u_int = 0} },
        { MP_QSTR_tx_timeout,   MP_ARG_INT,  {.u_int = 0} },
        { MP_QSTR_rx_timeout,   MP_ARG_INT,  {.u_int = 0} },
    };
    mp_arg_val_t p_args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, p_args);

    log_uart_obj_t *self = MP_OBJ_TO_PTR(args[0]);

    if (p_args[ARG_baudrate].u_int > 0) {
        self->params.baudrate = p_args[ARG_baudrate].u_int;
    }

    if (p_args[ARG_bits].u_int > 0) {
        self->params.data_bits = p_args[ARG_bits].u_int;
    }

    if (p_args[ARG_parity].u_obj != MP_OBJ_NULL) {
        if (p_args[ARG_parity].u_obj == mp_const_none) {
            self->params.parity = 0;
        } else {
            self->params.parity = mp_obj_get_int(p_args[ARG_parity].u_obj);;
        }
    }

    if (p_args[ARG_stop].u_int > 0) {
        self->params.stop_bits = p_args[ARG_stop].u_int;
    }

    if (p_args[ARG_tx_timeout].u_int > 0) {
        self->tx.timeout_ms = p_args[ARG_tx_timeout].u_int;
    }

    if (p_args[ARG_rx_timeout].u_int > 0) {
        self->rx.timeout_ms = p_args[ARG_rx_timeout].u_int;
    }

    if (log_uart_init(&(self->obj), self->params.baudrate,
                self->params.data_bits, self->params.parity, self->params.stop_bits) < 0) {
        mp_raise_ValueError("Invalid parameters");
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(log_uart_init_obj, 1, log_uart_init0);

STATIC mp_obj_t log_uart_deinit(mp_obj_t *self_in) {
    log_uart_obj_t *self = MP_OBJ_TO_PTR(self_in);
    log_uart_free(&(self->obj));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(log_uart_deinit_obj, log_uart_deinit);

STATIC mp_obj_t log_uart_irq_enable(mp_uint_t n_args, const mp_obj_t *args) {
    log_uart_obj_t *self = args[0];
    if (n_args == 1) {
        // get the value
        return MP_OBJ_NEW_SMALL_INT(self->irq_enabled);
    } else {
        // set the pin value
        if (mp_obj_is_true(args[1])) {
            self->irq_enabled = 1;
        } else {
            self->irq_enabled = 0;
        }
        log_uart_irq_set(&(self->obj), IIR_RX_RDY, self->irq_enabled);
        return mp_const_none;
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(log_uart_irq_enable_obj, 1, 2, log_uart_irq_enable);

STATIC mp_obj_t log_uart_irq_handler0(mp_obj_t self_in, mp_obj_t callback_in) {
    log_uart_obj_t *self = self_in;
    
    if (!MP_OBJ_IS_FUN(callback_in) && (callback_in != mp_const_none)) {
        mp_raise_ValueError("Error function type");
    }

    self->irq_handler = callback_in;
    log_uart_irq_handler(&(self->obj), mp_obj_log_uart_irq_handler, self);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(log_uart_irq_handler_obj, log_uart_irq_handler0);

STATIC const mp_map_elem_t log_uart_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_init),        MP_OBJ_FROM_PTR(&log_uart_init_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_deinit),      MP_OBJ_FROM_PTR(&log_uart_deinit_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_read),        MP_OBJ_FROM_PTR(&mp_stream_read_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_readline),    MP_OBJ_FROM_PTR(&mp_stream_unbuffered_readline_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_readinto),    MP_OBJ_FROM_PTR(&mp_stream_readinto_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_write),       MP_OBJ_FROM_PTR(&mp_stream_write_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_irq_enable),  MP_OBJ_FROM_PTR(&log_uart_irq_enable_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_irq_handler), MP_OBJ_FROM_PTR(&log_uart_irq_handler_obj) },
};
STATIC MP_DEFINE_CONST_DICT(log_uart_locals_dict, log_uart_locals_dict_table);

STATIC mp_uint_t log_uart_read(mp_obj_t self_in, char *buf_in, mp_uint_t size, int *errcode) {
    log_uart_obj_t *self = MP_OBJ_TO_PTR(self_in);

    // Direct return 0 when size = 0, to save the time
    if (size == 0) 
        return 0;

    // Here we use the blocking mode with timeout to receive the RX bytes
    int32_t ret = log_uart_recv(&(self->obj), buf_in, size, self->rx.timeout_ms);

    if (ret < 0) {
        *errcode = MP_EAGAIN;
        return MP_STREAM_ERROR;
    } else {
        return ret;
    }
}

STATIC mp_uint_t log_uart_write(mp_obj_t self_in, const char *buf_in, mp_uint_t size, int *errcode) {
    log_uart_obj_t *self = MP_OBJ_TO_PTR(self_in);

    // Here we use the blocking mode with timeout to receive the RX bytes
    int32_t ret = log_uart_send(&(self->obj), buf_in, size, self->tx.timeout_ms);

    if (ret < 0) {
        *errcode = MP_EAGAIN;
        return MP_STREAM_ERROR;
    } else {
        return ret;
    }
}

STATIC const mp_stream_p_t log_uart_stream_p = {
    .read    = log_uart_read,
    .write   = log_uart_write,
    .is_text = false,
};

const mp_obj_type_t log_uart_type = {
    { &mp_type_type },
    .name        = MP_QSTR_LOGUART,
    .print       = log_uart_print,
    .make_new    = log_uart_make_new,
    .getiter     = mp_identity,
    .iternext    = mp_stream_unbuffered_iter,
    .protocol    = &log_uart_stream_p,
    .locals_dict = (mp_obj_dict_t *)&log_uart_locals_dict,
};
