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
#include "py/mpconfig.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/objarray.h"
#include "py/stream.h"
#include "py/objstr.h"
#include "lib/utils/interrupt_char.h"

#include "objloguart.h"
/*****************************************************************************
 *                              Internal variables
 * ***************************************************************************/

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/
void term_init (void) {
    // Init terminal here   
    mp_obj_list_init(&MP_STATE_PORT(term_list_obj), 0);
    MP_STATE_PORT(dupterm_arr_obj) = mp_obj_new_bytearray(1, "");

    // Append the singleton LOGUART object to terminal list in default
    mp_obj_list_append(&MP_STATE_PORT(term_list_obj), log_uart_type.make_new(&log_uart_type, 0, 0, MP_OBJ_NULL));
}

void mp_term_tx_strn(const char *str, size_t len) {
    if (&MP_STATE_PORT(term_list_obj) != MP_OBJ_NULL) {
        // Send character to every element in term_list_obj
        for (mp_uint_t i = 0; i < MP_STATE_PORT(term_list_obj).len; i++) {
            nlr_buf_t nlr;
            if (nlr_push(&nlr) == 0) {
                mp_obj_t write_m[3];
                mp_load_method(MP_STATE_PORT(term_list_obj).items[i], MP_QSTR_write, write_m);
                mp_obj_array_t *arr = MP_OBJ_TO_PTR(MP_STATE_PORT(dupterm_arr_obj));
                arr->items = (void *)str;
                arr->len = len;
                write_m[2] = MP_STATE_PORT(dupterm_arr_obj);
                mp_call_method_n_kw(1, 0, write_m);
                nlr_pop();
            } else {
                // When exception raised, remove the target object from term_list_obj
                mp_printf(&mp_plat_print, "term: Exception in write() method, deactivating: ");
                if (nlr.ret_val != MP_OBJ_NULL) {
                    mp_obj_print_exception(&mp_plat_print, nlr.ret_val);
                }
                mp_stream_close(MP_STATE_PORT(term_list_obj).items[i]);
                mp_obj_list_remove(&MP_STATE_PORT(term_list_obj), i);
            }
        }
    }
}

int mp_term_rx_chr() {
    if (&MP_STATE_PORT(term_list_obj) != MP_OBJ_NULL) {
        // Send character to every element in term_list_obj
        for (mp_uint_t i = 0; i < MP_STATE_PORT(term_list_obj).len; i++) {
            nlr_buf_t nlr;
            if (nlr_push(&nlr) == 0) {
                mp_obj_t readinto_m[3];
                mp_load_method(MP_STATE_PORT(term_list_obj).items[i], MP_QSTR_readinto, readinto_m);
                readinto_m[2] = MP_STATE_PORT(dupterm_arr_obj);
                mp_obj_t res = mp_call_method_n_kw(1, 0, readinto_m);
                if (res == mp_const_none) {
                    nlr_pop();
                    return -2;
                }
                if (res == MP_OBJ_NEW_SMALL_INT(0)) {
                    nlr_pop();
                    return -1;
                }
                mp_buffer_info_t bufinfo;
                mp_get_buffer_raise(MP_STATE_PORT(dupterm_arr_obj), &bufinfo, MP_BUFFER_READ);
                nlr_pop();
                if (*(byte*)bufinfo.buf == mp_interrupt_char) {
                    mp_keyboard_interrupt();
                    return -2;
                }
                return *(byte*)bufinfo.buf;
            } else {
                // When exception raised, remove the target object from term_list_obj
                mp_printf(&mp_plat_print, "term: Exception in readinto() method, deactivating: ");
                if (nlr.ret_val != MP_OBJ_NULL) {
                    mp_obj_print_exception(&mp_plat_print, nlr.ret_val);
                }
                mp_stream_close(MP_STATE_PORT(term_list_obj).items[i]);
                mp_obj_list_remove(&MP_STATE_PORT(term_list_obj), i);
            }
        }
    }
}

STATIC mp_obj_t term_dump(void) {
    return &MP_STATE_PORT(term_list_obj);
}
MP_DEFINE_CONST_FUN_OBJ_0(term_dump_obj, term_dump);

STATIC mp_obj_t term_tx(mp_obj_t array_in) {
    mp_uint_t len = 0;
    char *str = mp_obj_str_get_data(array_in, &len);
    mp_term_tx_strn(str, len);
    return mp_obj_new_int(len);
}
MP_DEFINE_CONST_FUN_OBJ_1(term_tx_obj, term_tx);

STATIC mp_obj_t term_rx(void) {
    char chr = mp_term_rx_chr();
    return mp_obj_new_bytes(&chr, 1);
}
MP_DEFINE_CONST_FUN_OBJ_0(term_rx_obj, term_rx);

STATIC const mp_map_elem_t uterminal_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__),      MP_OBJ_NEW_QSTR(MP_QSTR_uterm) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dump),          MP_OBJ_FROM_PTR(&term_dump_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_tx),            MP_OBJ_FROM_PTR(&term_tx_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_rx),            MP_OBJ_FROM_PTR(&term_rx_obj) },
};
STATIC MP_DEFINE_CONST_DICT(uterminal_module_globals, uterminal_module_globals_table);

const mp_obj_module_t mp_module_uterminal = {
    .base    = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&uterminal_module_globals,
};
