#include "py/obj.h"
#include "py/runtime.h"
#include "lib/fatfs/ff.h"
#include "extmod/fsusermount.h"
#include "flash.h"

flash_t         flash_obj;

static bool flash_is_initialised = false;
static __IO uint8_t flash_flags = 0;
static uint32_t flash_cache_sector_id;
static uint32_t flash_cache_sector_start;
static uint32_t flash_cache_sector_size;
static uint32_t flash_tick_counter_last_write;

STATIC const mp_obj_base_t pyb_flash_obj = {&pyb_flash_type};

void storage_init(void) {
    if (!flash_is_initialised) {
        flash_flags = 0;
        flash_cache_sector_id = 0;
        flash_tick_counter_last_write = 0;
        flash_is_initialised = true;
        flash_write_protect(&flash_obj, false);
    }
}

void storage_flush(void) {
    int32_t data = 0;
    data = flash_get_status(&flash_obj);
    DiagPrintf("data is 0x%x\n", data);
}

uint32_t storage_get_block_count(void) {
    return FLASH_START_BLOCK + FLASH_NUM_BLOCKS;
}

uint32_t storage_get_block_size(void) {
    return FLASH_BLOCK_SIZE;
}

static uint32_t convert_block_to_flash_addr(uint32_t block) {
    if (FLASH_START_BLOCK <= block && block < FLASH_START_BLOCK + FLASH_NUM_BLOCKS) {
        block -= FLASH_START_BLOCK;
        return FLASH_START_BLOCK + block * FLASH_BLOCK_SIZE + FLASH_START_BASE;
    }
    // bad block
    return -1;
}

bool storage_read_block(uint8_t *dest, uint32_t block) {
    uint16_t i = 0;
    uint32_t value = 0;
    uint32_t flash_addr = convert_block_to_flash_addr(block);
    if (flash_addr == -1) {
        return false;
    }
    //TODO need error handle
    for (i = 0; i < FLASH_BLOCK_SIZE; i+= 4) {
        flash_read_word(&flash_obj, flash_addr + i, &value);
        dest[i+0] = (uint8_t)(value & 0xff);
        dest[i+1] = (uint8_t)((value & 0xff00) >> 8);
        dest[i+2] = (uint8_t)((value & 0xff0000) >> 16);
        dest[i+3] = (uint8_t)((value & 0xff000000) >> 24);
    }
    return true;
}

bool storage_write_block(const uint8_t *src, uint32_t block) {
    uint16_t i = 0;
    uint8_t temp[FLASH_BLOCK_SIZE] = {0};
    uint32_t value = 0;
    uint32_t flash_addr = convert_block_to_flash_addr(block);
    if (flash_addr == -1) {
        return false;
    }
    //TODO need error handle
    flash_erase_sector(&flash_obj, flash_addr); 
    for (i = 0;i < FLASH_BLOCK_SIZE; i+= 4) {
        value = (src[i+3]<<24) + (src[i+2]<<16) + (src[i+1]<<8) + src[i];
        flash_write_word(&flash_obj, flash_addr + i, value); 
    }
    return true;
}

mp_uint_t storage_read_blocks(uint8_t *dest, uint32_t block_num, uint32_t num_blocks) {
    for (size_t i = 0; i < num_blocks; i++) {
        if (!storage_read_block(dest + i * FLASH_BLOCK_SIZE, block_num + i)) {
            return 1; // error
        }
    }
    return 0; // success
}

mp_uint_t storage_write_blocks(const uint8_t *src, uint32_t block_num, uint32_t num_blocks) {
    for (size_t i = 0; i < num_blocks; i++) {
        if (!storage_write_block(src + i * FLASH_BLOCK_SIZE, block_num + i)) {
            return 1; // error
        }
    }
    return 0; // success
}

STATIC mp_obj_t pyb_flash_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    // check arguments
    mp_arg_check_num(n_args, n_kw, 0, 0, false);

    // return singleton object
    return (mp_obj_t)&pyb_flash_obj;
}

STATIC mp_obj_t pyb_flash_readblocks(mp_obj_t self, mp_obj_t block_num, mp_obj_t buf) {
    mp_uint_t ret;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_WRITE);
    ret = storage_read_blocks(bufinfo.buf, mp_obj_get_int(block_num), bufinfo.len / FLASH_BLOCK_SIZE);
    return MP_OBJ_NEW_SMALL_INT(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(pyb_flash_readblocks_obj, pyb_flash_readblocks);

STATIC mp_obj_t pyb_flash_writeblocks(mp_obj_t self, mp_obj_t block_num, mp_obj_t buf) {
    mp_uint_t ret;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_READ);
    ret = storage_write_blocks(bufinfo.buf, mp_obj_get_int(block_num), bufinfo.len / FLASH_BLOCK_SIZE);
    return MP_OBJ_NEW_SMALL_INT(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(pyb_flash_writeblocks_obj, pyb_flash_writeblocks);

STATIC mp_obj_t pyb_flash_ioctl(mp_obj_t self, mp_obj_t cmd_in, mp_obj_t arg_in) {
    mp_int_t cmd = mp_obj_get_int(cmd_in);
    switch (cmd) {
        case BP_IOCTL_INIT: storage_init(); return MP_OBJ_NEW_SMALL_INT(0);
        case BP_IOCTL_DEINIT: storage_flush(); return MP_OBJ_NEW_SMALL_INT(0); // TODO properly
        case BP_IOCTL_SYNC: storage_flush(); return MP_OBJ_NEW_SMALL_INT(0);
        case BP_IOCTL_SEC_COUNT: return MP_OBJ_NEW_SMALL_INT(storage_get_block_count());
        case BP_IOCTL_SEC_SIZE: return MP_OBJ_NEW_SMALL_INT(storage_get_block_size());

        default: return mp_const_none;
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(pyb_flash_ioctl_obj, pyb_flash_ioctl);


STATIC const mp_map_elem_t pyb_flash_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_readblocks), (mp_obj_t)&pyb_flash_readblocks_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_writeblocks), (mp_obj_t)&pyb_flash_writeblocks_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_ioctl), (mp_obj_t)&pyb_flash_ioctl_obj },
};

STATIC MP_DEFINE_CONST_DICT(pyb_flash_locals_dict, pyb_flash_locals_dict_table);

const mp_obj_type_t pyb_flash_type = {
    { &mp_type_type },
    .name = MP_QSTR_Flash,
    .make_new = pyb_flash_make_new,
    .locals_dict = (mp_obj_t)&pyb_flash_locals_dict,
};

void flash_init0 (fs_user_mount_t *vfs) {
    vfs->flags |= FSUSER_NATIVE | FSUSER_HAVE_IOCTL;
    vfs->readblocks[0]  = (mp_obj_t)&pyb_flash_readblocks_obj;
    vfs->readblocks[1]  = (mp_obj_t)&pyb_flash_obj;
    vfs->readblocks[2]  = (mp_obj_t)storage_read_blocks; // native version
    vfs->writeblocks[0] = (mp_obj_t)&pyb_flash_writeblocks_obj;
    vfs->writeblocks[1] = (mp_obj_t)&pyb_flash_obj;
    vfs->writeblocks[2] = (mp_obj_t)storage_write_blocks; // native version
    vfs->u.ioctl[0]     = (mp_obj_t)&pyb_flash_ioctl_obj;
    vfs->u.ioctl[1]     = (mp_obj_t)&pyb_flash_obj;
}
