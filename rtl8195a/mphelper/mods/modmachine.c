/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 * Copyright (c) 2015 Daniel Campora
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
// micropython headers
#include "py/mpstate.h"
#include "py/runtime.h"
#include "py/mphal.h"

// local object headers
#include "machine/objpin.h"
#include "machine/obji2c.h"
#include "machine/objuart.h"
#include "machine/objspi.h"
#include "machine/objadc.h"
#include "machine/objdac.h"
#include "machine/objpwm.h"
#include "machine/objrtc.h"
#include "machine/objsdio.h"

#include "sys_api.h"

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/
STATIC mp_obj_t machine_reset(void) {
    sys_reset();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_reset_obj, machine_reset);

STATIC const mp_map_elem_t machine_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__),      MP_OBJ_NEW_QSTR(MP_QSTR_umachine) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_reset),         (mp_obj_t)&machine_reset_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Pin),           (mp_obj_t)&pin_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_I2C),           (mp_obj_t)&i2c_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_UART),          (mp_obj_t)&uart_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SPI),           (mp_obj_t)&spi_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_ADC),           (mp_obj_t)&adc_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_DAC),           (mp_obj_t)&dac_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PWM),           (mp_obj_t)&pwm_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_RTC),           (mp_obj_t)&rtc_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SDIO),          (mp_obj_t)&sdio_type },
};
STATIC MP_DEFINE_CONST_DICT(machine_module_globals, machine_module_globals_table);

const mp_obj_module_t mp_module_umachine = {
    .base    = { &mp_type_module },
    .name    = MP_QSTR_umachine,
    .globals = (mp_obj_dict_t*)&machine_module_globals,
};
