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

#include "rtc_api.h"
#include "timeutils.h"

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/

void rtc_init0(void) {
    rtc_init();
}

STATIC mp_obj_t time_localtime(mp_uint_t n_args, const mp_obj_t *args) {
    time_t datetime;
    struct tm *timeinfo_read;
    if (n_args == 0 || args[0] == mp_const_none) {
        datetime = rtc_read();
        timeinfo_read = localtime(&datetime);
        mp_obj_t tuple[8] = {
            mp_obj_new_int(timeinfo_read->tm_year + 1900),
            mp_obj_new_int(timeinfo_read->tm_mon),
            mp_obj_new_int(timeinfo_read->tm_mday),
            mp_obj_new_int(timeinfo_read->tm_hour),
            mp_obj_new_int(timeinfo_read->tm_min),
            mp_obj_new_int(timeinfo_read->tm_sec),
            mp_obj_new_int(timeinfo_read->tm_wday),
            mp_obj_new_int(timeutils_year_day(timeinfo_read->tm_year, timeinfo_read->tm_mon, timeinfo_read->tm_mday)),
        };
        return mp_obj_new_tuple(8, tuple);
    } else {
        mp_obj_t *items;
        struct tm timeinfo_write;
        mp_obj_get_array_fixed_n(args[0], 8, &items);
        timeinfo_write.tm_year = mp_obj_get_int(items[0]) - 1900;
        timeinfo_write.tm_mon = mp_obj_get_int(items[1]);
        timeinfo_write.tm_mday = mp_obj_get_int(items[2]);
        timeinfo_write.tm_hour = mp_obj_get_int(items[3]);
        timeinfo_write.tm_min = mp_obj_get_int(items[4]);
        timeinfo_write.tm_sec = mp_obj_get_int(items[5]);
        timeinfo_write.tm_wday = mp_obj_get_int(items[6]);
        timeinfo_write.tm_yday = mp_obj_get_int(items[7]);
        datetime = mktime(&timeinfo_write);
        rtc_write(datetime);
        return mp_const_none;
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(time_localtime_obj, 0, 1, time_localtime);

STATIC mp_obj_t time_mktime(mp_obj_t tuple) {

    mp_uint_t len;
    mp_obj_t *elem;

    mp_obj_get_array(tuple, &len, &elem);

    // localtime generates a tuple of len 8. CPython uses 9, so we accept both.
    if (len < 8 || len > 9) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_TypeError, "mktime needs a tuple of length 8 or 9 (%d given)", len));
    }

    return mp_obj_new_int_from_uint(timeutils_mktime(mp_obj_get_int(elem[0]),
            mp_obj_get_int(elem[1]), mp_obj_get_int(elem[2]), mp_obj_get_int(elem[3]),
            mp_obj_get_int(elem[4]), mp_obj_get_int(elem[5])));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(time_mktime_obj, time_mktime);

STATIC mp_obj_t time_ctime(void) {
    time_t seconds;
    int8_t *ctime_val;
    seconds = rtc_read();
    ctime_val = ctime(&seconds);
    return mp_obj_new_str(ctime_val, strlen(ctime_val), false);;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(time_ctime_obj, time_ctime);


STATIC mp_obj_t time_sleep(mp_obj_t seconds_o) {
#if MICROPY_PY_BUILTINS_FLOAT
    if (MP_OBJ_IS_INT(seconds_o)) {
#endif
        mp_hal_delay_ms(1000 * mp_obj_get_int(seconds_o));
#if MICROPY_PY_BUILTINS_FLOAT
    } else {
        mp_hal_delay_ms((uint32_t)(1000 * mp_obj_get_float(seconds_o)));
    }
#endif
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(time_sleep_obj, time_sleep);

STATIC mp_obj_t time_sleep_ms(mp_obj_t ms_in) {
    mp_int_t ms = mp_obj_get_int(ms_in);
    if (ms > 0) {
        mp_hal_delay_ms(ms);
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(time_sleep_ms_obj, time_sleep_ms);

STATIC mp_obj_t time_sleep_us(mp_obj_t usec_in) {
    mp_int_t usec = mp_obj_get_int(usec_in);
    if (usec > 0) {
        mp_hal_delay_us(usec);
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(time_sleep_us_obj, time_sleep_us);

STATIC const mp_map_elem_t time_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__),        MP_OBJ_NEW_QSTR(MP_QSTR_time) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_localtime),       (mp_obj_t)&time_localtime_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_mktime),          (mp_obj_t)&time_mktime_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_ctime),           (mp_obj_t)&time_ctime_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_sleep),           (mp_obj_t)&time_sleep_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_sleep_ms),        (mp_obj_t)&time_sleep_ms_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_sleep_us),        (mp_obj_t)&time_sleep_us_obj },
};
STATIC MP_DEFINE_CONST_DICT(time_module_globals, time_module_globals_table);

const mp_obj_module_t mp_time_module = {
    .base    = { &mp_type_module },
    .name    = MP_QSTR_time,
    .globals = (mp_obj_dict_t*)&time_module_globals,
};
