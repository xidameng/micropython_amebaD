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

#include "obji2c.h"

i2c_t i2c_channel;

STATIC bool pyb_i2c_write(i2c_t *i2c, byte addr, byte *data, uint len, bool stop) {
    int8_t retval = true;

    retval = i2c_write(i2c, addr, data, len, stop);

     if (retval <= 0) {
        return false;
    }
ret:
    return true;
}

STATIC bool pyb_i2c_read(i2c_t *i2c, byte addr, byte **data, uint len, bool stop) {
    int8_t retval = true;

    retval = i2c_read(i2c, addr, *data, len, stop);
    
    if (retval != len) {
        return false;
    }
ret:
    return true;
}

STATIC mp_obj_t mp_i2c_reset(mp_obj_t self_in) {
    i2c_obj_t *self = (i2c_obj_t *)&self_in;
    i2c_reset(self->obj);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(i2c_reset_obj, mp_i2c_reset);

STATIC mp_obj_t i2c_recv(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    STATIC const mp_arg_t i2c_readfrom_args[] = {
        { MP_QSTR_nbytes,  MP_ARG_REQUIRED | MP_ARG_OBJ, },
        { MP_QSTR_addr,    MP_ARG_REQUIRED | MP_ARG_INT, },
        { MP_QSTR_stop,    MP_ARG_KW_ONLY  | MP_ARG_BOOL, {.u_bool = true} },
    };

    i2c_obj_t *self = pos_args[0];
    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(i2c_readfrom_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(args), i2c_readfrom_args, args);

    vstr_t vstr;

    // get the buffer to receive into
    pyb_buf_get_for_recv(args[0].u_obj, &vstr);

    // receive the data
    if (!pyb_i2c_read(self->obj, args[1].u_int, (byte *)&vstr.buf, vstr.len, args[2].u_bool)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, mpexception_os_operation_failed));
    }
    // return the received data
    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(i2c_recv_obj, 3, i2c_recv);

STATIC mp_obj_t i2c_send(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    STATIC const mp_arg_t pyb_i2c_writeto_args[] = {
        { MP_QSTR_buf,     MP_ARG_REQUIRED | MP_ARG_OBJ,  },
        { MP_QSTR_addr,    MP_ARG_REQUIRED | MP_ARG_INT,  },
        { MP_QSTR_stop,    MP_ARG_KW_ONLY  | MP_ARG_BOOL, {.u_bool = true} },
    };

    i2c_obj_t *self = pos_args[0];
    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(pyb_i2c_writeto_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(args), pyb_i2c_writeto_args, args);

    // get the buffer to send from
    mp_buffer_info_t bufinfo;
    uint8_t data[1];
    pyb_buf_get_for_send(args[0].u_obj, &bufinfo, data);

    // send the data
    if (!pyb_i2c_write(self->obj, args[1].u_int, bufinfo.buf, bufinfo.len, args[2].u_bool)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, mpexception_os_operation_failed));
    }

    // return the number of bytes written
    return mp_obj_new_int(bufinfo.len);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(i2c_send_obj, 1, i2c_send);

STATIC void i2c_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    i2c_obj_t *self = self_in;
    qstr mode;
    if (self->mode == I2C_MASTER) {
        mode = MP_QSTR_MASTER;
    }
    else {
        mode = MP_QSTR_SLAVE;
    }
    mp_printf(print, "I2C(%d, mode=%q, baudrate=%u, SCL=%q, SDA=%q)", self->unit, mode, self->baudrate, self->scl->name, self->sda->name);
}

STATIC mp_obj_t i2c_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {

    // TODO: alloc a new tulpe ???
    mp_obj_t tuple[2];
    tuple[0] = &pin_PD_5;
    tuple[1] = &pin_PD_4;

    const mp_arg_t i2c_init_args[] = {
        { MP_QSTR_unit,                       MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_mode,      MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = I2C_MASTER} },
        { MP_QSTR_baudrate,  MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = I2C_DEFAULT_BAUD_RATE_HZ} },
        { MP_QSTR_pins,      MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = mp_obj_new_tuple(2, tuple)} },
    };
    // parse args
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(i2c_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), i2c_init_args, args);

    mp_obj_t *pins;
    mp_obj_get_array_fixed_n(args[3].u_obj, 2, &pins);

    pin_obj_t *scl = pin_find(pins[0]);
    pin_obj_t *sda = pin_find(pins[1]);

    if (pin_obj_find_af(scl, PIN_FN_I2C, args[0].u_int, PIN_TYPE_I2C_SCL) < 0) 
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid SCL pin"));

    if (pin_obj_find_af(sda, PIN_FN_I2C, args[0].u_int, PIN_TYPE_I2C_SDA) < 0) 
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid SDA pin"));

    i2c_obj_t *self = m_new_obj(i2c_obj_t);
    self->base.type = &i2c_type;
    self->unit      = args[0].u_int;
    self->mode      = args[1].u_int;
    self->baudrate  = MIN(MAX(args[2].u_int, I2C_MIN_BAUD_RATE_HZ), I2C_MAX_BAUD_RATE_HZ);
    self->scl       = scl;
    self->sda       = sda;
    
    // Work around: alloc a new i2c_t memory to prevent from hang
    self->obj       = &i2c_channel;
    //self->obj       = malloc(sizeof(i2c_t));

    i2c_init(self->obj, self->sda->id, self->scl->id);

    i2c_frequency(self->obj, self->baudrate);

    return (mp_obj_t)self;
}

