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
#include "objsdio_host.h"
#include "lib/oofatfs/ff.h"

STATIC sdio_host_obj_t sdio_host_obj = {
    .base.type = &sdio_host_type,
};

uint8_t sdio_host_init(void) {
    SD_RESULT ret = SD_ERROR;
    if ((ret = SD_Init()) != SD_OK)
        return -1;
    else
        return 0;
}

uint8_t sdio_host_deinit(void) {
    SD_RESULT ret = SD_ERROR;
    if ((ret = SD_DeInit()) != SD_OK)
        return -1;
    else
        return 0;
}

uint8_t sdio_host_flush(void) {
    // Doesn't have a proper API to flush
    return 0;
}

mp_uint_t sdio_host_get_block_size(void) {
    return SDIO_HOST_BLOCK_SIZE;
}

mp_uint_t sdio_host_get_block_count(void) {
    uint32_t sector;
    SD_GetCapacity(&sector);
    // TODO Debug only, will be removed later;
    DiagPrintf("get_block_count %d\r\n", sector);
    return sector;
}

STATIC mp_obj_t sdio_host_readblocks(mp_obj_t self, mp_obj_t block_num, mp_obj_t buf) {
    mp_uint_t ret;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_WRITE);
    if ((ret = SD_WaitReady()) != SD_OK) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_TimeoutError,
                "SDIO_HOST wait read ready timeout [%u]", ret));
    } else {
        if ((ret = SD_ReadBlocks(mp_obj_get_int(block_num), bufinfo.buf, 
                        (bufinfo.len / sdio_host_get_block_size()))) != SD_OK) {
            nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_OSError,
                "SDIO_HOST read block failed [%u]", ret));
        }
    }
    return MP_OBJ_NEW_SMALL_INT(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(sdio_host_readblocks_obj, sdio_host_readblocks);

STATIC mp_obj_t sdio_host_writeblocks(mp_obj_t self, mp_obj_t block_num, mp_obj_t buf) {
    mp_uint_t ret;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_READ);
    if ((ret = SD_WaitReady()) != SD_OK) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_TimeoutError,
                "SDIO_HOST wait write ready timeout [%u]", ret));
    } else {
        if ((ret = SD_WriteBlocks(mp_obj_get_int(block_num), bufinfo.buf,
                        (bufinfo.len / sdio_host_get_block_size()))) != SD_OK) {
            nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_OSError,
                "SDIO_HOST write block failed [%u]", ret));
        }
    }
    return MP_OBJ_NEW_SMALL_INT(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(sdio_host_writeblocks_obj, sdio_host_writeblocks);

STATIC mp_obj_t sdio_host_ioctl(mp_obj_t self, mp_obj_t cmd_in, mp_obj_t arg_in) {
    mp_int_t cmd = mp_obj_get_int(cmd_in);
#if 0
    switch (cmd) {
        case BP_IOCTL_INIT: return MP_OBJ_NEW_SMALL_INT(sdio_host_init());
        case BP_IOCTL_DEINIT: return MP_OBJ_NEW_SMALL_INT(sdio_host_deinit());
        case BP_IOCTL_SYNC: return MP_OBJ_NEW_SMALL_INT(sdio_host_flush());
        case BP_IOCTL_SEC_COUNT: return MP_OBJ_NEW_SMALL_INT(sdio_host_get_block_count());
        case BP_IOCTL_SEC_SIZE: return MP_OBJ_NEW_SMALL_INT(sdio_host_get_block_size());
        case SDIO_HOST_SET_CLOCK: return MP_OBJ_NEW_SMALL_INT(SD_SetCLK(mp_obj_get_int(arg_in)));

        default: return mp_const_none;
    }
#endif
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(sdio_host_ioctl_obj, sdio_host_ioctl);

STATIC mp_obj_t sdio_host_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    // check arguments
    mp_arg_check_num(n_args, n_kw, 0, 0, false);

    // return singleton object
    return (mp_obj_t)&sdio_host_obj;
}

STATIC const mp_map_elem_t sdio_host_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_readblocks),  MP_OBJ_FROM_PTR(&sdio_host_readblocks_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_writeblocks), MP_OBJ_FROM_PTR(&sdio_host_writeblocks_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_ioctl),       MP_OBJ_FROM_PTR(&sdio_host_ioctl_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_IOCTL_CLK),   MP_OBJ_NEW_SMALL_INT(SDIO_HOST_SET_CLOCK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_CLK_LOW),     MP_OBJ_NEW_SMALL_INT(SD_CLK_LOW) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_CLK_MID),     MP_OBJ_NEW_SMALL_INT(SD_CLK_MID) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_CLK_HIGH),    MP_OBJ_NEW_SMALL_INT(SD_CLK_HIGH) },
};
STATIC MP_DEFINE_CONST_DICT(sdio_host_locals_dict, sdio_host_locals_dict_table);

const mp_obj_type_t sdio_host_type = {
    { &mp_type_type },
    .name        = MP_QSTR_SDIO_HOST,
    .make_new    = sdio_host_make_new,
    .locals_dict = MP_OBJ_FROM_PTR(&sdio_host_locals_dict),
};
