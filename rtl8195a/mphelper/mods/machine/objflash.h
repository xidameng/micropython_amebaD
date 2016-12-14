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

#ifndef _OBJFLASH_H_
#define _OBJFLASH_H_

// Micropython headers
#include "py/obj.h"
#include "py/runtime.h"
#include "lib/fatfs/ff.h"
#include "extmod/fsusermount.h"

// mbed headers
#include "device_lock.h"
#include "flash_api.h"

extern const struct _mp_obj_type_t flash_type;

extern uint32_t __fatfs_start_address__;
extern uint32_t __fatfs_end_address__;
extern uint32_t __fatfs_num_blocks__;

typedef struct {
    mp_obj_base_t base;
    flash_t       obj;
} flash_obj_t;

void flash_init0(fs_user_mount_t *vfs);

#endif /* _OBJFLASH_H_ */
