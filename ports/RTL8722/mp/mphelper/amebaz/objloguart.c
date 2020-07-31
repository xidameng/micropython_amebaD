/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Chester Tseng
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

STATIC const char *_parity_name[] = {"None", "1", "0"};
STATIC byte input_buf_array[32];
ringbuf_t input_buf = {input_buf_array, sizeof(input_buf_array)};

STATIC log_uart_obj_t log_uart_obj = {
    .base.type      = &log_uart_type,
    .params = {
        .baudrate  = LOGUART_DEFAULT_BAUDRATE,
        .data_bits = LOGUART_DEFAULT_DATA_BITS,
        .parity    = LOGUART_DEFAULT_PARITY,
        .stop_bits = LOGUART_DEFAULT_STOP_BITS,
    },
    .tx = {
        .timeout_ms = LOGUART_DEFAULT_TX_TIMEOUT,
    },
    .rx = {
        .timeout_ms = LOGUART_DEFAULT_RX_TIMEOUT,
    },
};

/*
 *  OBJLOG_UART Interrupt Service Handler
 */
void mp_obj_log_uart_irq_handler (log_uart_obj_t *self) {
    u32 IrqEn = LOGUART_GetIMR();
    DiagSetIsrEnReg(0);
    int c = DiagGetChar(_FALSE);
    if (c > 0)
      ringbuf_put(&input_buf, c);
    LOGUART_SetIMR(IrqEn);
}

void loguart_init0(void) {
  // Do nothing
}

