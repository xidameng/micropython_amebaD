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

#include "objflash.h"

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/

/*****************************************************************************
 *                              Internal functions
 * ***************************************************************************/
STATIC flash_obj_t flash_obj = {
    .base.type = &flash_type,
};

STATIC void flash_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    flash_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "FLASH()");
}

STATIC mp_obj_t flash_read(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum {ARG_nbytes, ARG_addr};
    STATIC const mp_arg_t flash_read_args[] = {
        { MP_QSTR_nbytes, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_addr,   MP_ARG_REQUIRED | MP_ARG_INT, },
    };

    flash_obj_t *self = pos_args[0];

    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(flash_read_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(args),
        flash_read_args, args);

    vstr_t vstr;
    mp_obj_t o_ret = pyb_buf_get_for_recv(args[ARG_nbytes].u_obj, &vstr);
    
    uint8_t ret = 0;

    if ((ret = flash_burst_read(&(self->obj), args[ARG_addr].u_int, vstr.len, vstr.buf)) != 1) {
        mp_raise_msg(&mp_type_OSError, "FLASH read error");
    }

    if (o_ret != MP_OBJ_NULL) {
      return o_ret;
    } else {
      return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(flash_read_obj, 2, flash_read);

STATIC mp_obj_t flash_write(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_buf, ARG_addr, ARG_stop };
    STATIC const mp_arg_t flash_write_args[] = {
        { MP_QSTR_buf,  MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_addr, MP_ARG_REQUIRED | MP_ARG_INT,  },
    };

    flash_obj_t *self = pos_args[0];

    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(flash_write_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(args), 
        flash_write_args, args);

    // get the buffer to send from
    mp_buffer_info_t bufinfo;
    uint8_t data[1];
    pyb_buf_get_for_send(args[ARG_buf].u_obj, &bufinfo, data);

    if (flash_stream_write(&(self->obj), args[ARG_addr].u_int, bufinfo.len, bufinfo.buf) != 1) {
        mp_raise_msg(&mp_type_OSError, "FLASH write error");
    }
  
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(flash_write_obj, 2, flash_write);

STATIC mp_obj_t flash_erase_sector0(mp_obj_t self_in, mp_obj_t addr_in) {
    mp_int_t addr = mp_obj_get_int(addr_in);
    flash_obj_t *self = MP_OBJ_TO_PTR(self_in);
    flash_erase_sector(&(self->obj), addr);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(flash_erase_sector_obj, flash_erase_sector0);

STATIC const mp_map_elem_t flash_locals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_flash) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_read),  MP_OBJ_FROM_PTR(&flash_read_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_write), MP_OBJ_FROM_PTR(&flash_write_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_erase_sector), MP_OBJ_FROM_PTR(&flash_erase_sector_obj) },
};
STATIC MP_DEFINE_CONST_DICT(flash_locals_dict, flash_locals_table);

STATIC mp_obj_t flash_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    // check arguments
    mp_arg_check_num(n_args, n_kw, 0, 0, false);

    // return singleton object
    return (mp_obj_t)&flash_obj;
}

const mp_obj_type_t flash_type = {
    { &mp_type_type },
    .name        = MP_QSTR_FLASH,
    .print       = flash_print,
    .make_new    = flash_make_new,
    .locals_dict = (mp_obj_dict_t *)&flash_locals_dict,
};