STATIC mp_obj_t i2c_mem_read(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    const mp_arg_t i2c_mem_read_allowed_args[] = {
        { MP_QSTR_data,      MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_addr,      MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_memaddr,   MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_addr_size, MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 8} },
    };
    i2c_obj_t *self = pos_args[0];
    mp_arg_val_t args[MP_ARRAY_SIZE(i2c_mem_read_allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(i2c_mem_read_allowed_args), i2c_mem_read_allowed_args, args);

    if ((args[3].u_int != 8) && (args[3].u_int != 16) )
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "addr_size are only 8 or 16 bits support"));

    vstr_t vstr;
    mp_obj_t o_ret = pyb_buf_get_for_recv(args[0].u_obj, &vstr);

    uint8_t   i2c_addr = args[1].u_int;
    uint16_t  mem_addr = args[2].u_int;
    uint16_t  addr_size = args[3].u_int;
    if (i2c_write(self->obj, i2c_addr, &mem_addr, addr_size/8, false) <= 0)
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "I2C memory write address failed"));

    uint16_t ret = i2c_read(self->obj, i2c_addr, vstr.buf, vstr.len, true);

    if (ret != vstr.len) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "I2C memory read data failed"));
    }
    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(i2c_mem_read_obj, 1, i2c_mem_read);

STATIC mp_obj_t i2c_mem_write(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    const mp_arg_t i2c_mem_read_allowed_args[] = {
        { MP_QSTR_data,      MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_addr,      MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_memaddr,   MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_addr_size, MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 8} },
    };
    i2c_obj_t *self = pos_args[0];
    mp_arg_val_t args[MP_ARRAY_SIZE(i2c_mem_read_allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(i2c_mem_read_allowed_args), i2c_mem_read_allowed_args, args);

    if ((args[3].u_int != 8) && (args[3].u_int != 16) )
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "addr_size are only 8 or 16 bits support"));

    mp_buffer_info_t bufinfo;
    uint8_t data;
    pyb_buf_get_for_send(args[0].u_obj, &bufinfo, &data);

    uint8_t i2c_addr = args[1].u_int;
    uint16_t mem_addr = args[2].u_int;
    uint16_t addr_size = args[3].u_int;

    int8_t *buffer = malloc(bufinfo.len + addr_size/8);
    memcpy(buffer, &mem_addr, addr_size/8);
    strcat(buffer, bufinfo.buf);
    
    if (i2c_write(self->obj, i2c_addr, buffer, bufinfo.len + addr_size/8, true) <= 0) {
        free(buffer);
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "addr_size are only 8 or 16 bits support"));
    }
    free(buffer);
    return mp_obj_new_int(bufinfo.len);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(i2c_mem_write_obj, 1, i2c_mem_write);

STATIC const mp_map_elem_t i2c_locals_dict_table[] = {
    // instance methods
#if 0 // TODO(Chester): Need to figure out how to scan ...
    { MP_OBJ_NEW_QSTR(MP_QSTR_scan),                (mp_obj_t)&i2c_scan_obj },
#endif
    { MP_OBJ_NEW_QSTR(MP_QSTR_reset),               (mp_obj_t)&i2c_reset_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_recv),                (mp_obj_t)&i2c_recv_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_send),                (mp_obj_t)&i2c_send_obj },

    { MP_OBJ_NEW_QSTR(MP_QSTR_mem_read),            (mp_obj_t)&i2c_mem_read_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_mem_write),           (mp_obj_t)&i2c_mem_write_obj },

    // class constants
    { MP_OBJ_NEW_QSTR(MP_QSTR_MASTER),              MP_OBJ_NEW_SMALL_INT(I2C_MASTER) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SLAVE),               MP_OBJ_NEW_SMALL_INT(I2C_SLAVE) },
};
STATIC MP_DEFINE_CONST_DICT(i2c_locals_dict, i2c_locals_dict_table);

const mp_obj_type_t i2c_type = {
    { &mp_type_type },
    .name        = MP_QSTR_I2C,
    .print       = i2c_print,
    .make_new    = i2c_make_new,
    .locals_dict = (mp_obj_t)&i2c_locals_dict,
};
