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
#include "lib/utils/interrupt_char.h"

// mbed lib headers
#include "serial_api.h"
//#include "log_uart_api.h"

#include "objuart.h"
#include "ringbuffer.h"

#ifdef AMEBAD
typedef enum {
    IIR_MODEM_STATUS = 0,   //Clear to send or data set ready or ring indicator or data carrier detect.
    IIR_NO_PENDING = 1,
    IIR_THR_EMPTY = 2,   // TX FIFO level lower than threshold or FIFO empty
    IIR_RX_RDY = 4,     // RX data ready
    IIR_RX_LINE_STATUS = 6, // Overrun/parity/framing errors or break interrupt
    IIR_BUSY = 7,
    IIR_CHAR_TIMEOUT = 12   // timeout: Rx dara ready but no read 
} LOG_UART_INT_ID;
#endif


extern const mp_obj_type_t log_uart_type;

typedef struct {
    mp_obj_base_t   base;
    serial_t        obj;
    UartParams      params;
    UartLane        tx;
    UartLane        rx;
    uint32_t        irq_enabled;
    mp_obj_t        irq_handler;
} log_uart_obj_t;

#endif /* OBJLOGUART_H_ */