STATIC void log_uart_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    log_uart_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "LOGUART(baudrate=%u, bits=%u, parity=%s, stop=%u, \
      tx_timeout=%u, rx_timeout=%u)",
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
        { MP_QSTR_baudrate,     MP_ARG_OBJ,  {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_bits,         MP_ARG_OBJ,  {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_parity,       MP_ARG_OBJ,  {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_stop,         MP_ARG_OBJ,  {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_tx_timeout,   MP_ARG_OBJ,  {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_rx_timeout,   MP_ARG_OBJ,  {.u_obj = MP_OBJ_NULL} },
    };
    mp_arg_val_t p_args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, args + 1, kw_args, MP_ARRAY_SIZE(allowed_args),
          allowed_args, p_args);

    log_uart_obj_t *self = MP_OBJ_TO_PTR(args[0]);

    if (p_args[ARG_baudrate].u_obj != MP_OBJ_NULL) {
        self->params.baudrate = mp_obj_get_int(p_args[ARG_baudrate].u_int);
    }

    if (p_args[ARG_bits].u_obj != MP_OBJ_NULL) {
        self->params.data_bits = mp_obj_get_int(p_args[ARG_bits].u_int);
    }

    if (p_args[ARG_parity].u_obj != MP_OBJ_NULL) {
        self->params.parity = mp_obj_get_int(p_args[ARG_parity].u_obj);;
    }

    if (p_args[ARG_stop].u_obj != MP_OBJ_NULL) {
        self->params.stop_bits = mp_obj_get_int(p_args[ARG_stop].u_int);
    }

    if (p_args[ARG_tx_timeout].u_obj != MP_OBJ_NULL) {
        self->tx.timeout_ms = mp_obj_get_int(p_args[ARG_tx_timeout].u_int);
    }

    if (p_args[ARG_rx_timeout].u_obj != MP_OBJ_NULL) {
        self->rx.timeout_ms = mp_obj_get_int(p_args[ARG_rx_timeout].u_int);
    }

    DIAG_UartReInit((IRQ_FUN) mp_obj_log_uart_irq_handler);

    NVIC_SetPriority(UART_LOG_IRQ, 10); /* this is rom_code_patch */

    UART_INTConfig(UART2_DEV, RUART_IER_ERBI | RUART_IER_ELSI, DISABLE);

	  UART_RxCmd(UART2_DEV, DISABLE);
	  
	  UART_SetBaud(UART2_DEV, self->params.baudrate);
	  
	  UART_INTConfig(UART2_DEV, RUART_IER_ERBI | RUART_IER_ELSI, ENABLE);

	  UART_RxCmd(UART2_DEV, ENABLE);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(log_uart_init_obj, 1, log_uart_init0);

STATIC mp_obj_t log_uart_deinit(mp_obj_t *self_in) {
    log_uart_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(log_uart_deinit_obj, log_uart_deinit);

STATIC const mp_map_elem_t log_uart_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_init),        MP_OBJ_FROM_PTR(&log_uart_init_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_deinit),      MP_OBJ_FROM_PTR(&log_uart_deinit_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_read),        MP_OBJ_FROM_PTR(&mp_stream_read_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_readline),    MP_OBJ_FROM_PTR(&mp_stream_unbuffered_readline_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_readinto),    MP_OBJ_FROM_PTR(&mp_stream_readinto_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_write),       MP_OBJ_FROM_PTR(&mp_stream_write_obj) },
};
STATIC MP_DEFINE_CONST_DICT(log_uart_locals_dict, log_uart_locals_dict_table);

STATIC mp_uint_t log_uart_read(mp_obj_t self_in, char *buf_in, mp_uint_t size, int *errcode) {
    log_uart_obj_t *self = MP_OBJ_TO_PTR(self_in);

    // Direct return 0 when size = 0, to save the time
    if (size == 0) 
        return 0;

    int32_t ret = 0;

    mp_uint_t start = mp_hal_ticks_ms();
    for (mp_uint_t i = 0; i < size; i++) {
        int c = 0;
        do {
          c = ringbuf_get(&input_buf);
          if ((mp_hal_ticks_ms() - start) > self->rx.timeout_ms) {
            *errcode = MP_ETIMEDOUT;
            goto ret;
          }
        } while(c <= 0);
        ret += 1;
        buf_in[i] = c;
    }

ret:
    if (ret == 0) {
        return MP_STREAM_ERROR;
    } else {
        return ret;
    }
}

STATIC mp_uint_t log_uart_write(mp_obj_t self_in, const char *buf_in, mp_uint_t size, int *errcode) {
    log_uart_obj_t *self = MP_OBJ_TO_PTR(self_in);

    int32_t ret = 0;

    for (mp_uint_t i = 0; i < size; i++) {
        LOGUART_PutChar(buf_in[i]);
        // A workaround, it seems log uart's FIFO is not working ...
        ret += 1;
    }

ret:
    if (ret == 0) {
        return MP_STREAM_ERROR;
    } else {
        return ret;
    }
}

STATIC mp_uint_t log_uart_ioctl(mp_obj_t self_in, mp_uint_t request, mp_uint_t arg, int *errcode) {
    log_uart_obj_t *self = self_in;
    mp_uint_t ret;
    if (request == MP_STREAM_POLL) {
        mp_uint_t flags = arg;
        ret = 0;
        mp_uint_t status = 0;
        ringbuf_t *r = &input_buf;
        status = (r->iget == r->iput) ? false:true;
        // Only return none zero when RX FIFO is not empty
        if ((flags & MP_STREAM_POLL_RD) && (status & true)) {
            ret |= MP_STREAM_POLL_RD;
        }

        // There's no LOGUART TX API to check if it writeable, so always set true
        status = true;
        if ((flags & MP_STREAM_POLL_WR) && (status & true)) {
            ret |= MP_STREAM_POLL_WR;
        }
    } else {
        *errcode = MP_EINVAL;
        ret = MP_STREAM_ERROR;
    }
    return ret;
}

STATIC const mp_stream_p_t log_uart_stream_p = {
    .read    = log_uart_read,
    .write   = log_uart_write,
    .ioctl   = log_uart_ioctl,
    .is_text = false,
};

const mp_obj_type_t log_uart_type = {
    { &mp_type_type },
    .name        = MP_QSTR_LOGUART,
    .print       = log_uart_print,
    .make_new    = log_uart_make_new,
    .getiter     = mp_identity_getiter,
    .iternext    = mp_stream_unbuffered_iter,
    .protocol    = &log_uart_stream_p,
    .locals_dict = (mp_obj_dict_t *)&log_uart_locals_dict,
};
