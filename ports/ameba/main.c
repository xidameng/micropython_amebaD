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

/*#include "py/mpconfig.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/compile.h"
#include "py/gc.h"
#include "lib/utils/pyexec.h"
#include "gccollect.h"
#include "exception.h"
*/
#include "section_config.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "osdep_service.h"
//#include "serial_api.h"
//#include "ringbuffer.h"
#if 0
extern serial_t log_uart_obj;
extern void mp_loguart_irq_handler(uint32_t id, SerialIrq event);
extern ringbuf_t *_rx_buffer;
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern uint32_t                     DiagPrintf(const char *fmt, ...);
extern int                          _rtl_printf(const char *fmt, ...);
extern int                          _rtl_sprintf(char* str, const char* fmt, ...);
#ifndef printf
#define printf                      _rtl_printf
#endif
#ifndef sprintf
#define sprintf                     _rtl_sprintf
#endif

#if 0
/*****************************************************************************
 *                              Internal variables
 * ***************************************************************************/

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/
uint8_t mpHeap[MP_HEAP_SIZE];

void micropython_task(void const *arg) {
    printf("--Test 01--\n");
    mp_stack_ctrl_init();
#if MICROPY_ENABLE_GC
    gc_init(mpHeap, mpHeap + sizeof(mpHeap));
    printf("--Test gc--\n");
#endif
    printf("--Test 02--\n");
    // Init micropython basic system
    mp_init();
    printf("--Test 03--mp init--\n");
    mp_obj_list_init(mp_sys_path, 0);
    mp_obj_list_init(mp_sys_argv, 0);
    printf("--Test 04--\n");
    modmachine_init();
    //loguart_init0();
    printf("--Test 05--loguart init--\n");
    //modnetwork_init();
    printf("--Test 06--\n");
    //modwireless_init();
    printf("--Test 07--\n");
    modterm_init();
    printf("--Test 08--term init--\n");
    pyexec_frozen_module("_boot.py");
#if MICROPY_REPL_EVENT_DRIVEN
    pyexec_event_repl_init();
    printf("--Test 09--\n");
#endif
    for ( ; ; ) {
        if (pyexec_mode_kind == PYEXEC_MODE_RAW_REPL) {
            printf("--Test 10--\n");
            if (pyexec_raw_repl() != 0)
                mp_printf(&mp_plat_print, "soft reboot\n");
        } else {
            printf("--Test 11--\n");
            if (pyexec_friendly_repl() != 0) 
                mp_printf(&mp_plat_print, "soft reboot\n");
        }
    }
    printf("--Test 12--\n");
    rtw_thread_exit();
    printf("--Test 13--\n");
}

#endif

int main (void) {

    //__libc_init_array();
    printf("--Test 00--\n");
    printf("--Test 00--\n");
    printf("--Test 00--\n");
    printf("--Test 00--\n");
    printf("--Test 00--\n");
    printf("--Test 00--\n");
    printf("--Test 00--\n");
    printf("--Test 00--\n");
    printf("--Test 00--\n");
    printf("--Test 00--\n");
    printf("--Test 00--\n");
    printf("--after--uart-init--\n");
    printf("--after--uart-init--\n");
    printf("--after--uart-init--\n");
    printf("--after--uart-init--\n");
    printf("--after--uart-init--\n");
    printf("--after--uart-init--\n");
    printf("--after--uart-init--\n");
    printf("--after--uart-init--\n");
    printf("--after--uart-init--\n");
    printf("--after--uart-init--\n");
    #if 0
    struct task_struct stUpyTask;
    BaseType_t xReturn = rtw_create_task(&stUpyTask, MICROPY_TASK_NAME,
            MICROPY_TASK_STACK_DEPTH, MICROPY_TASK_PRIORITY, micropython_task, NULL);
    printf("--Test 00--after--taskcreate--\n");
    
    printf("--Test 00--after--scheduler--\n");
    for(;;);
    #endif

#if 0
    ringbuf_init();
    //serial_init(&log_uart_obj, PA_7, PA_8);
    serial_init(&log_uart_obj, PB_19, PB_18);
    //printf("inti log uart-1\n");
    serial_format(&log_uart_obj, 8, ParityNone, 1);
    //printf("inti log uart-2\n");
    serial_baud(&log_uart_obj, 115200);
    //printf("inti log uart-3\n");
    //serial_irq_handler(&log_uart_obj, mp_loguart_irq_handler, (uint32_t)_rx_buffer);
    //printf("inti log uart-4\n");
    serial_irq_handler(&log_uart_obj, mp_loguart_irq_handler, (uint32_t)_rx_buffer);
    serial_irq_set(&log_uart_obj, RxIrq, 1);
#endif

    //vTaskStartScheduler();
    while(1);
    return 0;

}
#if 0
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
    while(1);
}
#endif