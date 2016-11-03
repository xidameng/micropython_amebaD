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
#include "py/mpconfig.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/compile.h"
#include "py/gc.h"

#include "lib/utils/pyexec.h"

#include "gccollect.h"
#include "gchelper.h"
#include "exception.h"

#include "lib/fatfs/ff.h"
#include "extmod/fsusermount.h"

#include "sys_api.h"

#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "main.h"
#include <example_entry.h>

/*****************************************************************************
 *                              Internal variables
 * ***************************************************************************/
osThreadId ftpd_tid = 0;

void main_task(void const *arg) {
    if (pyexec_friendly_repl() != 0) {
        sys_reset();
    }
}

void ftpd_task(void const *arg) {
    ftpd_init();
    for(;;);
}

void main (void) {
    uint32_t sp = gc_helper_get_sp();
    gc_collect_init (sp);
    gc_init(&_mp_gc_head, &_mp_gc_end);

    // Init micropython basic system
    mp_init();
    mp_obj_list_init(mp_sys_path, 0);
    mp_obj_list_init(mp_sys_argv, 0);
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR_)); 
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_flash));
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_flash_slash_lib));
    netif_init0();
    rtc_init0();
    crypto_init0();
    wlan_init0();
    term_init();
    MP_STATE_PORT(mp_kbd_exception) = mp_obj_new_exception(&mp_type_KeyboardInterrupt);
    // Create main task
    xTaskCreate( main_task, (signed char*)"Task1", MICROPY_MAIN_TASK_STACK_SIZE, NULL, MICROPY_MAIN_TASK_PRIORITY, NULL );
    vTaskStartScheduler();
    for(;;);
    return;
}

void nlr_jump_fail(void *val) {
    mp_printf(&mp_plat_print, "FATAL: uncaught exception %p\r\n", val);
    for (uint i = 0;;) {
        for (volatile uint delay = 0; delay < 10000000; delay++);
    }
}

mp_import_stat_t mp_import_stat(const char *path) {
    return fat_vfs_import_stat(path);
}

mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    return fat_vfs_lexer_new_from_file(filename);
}

mp_obj_t mp_builtin_open(uint n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    return fatfs_builtin_open(n_args, args, kwargs);
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

mp_obj_t mp_builtin_ftpd(mp_obj_t enable_in) {
    bool enable = mp_obj_is_true(enable_in);
    if (enable) {
        if (ftpd_tid != NULL) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "Thread has been created"));
        }
        osThreadDef(ftpd_task, MICROPY_FTPD_TASK_PRIORITY, 1, MICROPY_FTPD_STACK_SIZE);
        ftpd_tid = osThreadCreate (osThread (ftpd_task), NULL);
    } else {
        if (ftpd_tid == NULL) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "Thread has not created"));
        }
        osStatus status = osThreadTerminate(ftpd_tid);
        if (status == osOK) {
            ftpd_tid = NULL;
        } else {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "Thread terminate failed"));
        }
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_builtin_ftpd_obj, mp_builtin_ftpd);
