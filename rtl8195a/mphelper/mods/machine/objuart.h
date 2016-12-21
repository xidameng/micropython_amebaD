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
#ifndef OBJUART_H_
#define OBJUART_H_

#include "objpin.h"

#include "serial_api.h"

#define UART_MIN_BAUD_RATE                (4800)
#define UART_DEFAULT_BAUD_RATE            (9600)
#define UART_MAX_BAUD_RATE                (115200)

extern const mp_obj_type_t uart_type;
extern const PinMap PinMap_UART_TX[];
extern const PinMap PinMap_UART_RX[];

typedef struct {
    mp_obj_base_t base;
    serial_t  obj;
    uint8_t   unit;
    uint8_t   bits;
    uint8_t   stop;
    uint8_t   parity;
    uint32_t  baudrate;
    uint16_t  timeout;
    bool      irq_enabled;
    mp_obj_t  irq_handler;
    pin_obj_t *tx;
    pin_obj_t *rx;
} uart_obj_t;

#endif  // OBJUART_H_
