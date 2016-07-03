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
// micropython headers
#include "py/mpstate.h"
#include "py/runtime.h"
#include "py/mphal.h"

#include "wdt_api.h"

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/

STATIC mp_obj_t wdt_start(mp_obj_t msec_in) {
    mp_int_t msec = mp_obj_get_int(msec_in);
    if (msec > 0) {
        watchdog_init(msec);
    } else {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "millisecond must > 0"));
    }
    watchdog_start();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wdt_start_obj, wdt_start);

STATIC mp_obj_t wdt_stop(void) {
    watchdog_stop();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(wdt_stop_obj, wdt_stop);

STATIC mp_obj_t wdt_refresh(void) {
    watchdog_refresh();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(wdt_refresh_obj, wdt_refresh);

STATIC const mp_map_elem_t wdt_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__),        MP_OBJ_NEW_QSTR(MP_QSTR_wdt) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_start),           (mp_obj_t)&wdt_start_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_stop),            (mp_obj_t)&wdt_stop_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_refresh),         (mp_obj_t)&wdt_refresh_obj },
};
STATIC MP_DEFINE_CONST_DICT(wdt_module_globals, wdt_module_globals_table);

const mp_obj_module_t mp_watchdog_module = {
    .base    = { &mp_type_module },
    .name    = MP_QSTR_wdt,
    .globals = (mp_obj_dict_t*)&wdt_module_globals,
};
