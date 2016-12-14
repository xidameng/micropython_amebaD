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
#include "objflash.h"

STATIC const flash_obj_t flash_obj = {
    .base.type = &flash_type,
};

STATIC uint32_t fatfs_start_address = 0;
STATIC uint32_t fatfs_end_address = 0;
STATIC uint32_t fatfs_num_blocks = 0;
STATIC uint32_t fatfs_block_size = 0x1000;

void flash_init(void) {
    // Do nothing here
    fatfs_start_address = &__fatfs_start_address__;
    fatfs_end_address = &__fatfs_end_address__;
    fatfs_num_blocks = &__fatfs_num_blocks__;
}

void flash_flush(void) {
    // Do nothing here
}

uint32_t flash_get_block_count(void) {
    return fatfs_num_blocks;
}

uint32_t flash_get_block_size(void) {
    return fatfs_block_size;
}

bool flash_read_block(uint8_t *dest, uint32_t block) {
    int16_t ret = -1;
    if (block > flash_get_block_count()) {
        block -= flash_get_block_count();
    }
    uint32_t flash_addr = (block * flash_get_block_size()) + fatfs_start_address;
    device_mutex_lock(RT_DEV_LOCK_FLASH);
    ret = flash_burst_read(&(flash_obj.obj), flash_addr, flash_get_block_size(), dest);
    device_mutex_unlock(RT_DEV_LOCK_FLASH);
    return ret == true? true:false;
}

bool flash_write_block(const uint8_t *src, uint32_t block) {
    int16_t ret = -1;
    if (block > flash_get_block_count()) {
        block -= flash_get_block_count();
    }
    uint32_t flash_addr = (block * flash_get_block_size()) + fatfs_start_address;
    device_mutex_lock(RT_DEV_LOCK_FLASH);
    // Erase should be before write
    flash_erase_sector(&(flash_obj.obj), flash_addr); 
    ret = flash_burst_write(&(flash_obj.obj), flash_addr, flash_get_block_size(), src);
    device_mutex_unlock(RT_DEV_LOCK_FLASH);
    
    return ret == true? true:false;
}

mp_uint_t flash_read_blocks(uint8_t *dest, uint32_t block_num, uint32_t num_blocks) {
    for (size_t i = 0; i < num_blocks; i++) {
        if (!flash_read_block(dest + i * flash_get_block_size(), block_num + i)) {
            DiagPrintf("block %d read error\r\n", block_num);
            return 1; // error
        }
    }
    return 0; // success
}

mp_uint_t flash_write_blocks(const uint8_t *src, uint32_t block_num, uint32_t num_blocks) {
    for (size_t i = 0; i < num_blocks; i++) {
        if (!flash_write_block(src + i * flash_get_block_size(), block_num + i)) {
            DiagPrintf("block %d write error\r\n", block_num);
            return 1; // error
        }
    }
    return 0; // success
}

STATIC mp_obj_t flash_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    // check arguments
    mp_arg_check_num(n_args, n_kw, 0, 0, false);

    // return singleton object
    return MP_OBJ_FROM_PTR(&flash_obj);
}

STATIC mp_obj_t flash_readblocks(mp_obj_t self, mp_obj_t block_num, mp_obj_t buf) {
    mp_uint_t ret;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_WRITE);
    ret = flash_read_blocks(bufinfo.buf, mp_obj_get_int(block_num), bufinfo.len / fatfs_block_size);
    return MP_OBJ_NEW_SMALL_INT(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(flash_readblocks_obj, flash_readblocks);

STATIC mp_obj_t flash_writeblocks(mp_obj_t self, mp_obj_t block_num, mp_obj_t buf) {
    mp_uint_t ret;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_READ);
    ret = flash_write_blocks(bufinfo.buf, mp_obj_get_int(block_num), bufinfo.len / fatfs_block_size);
    return MP_OBJ_NEW_SMALL_INT(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(flash_writeblocks_obj, flash_writeblocks);

STATIC mp_obj_t flash_ioctl(mp_obj_t self, mp_obj_t cmd_in, mp_obj_t arg_in) {
    mp_int_t cmd = mp_obj_get_int(cmd_in);
    switch (cmd) {
        case BP_IOCTL_INIT: flash_init(); return MP_OBJ_NEW_SMALL_INT(0);
        case BP_IOCTL_DEINIT: return MP_OBJ_NEW_SMALL_INT(0);
        case BP_IOCTL_SYNC: flash_flush(); return MP_OBJ_NEW_SMALL_INT(0);
        case BP_IOCTL_SEC_COUNT: return MP_OBJ_NEW_SMALL_INT(flash_get_block_count());
        case BP_IOCTL_SEC_SIZE: return MP_OBJ_NEW_SMALL_INT(flash_get_block_size());
        default: return mp_const_none;
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(flash_ioctl_obj, flash_ioctl);

STATIC const mp_map_elem_t flash_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_readblocks),  MP_OBJ_FROM_PTR(&flash_readblocks_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_writeblocks), MP_OBJ_FROM_PTR(&flash_writeblocks_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_ioctl),       MP_OBJ_FROM_PTR(&flash_ioctl_obj) },
};
STATIC MP_DEFINE_CONST_DICT(flash_locals_dict, flash_locals_dict_table);

const mp_obj_type_t flash_type = {
    { &mp_type_type },
    .name        = MP_QSTR_FLASH,
    .make_new    = flash_make_new,
    .locals_dict = (mp_obj_t)&flash_locals_dict,
};

void flash_init0 (fs_user_mount_t *vfs) {
    vfs->flags |= FSUSER_NATIVE | FSUSER_HAVE_IOCTL;
    vfs->readblocks[0]  = MP_OBJ_FROM_PTR(&flash_readblocks_obj);
    vfs->readblocks[1]  = MP_OBJ_FROM_PTR(&flash_obj);
    vfs->readblocks[2]  = MP_OBJ_FROM_PTR(flash_read_blocks); // native version
    vfs->writeblocks[0] = MP_OBJ_FROM_PTR(&flash_writeblocks_obj);
    vfs->writeblocks[1] = MP_OBJ_FROM_PTR(&flash_obj);
    vfs->writeblocks[2] = MP_OBJ_FROM_PTR(flash_write_blocks); // native version
    vfs->u.ioctl[0]     = MP_OBJ_FROM_PTR(&flash_ioctl_obj);
    vfs->u.ioctl[1]     = MP_OBJ_FROM_PTR(&flash_obj);
}
