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

#include "obji2c.h"

STATIC i2c_obj_t i2c_obj[4] = {
    {.base.type = &i2c_type, .unit = 0, .mode = I2C_MASTER, .baudrate = I2C_DEFAULT_BAUD_RATE_HZ },
    {.base.type = &i2c_type, .unit = 1, .mode = I2C_MASTER, .baudrate = I2C_DEFAULT_BAUD_RATE_HZ },
    {.base.type = &i2c_type, .unit = 2, .mode = I2C_MASTER, .baudrate = I2C_DEFAULT_BAUD_RATE_HZ },
    {.base.type = &i2c_type, .unit = 3, .mode = I2C_MASTER, .baudrate = I2C_DEFAULT_BAUD_RATE_HZ },
};

STATIC mp_obj_t i2c_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {
    enum {ARG_unit, ARG_mode, ARG_baudrate, ARG_sda, ARG_scl };
    const mp_arg_t i2c_init_args[] = {
        { MP_QSTR_unit,                      MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_mode,     MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = I2C_MASTER} },
        { MP_QSTR_baudrate, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = I2C_DEFAULT_BAUD_RATE_HZ} },
        { MP_QSTR_sda,      MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_scl,      MP_ARG_REQUIRED | MP_ARG_OBJ },
    };

    // parse args
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(i2c_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), i2c_init_args, args);

    pin_obj_t *sda = pin_find(args[ARG_sda].u_obj);
    pin_obj_t *scl = pin_find(args[ARG_scl].u_obj);

    // Use Realtek's api to validate pins
    PinName pn_sda = (PinName)pinmap_peripheral(sda->id, PinMap_I2C_SDA);
    PinName pn_scl = (PinName)pinmap_peripheral(scl->id, PinMap_I2C_SCL);

    if (pn_sda == NC)
        mp_raise_ValueError("I2C SDA pin not match");

    if (pn_scl == NC)
        mp_raise_ValueError("I2C SCL pin not match");
    
    i2c_obj_t *self = &i2c_obj[args[ARG_unit].u_int];
    self->mode      = args[ARG_mode].u_int;
    self->baudrate  = MIN(MAX(args[ARG_baudrate].u_int, I2C_MIN_BAUD_RATE_HZ), I2C_MAX_BAUD_RATE_HZ);
    self->scl       = scl;
    self->sda       = sda;
    
    i2c_init(&(self->obj), self->sda->id, self->scl->id);

    i2c_frequency(&(self->obj), self->baudrate);

    return (mp_obj_t)self;
}

STATIC void i2c_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    i2c_obj_t *self = self_in;
    qstr mode = MP_QSTR_MASTER;
    mp_printf(print, "I2C(%d, mode=%q, baudrate=%u, SCL=%q, SDA=%q)", self->unit, mode, self->baudrate, self->scl->name, self->sda->name);
}

