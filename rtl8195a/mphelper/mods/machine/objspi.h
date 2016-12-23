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

#include "exception.h"

#include "objpin.h"

#include "spi_api.h"
#include "spi_ex_api.h"
#include "PinNames.h"

#define SPI_MIN_BAUD_RATE               (10000000)
#define SPI_DEFAULT_BAUD_RATE           (20000000)
#define SPI_MAX_BAUD_RATE               (40000000)

#define SPI_MASTER                      (0)
#define SPI_SLAVE                       (1)

#define SPI_MSB                         (0)
#define SPI_LSB                         (1)

extern const mp_obj_type_t spi_type;
extern const PinMap PinMap_SPI_MOSI[];
extern const PinMap PinMap_SPI_MISO[];

typedef struct {
    mp_obj_base_t base;
    spi_t     obj;
    uint8_t   unit;
    uint8_t   mode;
    uint8_t   bits;
    uint32_t  baudrate;
    uint8_t   pol;
    uint8_t   pha;
    pin_obj_t *clk;
    pin_obj_t *miso;
    pin_obj_t *mosi;
    pin_obj_t *cs;
} spi_obj_t;

#endif  // OBJSPI_H_
