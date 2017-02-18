/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Chester Tseng
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
#include "py/ringbuf.h"
#include "lib/utils/interrupt_char.h"
#include "pyexec.h"

#include "modterm.h"

#include "objloguart.h"

/*****************************************************************************
 *                              Internal variables
 * ***************************************************************************/
#define MODTERM_RX_STACK_DEPTH  512
mp_map_t mp_terminal_map;
QueueHandle_t xRxQueue;

typedef struct _poll_obj_t {
    mp_obj_t obj;
    mp_uint_t (*ioctl)(mp_obj_t obj, mp_uint_t request, mp_uint_t arg, int *errcode);
    mp_uint_t (*read)(mp_obj_t obj, void *buf_in, mp_uint_t size, int *errcode);
    mp_uint_t (*write)(mp_obj_t obj, const void *buf_in, mp_uint_t size, int *errcode);
    mp_uint_t flags;
    mp_uint_t flags_ret;
} poll_obj_t;

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/
STATIC void modterm_map_add(mp_map_t *poll_map, const mp_obj_t *obj, mp_uint_t obj_len, mp_uint_t flags, bool or_flags) {
    for (mp_uint_t i = 0; i < obj_len; i++) {
        mp_map_elem_t *elem = mp_map_lookup(poll_map, mp_obj_id(obj[i]), MP_MAP_LOOKUP_ADD_IF_NOT_FOUND);
        if (elem->value == NULL) {
            // object not found; get its ioctl and add it to the poll list
            const mp_stream_p_t *stream_p = mp_get_stream_raise(obj[i], MP_STREAM_OP_IOCTL);
            poll_obj_t *poll_obj = m_new_obj(poll_obj_t);
            poll_obj->obj = obj[i];
            poll_obj->ioctl = stream_p->ioctl;
            poll_obj->read  = stream_p->read;
            poll_obj->write = stream_p->write;
            poll_obj->flags = flags;
            poll_obj->flags_ret = 0;
            elem->value = poll_obj;
        } else {
            // object exists; update its flags
            if (or_flags) {
                ((poll_obj_t*)elem->value)->flags |= flags;
            } else {
                ((poll_obj_t*)elem->value)->flags = flags;
            }
        }
    }
}

STATIC mp_uint_t terminal_map_poll(mp_map_t *poll_map, mp_uint_t *rwx_num) {
    mp_uint_t n_ready = 0;
    for (mp_uint_t i = 0; i < poll_map->alloc; ++i) {
        if (!MP_MAP_SLOT_IS_FILLED(poll_map, i)) {
            continue;
        }

        poll_obj_t *poll_obj = (poll_obj_t*)poll_map->table[i].value;
        int errcode;
        mp_int_t ret = poll_obj->ioctl(poll_obj->obj, MP_STREAM_POLL, poll_obj->flags, &errcode);
        poll_obj->flags_ret = ret;

#if 0 // Prevent from doing nlr in poll event, because RX will be in another thread
        if (ret == -1) {
            // error doing ioctl
            mp_raise_OSError(errcode);
        }
#endif
        if (ret != 0) {
            n_ready += 1;
            if (rwx_num != NULL) {
                if (ret & MP_STREAM_POLL_RD) {
                    rwx_num[0] += 1;
                }
                if (ret & MP_STREAM_POLL_WR) {
                    rwx_num[1] += 1;
                }
                if ((ret & ~(MP_STREAM_POLL_RD | MP_STREAM_POLL_WR)) != 0) {
                    rwx_num[2] += 1;
                }
            }
        }
    }
    return n_ready;
}

void mp_term_tx_strn(char *str, size_t len) {
    mp_uint_t n_ready = terminal_map_poll(&mp_terminal_map, NULL);
    if (n_ready > 0) {
        for (mp_uint_t i = 0; i < mp_terminal_map.alloc; ++i) {
            if (!MP_MAP_SLOT_IS_FILLED(&mp_terminal_map, i)) {
                continue;
            }
            poll_obj_t *poll_obj = (poll_obj_t*)mp_terminal_map.table[i].value;
            int errcode;
            if (poll_obj->flags_ret & MP_STREAM_POLL_WR) {
                mp_int_t ret = poll_obj->write(poll_obj->obj, str, len, &errcode);
                // Do something with ret
            }
        }
    }
}

int mp_term_rx_chr() {
    char chr = 0;
    if (xQueueReceive(xRxQueue, &chr, portMAX_DELAY) != pdPASS) {

    } else {
        return chr;
    }
}

void modterm_rx_thread(void *arg) {
    for(;;) {
        int chr = 0;
        mp_uint_t n_ready = terminal_map_poll(&mp_terminal_map, NULL);
        if (n_ready > 0) {
            for (mp_uint_t i = 0; i < mp_terminal_map.alloc; ++i) {
                if (!MP_MAP_SLOT_IS_FILLED(&mp_terminal_map, i)) {
                    continue;
                }
                poll_obj_t *poll_obj = (poll_obj_t*)mp_terminal_map.table[i].value;
                int errcode;
                if (poll_obj->flags_ret & MP_STREAM_POLL_RD) {
                    
                    mp_int_t ret = poll_obj->read(poll_obj->obj, &chr, 1, &errcode);
                    // Do something with ret
                    if (chr == mp_interrupt_char)
                        mp_keyboard_interrupt();
                    else {
                        xQueueSendToBack(xRxQueue, (void *)&chr, portMAX_DELAY);
                    }
                }
            }
        }
        // Should use queue instead of delay to save cpu resource
        mp_hal_delay_ms(1);
    }
    vTaskDelete(NULL);
}

void modterm_init (void) {
    mp_map_init(&mp_terminal_map, 0);
#if 0
    // Create REPL RX task
    TaskHandle_t xReturn = xTaskGenericCreate(modterm_rx_thread,
            (signed char *)"TERM_RX", 
            MODTERM_RX_STACK_DEPTH, 
            NULL,
            MICROPY_TASK_PRIORITY + 1,
            NULL,               // No arguments to pass to mp thread
            mpREPLRXStack,      // Use user define stack memory to make it predictable.
            NULL);
#else
    TaskHandle_t xReturn = xTaskCreate(modterm_rx_thread,
            (signed char*)"TERM_RX",
            MODTERM_RX_STACK_DEPTH,
            NULL,
            MICROPY_TASK_PRIORITY + 1,
            NULL);
#endif

    if (xReturn != pdTRUE)
        mp_printf(&mp_plat_print, "Create %s task failed", "TERM_RX");

    // Create Queue to sync RX buffer
    xRxQueue = xQueueCreate(256, sizeof(char));
}

STATIC mp_obj_t term_register(mp_obj_t obj_in) {
    modterm_map_add(&mp_terminal_map, &obj_in, 1, MP_STREAM_POLL_RD | MP_STREAM_POLL_WR, false);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(term_register_obj, term_register);

STATIC mp_obj_t term_unregister(mp_obj_t obj_in) {
    mp_map_lookup(&mp_terminal_map, mp_obj_id(obj_in), MP_MAP_LOOKUP_REMOVE_IF_FOUND);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(term_unregister_obj, term_unregister);

STATIC const mp_map_elem_t uterminal_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__),   MP_OBJ_NEW_QSTR(MP_QSTR_uterm) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_register),   MP_OBJ_FROM_PTR(&term_register_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_unregister), MP_OBJ_FROM_PTR(&term_unregister_obj) },
};
STATIC MP_DEFINE_CONST_DICT(uterminal_module_globals, uterminal_module_globals_table);

const mp_obj_module_t mp_module_uterminal = {
    .base    = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&uterminal_module_globals,
};
