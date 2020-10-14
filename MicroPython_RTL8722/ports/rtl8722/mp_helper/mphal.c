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
#include <stdio.h>

#include "py/mpstate.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "py/mpstate.h"
#include "extmod/misc.h"
#include "lib/utils/pyexec.h"
#include "py/obj.h"
#include "py/ringbuf.h"
#include "wait_api.h"
#include "serial_api.h"
#include <stdio.h>
#include "lib/utils/interrupt_char.h"
//#include "osdep_api.h"  // xxm


TaskHandle_t mp_main_task_handle;


extern serial_t    uartobj;
//extern serial_t    sobj;

void uart_send_string(serial_t *uartobj, char *pstr)
{
    unsigned int i=0;

    while (*(pstr+i) != 0) {
        serial_putc(uartobj, *(pstr+i));
        i++;
    }
}


void uart_send_string_with_length(serial_t *uartobj, char *pstr, size_t len)
{
       for (uint32_t i = 0; i < len; ++i) {
        serial_putc(uartobj, pstr[i]);
    }
}


///////////////////////////////
//       HAL TX & RX         //
///////////////////////////////
int mp_hal_stdin_rx_chr(void) {
    return serial_getc(&uartobj);
}

void mp_hal_stdout_tx_strn(const char *str, size_t len) {
    //printf("--mp_hal_stdout_tx_strn--\n");
    //mp_term_tx_strn(str, len);
    uart_send_string_with_length(&uartobj, str, len);
}

void mp_hal_stdout_tx_chr(char c) {
    //printf("mp_hal_stdout_tx_chr\n");
    //mp_term_tx_strn(&c, 1);
    serial_putc(&uartobj, (int)c);
}

void mp_hal_stdout_tx_str(const char *str) {
    //printf("mp_hal_stdout_tx_str\n");
    //mp_term_tx_strn(str, strlen(str));
    uart_send_string(&uartobj, str);
}

void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len) {
    while (len--) {
        if (*str == '\n') {
            mp_hal_stdout_tx_chr('\r');
        }
        //printf("mp_hal_stdout_tx_strn_cooked\n");
        mp_hal_stdout_tx_chr(*str++);
    }
}


///////////////////////////////
//       Delay & Time        //
///////////////////////////////
void mp_hal_delay_ms(uint32_t ms) {
    wait_ms(ms);
}

void mp_hal_delay_us(uint32_t us) {
    wait_us(us);
}

uint32_t mp_hal_ticks_ms(void) {
    return rtw_get_current_time();
}

// Wake up the main task if it is sleeping
void mp_hal_wake_main_task_from_isr(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(mp_main_task_handle, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR(pdTRUE);
    }
}
