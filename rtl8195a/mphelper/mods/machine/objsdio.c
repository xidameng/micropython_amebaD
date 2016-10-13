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
#include "objsdio.h"
#include "lib/fatfs/ff.h"
#include "extmod/fsusermount.h"

STATIC sdio_obj_t sdio_obj = {
    .base.type = &sdio_type,
};

void sdio_init(void) {
    //TODO Should check ret value
    uint8_t ret = SD_Init();
}

void sdio_deinit(void) {
    //TODO Should check ret value
    uint8_t ret = SD_DeInit();
}

void sdio_flush(void) {
}

mp_uint_t sdio_get_block_size(void) {
    return SDIO_BLOCK_SIZE;
}

STATIC mp_obj_t sdio_readblocks(mp_obj_t self, mp_obj_t block_num, mp_obj_t buf) {
    mp_uint_t ret;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_WRITE);
    ret = SD_ReadBlocks(mp_obj_get_int(block_num), bufinfo.buf, bufinfo.len / SDIO_BLOCK_SIZE);
    return MP_OBJ_NEW_SMALL_INT(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(sdio_readblocks_obj, sdio_readblocks);

STATIC mp_obj_t sdio_writeblocks(mp_obj_t self, mp_obj_t block_num, mp_obj_t buf) {
    mp_uint_t ret;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_READ);
    ret = SD_WriteBlocks(mp_obj_get_int(block_num), bufinfo.buf, bufinfo.len / SDIO_BLOCK_SIZE);
    return MP_OBJ_NEW_SMALL_INT(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(sdio_writeblocks_obj, sdio_writeblocks);

STATIC mp_obj_t sdio_ioctl(mp_obj_t self, mp_obj_t cmd_in, mp_obj_t arg_in) {
    mp_int_t cmd = mp_obj_get_int(cmd_in);
    switch (cmd) {
        case BP_IOCTL_INIT: sdio_init(); return MP_OBJ_NEW_SMALL_INT(0);
        case BP_IOCTL_DEINIT: sdio_deinit(); return MP_OBJ_NEW_SMALL_INT(0);
        case BP_IOCTL_SYNC: sdio_flush(); return MP_OBJ_NEW_SMALL_INT(0);
        //case BP_IOCTL_SEC_COUNT: return MP_OBJ_NEW_SMALL_INT(storage_get_block_count());
        case BP_IOCTL_SEC_SIZE: return MP_OBJ_NEW_SMALL_INT(sdio_get_block_size());

        default: return mp_const_none;
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(sdio_ioctl_obj, sdio_ioctl);

STATIC mp_obj_t sdio_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    // check arguments
    mp_arg_check_num(n_args, n_kw, 0, 0, false);

    // return singleton object
    return (mp_obj_t)&sdio_obj;
}

STATIC const mp_map_elem_t sdio_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_readblocks),  MP_OBJ_FROM_PTR(&sdio_readblocks_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_writeblocks), MP_OBJ_FROM_PTR(&sdio_writeblocks_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_ioctl),       MP_OBJ_FROM_PTR(&sdio_ioctl_obj) },
};
STATIC MP_DEFINE_CONST_DICT(sdio_locals_dict, sdio_locals_dict_table);

const mp_obj_type_t sdio_type = {
    { &mp_type_type },
    .name        = MP_QSTR_SDIO,
    .make_new    = sdio_make_new,
    .locals_dict = (mp_obj_t)&sdio_locals_dict,
};
