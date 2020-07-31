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
#ifndef OBJSPI_H_
#define OBJSPI_H_

#include "py/mpstate.h"
#include "py/runtime.h"
#include "py/mphal.h"

#include "objpin.h"

#include "PinNames.h"
#include "extmod/machine_spi.h"
#include "rtl8711b_ssi.h"

#define SPI_DEFAULT_BAUD_RATE           (10000000)

#ifndef MICROPY_PY_MACHINE_SPI_MSB
#define MICROPY_PY_MACHINE_SPI_MSB      (0)
#define MICROPY_PY_MACHINE_SPI_LSB      (1)
#endif

extern const mp_obj_type_t spi_type;

typedef struct {
    mp_obj_base_t base;
    uint8_t   unit;
    uint8_t   bits;
    uint32_t  baudrate;
    uint8_t   pol;
    uint8_t   pha;
    GDMA_InitTypeDef SSITxGdmaInitStruct;
    GDMA_InitTypeDef SSIRxGdmaInitStruct;
} spi_obj_t;

#endif  // OBJSPI_H_
