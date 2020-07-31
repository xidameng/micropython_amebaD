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
#ifndef OBJLOGUART_H_
#define OBJLOGUART_H_

#include "py/mpconfig.h"
#include "py/runtime.h"
#include "py/stream.h"
#include "py/objarray.h"
#include "py/ringbuf.h"
#include "lib/utils/interrupt_char.h"
#include "ameba_soc.h"
#include "osdep_service.h"

// mbed lib headers

#include "objuart.h"

extern const mp_obj_type_t log_uart_type;

#define LOGUART_DEFAULT_BAUDRATE    115200
#define LOGUART_DEFAULT_DATA_BITS   8
#define LOGUART_DEFAULT_PARITY      0
#define LOGUART_DEFAULT_STOP_BITS   1
#define LOGUART_DEFAULT_TX_TIMEOUT  10
#define LOGUART_DEFAULT_RX_TIMEOUT  10

typedef struct {
    mp_obj_base_t   base;
    UartParams      params;
    UartLane        tx;
    UartLane        rx;
} log_uart_obj_t;

#endif /* OBJLOGUART_H_ */
