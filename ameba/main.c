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

#include "py/mpconfig.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/compile.h"
#include "py/gc.h"

#include "lib/utils/pyexec.h"

#include "gccollect.h"
#include "exception.h"

#include "section_config.h"
#include "FreeRTOS.h"
#include "task.h"

/*****************************************************************************
 *                              Internal variables
 * ***************************************************************************/

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/
extern uint8_t mpHeap[MP_HEAP_SIZE];
extern uint8_t ucHeap[configTOTAL_HEAP_SIZE];
extern StackType_t mpTaskStack[MICROPY_TASK_STACK_DEPTH];

void micropython_task(void const *arg) {

    mp_stack_ctrl_init();

#if MICROPY_PY_THREAD
    mp_thread_init();
#endif
#if MICROPY_ENABLE_GC
    gc_init(mpHeap, mpHeap + sizeof(mpHeap));
#endif
    // Init micropython basic system
    mp_init();
    mp_obj_list_init(mp_sys_path, 0);
    mp_obj_list_init(mp_sys_argv, 0);
    MP_STATE_PORT(mp_kbd_exception) = mp_obj_new_exception(&mp_type_KeyboardInterrupt);
    MP_STATE_PORT(dupterm_arr_obj) = MP_OBJ_NULL;


    modmachine_init();
    modwireless_init();
    modnetwork_init();
    mp_hal_delay_ms(20);
    modterm_init();
    pyexec_frozen_module("_boot.py");
    //pyexec_file("main.py");
#if MICROPY_REPL_EVENT_DRIVEN
    pyexec_event_repl_init();
#endif
    for ( ; ; ) {
        if (pyexec_mode_kind == PYEXEC_MODE_RAW_REPL) {
            if (pyexec_raw_repl() != 0) {
                mp_printf(&mp_plat_print, "soft reboot\n");
            }
        } else {
            if (pyexec_friendly_repl() != 0) {
                mp_printf(&mp_plat_print, "soft reboot\n");
            }
        }
    }
    vTaskDelete(NULL);
}

void main (void) {
    /* 
     * lwip init will use freertos thread, so vPortDefineHeapRegions
     * should be called before lwip init
     */
    HeapRegion_t xHeapRegions[] = {{ ucHeap, sizeof(ucHeap) },{ NULL, 0 }};
    vPortDefineHeapRegions(xHeapRegions);

    // Create MicroPython main task
    TaskHandle_t xReturn = xTaskGenericCreate(micropython_task,
            (signed char *)MICROPY_TASK_NAME, 
            MICROPY_TASK_STACK_DEPTH, 
            NULL,
            MICROPY_TASK_PRIORITY,
            NULL,           // No arguments to pass to mp thread
            mpTaskStack,    // Use user define stack memory to make it predictable.
            NULL);

    if (xReturn != pdTRUE)
        mp_printf(&mp_plat_print, "Create %s task failed", MICROPY_TASK_NAME);

    vTaskStartScheduler();

    for(;;);
    return;
}

#if !MICROPY_VFS
mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    return NULL;
}

mp_import_stat_t mp_import_stat(const char *path) {
    (void)path;
    return MP_IMPORT_STAT_NO_EXIST;
}

mp_obj_t mp_builtin_open(uint n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

#endif

void nlr_jump_fail(void *val) {
    mp_printf(&mp_plat_print, "FATAL: uncaught exception %p\r\n", val);
    for (uint i = 0;;) {
        for (volatile uint delay = 0; delay < 10000000; delay++);
    }
}
