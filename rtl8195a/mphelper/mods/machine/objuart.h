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

#include "pins.h"

#include "serial_api.h"

#define UART_MIN_BAUD_RATE                (4800)
#define UART_DEFAULT_BAUD_RATE            (9600)
#define UART_MAX_BAUD_RATE                (115200)

enum {
    eUART_BAUDRATE_2400    = 2400,
    eUART_BAUDRATE_4800    = 4800,
    eUART_BAUDRATE_9600    = 9600,
    eUART_BAUDRATE_19200   = 19200,
    eUART_BAUDRATE_38400   = 38400,
    eUART_BAUDRATE_57600   = 57600,
    eUART_BAUDRATE_115200  = 115200,
    eUART_BAUDRATE_921600  = 921600,
    eUART_BAUDRATE_1152000 = 115200,
} UartBaudrate;

enum {
    eUART_PARITY_NONE    = 0,
    eUART_PARITY_ODD     = 1,
    eUART_PARITY_EVEN    = 2,
    eUART_PARITY_FORCED1 = 3,
    eUART_PARITY_FORCED0 = 4,
} UartParity;

enum {
    eUART_STOP_1BIT = 1,
    eUART_STOP_2BIT = 2,
} UartStopBits;

enum {
    eUART_DATA_8BITS = 8,
    eUART_DATA_16BITS = 16,
} UartDataBits;

enum {
    eUART_BAUDRATE_2400    = 2400,
    eUART_BAUDRATE_4800    = 4800,
    eUART_BAUDRATE_9600    = 9600,
    eUART_BAUDRATE_19200   = 19200,
    eUART_BAUDRATE_38400   = 38400,
    eUART_BAUDRATE_57600   = 57600,
    eUART_BAUDRATE_115200  = 115200,
    eUART_BAUDRATE_921600  = 921600,
    eUART_BAUDRATE_1152000 = 115200,
} UartBaudrate;

enum {
    eUART_PARITY_NONE    = 0,
    eUART_PARITY_ODD     = 1,
    eUART_PARITY_EVEN    = 2,
    eUART_PARITY_FORCED1 = 3,
    eUART_PARITY_FORCED0 = 4,
} UartParity;

enum {
    eUART_STOP_1BIT = 1,
    eUART_STOP_2BIT = 2,
} UartStopBits;

enum {
    eUART_DATA_8BITS = 8,
    eUART_DATA_16BITS = 16,
} UartDataBits;

extern const mp_obj_type_t uart_type;
extern const mp_obj_dict_t pin_board_pins_locals_dict;

typedef struct {
    mp_obj_base_t base;
    serial_t  obj;
    bool      is_inited;
    uint8_t   unit;
    uint8_t   bits;
    uint8_t   stop;
    uint8_t   parity;
    uint32_t  baudrate;
    uint16_t  timeout;
    uint16_t  timeout_char;
    mp_obj_t  isr_handler;
    pin_obj_t *tx;
    pin_obj_t *rx;
} uart_obj_t;

#endif  // OBJUART_H_
