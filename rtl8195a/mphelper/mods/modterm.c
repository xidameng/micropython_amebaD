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
#include "py/misc.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/objarray.h"
#include "py/stream.h"
#include "py/objstr.h"
#include "lib/utils/interrupt_char.h"
#include "pyexec.h"

#include "modterm.h"

#include "objloguart.h"
/*****************************************************************************
 *                              Internal variables
 * ***************************************************************************/
STATIC xQueueHandle xRXCharQueue;

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/
void modterm_init (void) {
    // Init terminal here   
    mp_obj_list_init(&MP_STATE_PORT(term_list_obj), 0);
    MP_STATE_PORT(dupterm_arr_obj) = mp_obj_new_bytearray(1, "");

    xRXCharQueue = xQueueCreate(64, sizeof(struct terminal_msg *));
}

void mp_term_tx_strn(char *str, size_t len) {
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
    mp_not_implemented("mp_term_rx_chr and mp_hal_stdin_rx_chr not implement");
}

void modterminal_rx_loop(void) {

    pyexec_event_repl_init();

    struct terminal_msg *msg;
    for (;;) {
        /* 
         *  Queue is blocking forever until queue is incoming
         *  Blocking state will save CPU resource 
         */
        if (xQueueReceive(xRXCharQueue, (void *)&msg, (TickType_t) portMAX_DELAY)) {
            switch (msg->type) {
                case TERM_RX_CHR:
                    /*
                     * Check if term_list_obj is none or not
                     */
                    if (&MP_STATE_PORT(term_list_obj) == MP_OBJ_NULL) {
                        mp_raise_msg(&mp_type_OSError, "Terminal list object missing, assert");
                        return;
                    }
                    for (mp_uint_t i = 0; i < MP_STATE_PORT(term_list_obj).len; i++) {
                        if (msg->msg.chr.obj_from == MP_STATE_PORT(term_list_obj).items[i]) {

                            mp_buffer_info_t bufinfo;
                            mp_get_buffer_raise(msg->msg.chr.obj_array, &bufinfo, MP_BUFFER_READ);
                            char c = *(byte*)bufinfo.buf;

                            int ret = pyexec_event_repl_process_char(c);

                            if (ret & PYEXEC_FORCED_EXIT)
                                sys_reset();

                        } else {
                            // TODO what to do when obj_from is not in the list
                        }
                    }
                break;
            }
            // Should delete object at final or mpHeap would leak
            m_del_obj(struct terminal_msg *, msg);
        }   
    }
    vTaskDelete(NULL);
}

STATIC mp_obj_t term_dump(void) {
    return &MP_STATE_PORT(term_list_obj);
}
MP_DEFINE_CONST_FUN_OBJ_0(term_dump_obj, term_dump);

STATIC mp_obj_t term_rx_post(mp_obj_t obj_in, mp_obj_t array_in) {

    /*
     *  Check if keyboard interrupt is raised
     */
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(array_in, &bufinfo, MP_BUFFER_READ);
    char c = *(byte*)bufinfo.buf;
    if (c == mp_interrupt_char) {
        mp_keyboard_interrupt();
        return mp_const_none;
    }

    /* 
     * Malloc a queue from the mpHeap, it will be free in the modterminal_rx_loop after
     * the queue is consumed.
     */
    struct terminal_msg *msg = m_new_obj(struct terminal_msg *);

    msg->type = TERM_RX_CHR;
    msg->msg.chr.obj_from = obj_in;
    msg->msg.chr.obj_array = array_in;

    // Check if object have attribute 'irq_handler'
    mp_obj_t dest[2];
    mp_load_method_maybe(obj_in, MP_QSTR_irq_handler, dest);
    
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (dest[0] != MP_OBJ_NULL) {
        /*
         * If you are in an ISR, you need to call **FromISR API in RTOS
         */
        xQueueSendFromISR(xRXCharQueue, (void *)&msg, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
            taskYIELD();
    }
    else {
        // block for a long time if queue is full ...
        xQueueSend(xRXCharQueue, (void *)&msg, (TickType_t) portMAX_DELAY);
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(term_rx_post_obj, term_rx_post);

STATIC const mp_map_elem_t uterminal_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__),   MP_OBJ_NEW_QSTR(MP_QSTR_uterm) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dump),       MP_OBJ_FROM_PTR(&term_dump_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_rx_post),    MP_OBJ_FROM_PTR(&term_rx_post_obj) },
};
STATIC MP_DEFINE_CONST_DICT(uterminal_module_globals, uterminal_module_globals_table);

const mp_obj_module_t mp_module_uterminal = {
    .base    = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&uterminal_module_globals,
};