STATIC mp_obj_t mp_i2c_reset(mp_obj_t self_in) {
    i2c_obj_t *self = self_in;
    i2c_reset(&(self->obj));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(i2c_reset_obj, mp_i2c_reset);

STATIC mp_obj_t i2c_recv(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum {ARG_nbytes, ARG_addr, ARG_stop };
    STATIC const mp_arg_t i2c_recv_args[] = {
        { MP_QSTR_nbytes,  MP_ARG_REQUIRED | MP_ARG_OBJ, },
        { MP_QSTR_addr,    MP_ARG_REQUIRED | MP_ARG_INT, },
        { MP_QSTR_stop,    MP_ARG_KW_ONLY  | MP_ARG_BOOL, {.u_bool = true} },
    };

    i2c_obj_t *self = pos_args[0];

    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(i2c_recv_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(args), i2c_recv_args, args);

    vstr_t vstr;

    if (MP_OBJ_IS_INT(args[ARG_nbytes].u_obj)) {
        vstr_init_len(&vstr, mp_obj_get_int(args[ARG_nbytes].u_obj));
    } else {
        mp_buffer_info_t bufinfo;
        mp_get_buffer_raise(args[ARG_nbytes].u_obj, &bufinfo, MP_BUFFER_WRITE);
        vstr.buf = bufinfo.buf;
        vstr.len = bufinfo.len;
    }
    
    uint8_t ret = 0;

    if ((ret = i2c_read(&(self->obj), args[ARG_addr].u_int, vstr.buf, vstr.len,
                    args[ARG_stop].u_bool)) != vstr.len) {
        mp_raise_msg(&mp_type_OSError, "I2C read error");
    }

    // return the received data
    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(i2c_recv_obj, 3, i2c_recv);

STATIC mp_obj_t i2c_send(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_buf, ARG_addr, ARG_stop };
    STATIC const mp_arg_t i2c_send_args[] = {
        { MP_QSTR_buf,  MP_ARG_REQUIRED | MP_ARG_OBJ,  },
        { MP_QSTR_addr, MP_ARG_REQUIRED | MP_ARG_INT,  },
        { MP_QSTR_stop, MP_ARG_KW_ONLY  | MP_ARG_BOOL, {.u_bool = true} },
    };

    i2c_obj_t *self = pos_args[0];

    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(i2c_send_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(args), i2c_send_args, args);

    // get the buffer to send from
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[ARG_buf].u_obj, &bufinfo, MP_BUFFER_READ);

    if (i2c_write(&(self->obj), args[ARG_addr].u_int, bufinfo.buf, bufinfo.len, args[ARG_stop].u_bool) <= 0) {
        mp_raise_msg(&mp_type_OSError, "I2C send error");
    }

    // return the number of bytes written
    return mp_obj_new_int(bufinfo.len);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(i2c_send_obj, 1, i2c_send);

STATIC mp_obj_t i2c_mem_read(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum {ARG_data, ARG_addr, ARG_memaddr, ARG_addr_size };
    const mp_arg_t i2c_mem_read_allowed_args[] = {
        { MP_QSTR_data,      MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_addr,      MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_memaddr,   MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_addr_size, MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 8} },
    };

    i2c_obj_t *self = pos_args[0];

    mp_arg_val_t args[MP_ARRAY_SIZE(i2c_mem_read_allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(i2c_mem_read_allowed_args), i2c_mem_read_allowed_args, args);

    if ((args[ARG_addr_size].u_int != 8) && (args[ARG_addr_size].u_int != 16) )
        mp_raise_ValueError("addr_size are only support 8 / 16 bits");

    uint8_t *buffer = NULL;
    mp_uint_t len = 0;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[ARG_data].u_obj, &bufinfo, MP_BUFFER_WRITE);
    buffer = bufinfo.buf;
    len = bufinfo.len;

    uint8_t addr = args[ARG_addr].u_int;
    uint16_t mem_addr = args[ARG_memaddr].u_int;
    uint8_t addr_size = args[ARG_addr_size].u_int;

    if (i2c_write(&(self->obj), addr, &mem_addr, (addr_size / 8), false) <= 0)
        mp_raise_msg(&mp_type_OSError, "I2C memory write address error");

    uint8_t ret = i2c_read(&(self->obj), addr, buffer, len, true);

    if (ret != len) {
        mp_raise_msg(&mp_type_OSError, "I2C memory read data error");
    }

    return mp_obj_new_bytes(buffer, len);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(i2c_mem_read_obj, 1, i2c_mem_read);

STATIC mp_obj_t i2c_mem_write(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_data, ARG_addr, ARG_memaddr, ARG_addr_size };
    const mp_arg_t i2c_mem_read_allowed_args[] = {
        { MP_QSTR_data,      MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_addr,      MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_memaddr,   MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_addr_size, MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 8} },
    };

    i2c_obj_t *self = pos_args[0];

    mp_arg_val_t args[MP_ARRAY_SIZE(i2c_mem_read_allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args,
            MP_ARRAY_SIZE(i2c_mem_read_allowed_args), i2c_mem_read_allowed_args, args);

    if ((args[ARG_addr_size].u_int != 8) && (args[ARG_addr_size].u_int != 16) )
        mp_raise_ValueError("addr_size are obly support 8 and 16 bits");

    vstr_t vstr;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[ARG_data].u_obj, &bufinfo, MP_BUFFER_READ);
    vstr.buf = bufinfo.buf;
    vstr.len = bufinfo.len;

    uint8_t addr = args[ARG_addr].u_int;
    uint16_t memaddr = args[ARG_memaddr].u_int;
    uint8_t addr_size = args[ARG_addr_size].u_int;

    vstr_ins_byte(&vstr, 0, (memaddr & 0xFF));

    if (addr_size == 16)
        vstr_ins_byte(&vstr, 0, (uint8_t)((memaddr & 0xFF00) >> 8));

    if (i2c_write(&(self->obj), (uint8_t)addr, vstr.buf, vstr.len, true) <= 0) {
        mp_raise_msg(&mp_type_OSError, "I2C memory write error");
    }
    return mp_obj_new_int(bufinfo.len);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(i2c_mem_write_obj, 1, i2c_mem_write);

STATIC const mp_map_elem_t i2c_locals_dict_table[] = {
    // instance methods
#if 0 // TODO(Chester): Need to figure out how to scan ...
    { MP_OBJ_NEW_QSTR(MP_QSTR_scan),      MP_OBJ_FROM_PTR(&i2c_scan_obj) },
#endif
    { MP_OBJ_NEW_QSTR(MP_QSTR_reset),     MP_OBJ_FROM_PTR(&i2c_reset_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_recv),      MP_OBJ_FROM_PTR(&i2c_recv_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_send),      MP_OBJ_FROM_PTR(&i2c_send_obj) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_mem_read),  MP_OBJ_FROM_PTR(&i2c_mem_read_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_mem_write), MP_OBJ_FROM_PTR(&i2c_mem_write_obj) },

    // class constants
    { MP_OBJ_NEW_QSTR(MP_QSTR_MASTER),    MP_OBJ_NEW_SMALL_INT(I2C_MASTER) },

#if 0 // TODO (slave not support yet)
    { MP_OBJ_NEW_QSTR(MP_QSTR_SLAVE),     MP_OBJ_NEW_SMALL_INT(I2C_SLAVE) },
#endif
};
STATIC MP_DEFINE_CONST_DICT(i2c_locals_dict, i2c_locals_dict_table);

const mp_obj_type_t i2c_type = {
    { &mp_type_type },
    .name        = MP_QSTR_I2C,
    .print       = i2c_print,
    .make_new    = i2c_make_new,
    .locals_dict = (mp_obj_dict_t *)&i2c_locals_dict,
};
