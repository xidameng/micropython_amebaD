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
#include "py/mpstate.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "py/mpstate.h"
#include "extmod/misc.h"
#include "lib/utils/pyexec.h"
#include "py/obj.h"
#include "py/ringbuf.h"

#include <stdio.h>
#include "osdep_api.h"
#include "device.h"
#include "log_uart_api.h"

static log_uart_t log_uart_obj;
STATIC uint8_t input_buf_array[256];
ringbuf_t input_buf = {input_buf_array, sizeof(input_buf_array)};
int interrupt_char;

void log_uart_irq_service(void *arg) {
    int c = log_uart_getc(&log_uart_obj);
    if (c == interrupt_char) {
        mp_keyboard_interrupt();
    } else {
        ringbuf_put(&input_buf, c);
    }
}

void log_uart_init0(void)
{
    log_uart_init(&log_uart_obj, 38400, 8, ParityNone, 1);
    log_uart_irq_handler(&log_uart_obj, log_uart_irq_service, 1);
    log_uart_irq_set(&log_uart_obj, IIR_RX_RDY, true);
}

int mp_hal_stdin_rx_chr(void) {
    for (;;) {
        int c = ringbuf_get(&input_buf);
        if (c != -1) {
            return c;
        }
        mp_hal_delay_us(1);
    }
}

void mp_hal_stdout_tx_char(char c) {
    log_uart_putc(&log_uart_obj, c);
    mp_uos_dupterm_tx_strn(&c, 1);
}

void mp_hal_stdout_tx_str(const char *str) {
    while (*str) {
        mp_hal_stdout_tx_char(*str++);
    }
}

void mp_hal_stdout_tx_strn(const char *str, size_t len) {
    while (len--) {
        mp_hal_stdout_tx_char(*str++);
    }
}

void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len) {
    while (len--) {
        if (*str == '\n') {
            mp_hal_stdout_tx_char('\r');
        }
        mp_hal_stdout_tx_char(*str++);
    }
}

void mp_hal_set_interrupt_char(int c) {
    //mpexception_set_interrupt_char (c);
}

void mp_hal_delay_ms(uint32_t ms) {
    mdelay(ms);
}

void mp_hal_delay_us(uint32_t us) {
    udelay(us);
}

uint32_t mp_hal_ticks_ms(void) {
    // TODO
    return 0;
}


