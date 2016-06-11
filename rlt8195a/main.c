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
#include "lib/utils/pyexec.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/compile.h"
#include "py/gc.h"

#include "gccollect.h"
#include "gchelper.h"
#include "exception.h"

#include "lib/fatfs/ff.h"
#include "extmod/fsusermount.h"
#include "extmod/vfs_fat_file.h"
#include "cmsis_os.h"
#include "sys_api.h"

#include "flash.h"

#include "flash_api.h"


/*****************************************************************************
 *                              Internal variables
 * ***************************************************************************/
osThreadId main_tid = 0;
fs_user_mount_t fs_user_mount_flash;
static const char fresh_main_py[] =
"# main.py -- put your code here!\r\n"
;

/*****************************************************************************
 *                              External functions
 * ***************************************************************************/
void main_task(void const *arg) {
    if (pyexec_friendly_repl() != 0) {
        DiagPrintf("Soft reset\r\n");
        sys_reset();
    }
}

int main(void)
{
    // Get ARM's stack pointer
    uint32_t sp = gc_helper_get_sp();
    gc_collect_init (sp);
    // Init micropython gc from mp_heap_head to mp_heap_end
    gc_init(&_mp_heap_head, &_mp_heap_end);
    
    // Kernel initialization
    osKernelInitialize();

    // Init micropython basic system
    mp_init();
    mp_obj_list_init(mp_sys_path, 0);
    mp_obj_list_init(mp_sys_argv, 0);
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR_)); 

    mp_hal_log_uart_init();
    pin_init0();
    ip_init0();
    rtc_init0();
    // wifi should br init after lwip init, or it will hang.
    wlan_init0(); 
    mpexception_init0();
    readline_init0();
    flash_vfs_init0();
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_flash));
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_flash_slash_lib));
    DiagPrintf("Starting executing main.py\n");

    const uint8_t *main_py = "main.py";
    FRESULT res = f_stat(main_py, NULL);
    if (res == FR_OK) {
        int32_t ret = pyexec_file(main_py);
        if (!ret) {
            DiagPrintf("main.py executing error");
        }
    } else {
        DiagPrintf("main.py not found, skipping\n");
    }

    DiagPrintf("Starting main task\n");
    // Create main task
    osThreadDef(main_task, osPriorityRealtime, 1, 8152);
    main_tid = osThreadCreate (osThread (main_task), NULL);
    osKernelStart();
    while(1);
    return 0;
}

void NORETURN __fatal_error(const char *msg) {
    for (volatile uint delay = 0; delay < 10000000; delay++) {
    }
    mp_hal_stdout_tx_strn("\nFATAL ERROR:\n", 14);
    mp_hal_stdout_tx_strn(msg, strlen(msg));
    for (uint i = 0;;) {
        for (volatile uint delay = 0; delay < 10000000; delay++) {
        }
    }
}

void flash_vfs_init0(void) {
    fs_user_mount_t *vfs = &fs_user_mount_flash;
    vfs->str = "/flash";
    vfs->len = 6;
    vfs->flags = 0;
    flash_init0(vfs);
// put the flash device in slot 0 (it will be unused at this point)
    MP_STATE_PORT(fs_user_mount)[0] = vfs;

    FRESULT res = f_mount(&vfs->fatfs, vfs->str, 1);

    if (res == FR_NO_FILESYSTEM) {
        res = f_mkfs("/flash", 0, 0);
        if (res == FR_OK) {
            // success creating fresh
        } else {
            MP_STATE_PORT(fs_user_mount)[0] = NULL;
            return;
        }
        // create empty main.py
        FIL fp;
        res = f_open(&fp, "/flash/main.py", FA_WRITE | FA_CREATE_ALWAYS);
        UINT n;
        res = f_write(&fp, fresh_main_py, sizeof(fresh_main_py) - 1 /* don't count null terminator */, &n);
        // TODO check we could write n bytes
        f_close(&fp);
    } else if (res == FR_OK) {
        // mount successful
    } else {
        __fatal_error("PYB: can't mount flash\n");
        MP_STATE_PORT(fs_user_mount)[0] = NULL;
        return;
    }
}

void nlr_jump_fail(void *val) {
    DiagPrintf("FATAL: uncaught exception %p\n", val);
    mp_obj_print_exception(&mp_plat_print, (mp_obj_t)val);
    __fatal_error("error");
}

mp_import_stat_t mp_import_stat(const char *path) {
    return fat_vfs_import_stat(path);
}

mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    return fat_vfs_lexer_new_from_file(filename);
}

MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, fatfs_builtin_open);
