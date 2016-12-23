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

#include "objspi.h"

STATIC spi_obj_t spi_obj[3] = {
    {.base.type = &spi_type, .unit = 0, .bits = 8, .mode = SPI_MASTER, .baudrate = SPI_DEFAULT_BAUD_RATE, .pol = SCPOL_INACTIVE_IS_LOW, .pha = SCPH_TOGGLES_IN_MIDDLE },
    {.base.type = &spi_type, .unit = 1, .bits = 8, .mode = SPI_MASTER, .baudrate = SPI_DEFAULT_BAUD_RATE, .pol = SCPOL_INACTIVE_IS_LOW, .pha = SCPH_TOGGLES_IN_MIDDLE },
    {.base.type = &spi_type, .unit = 2, .bits = 8, .mode = SPI_MASTER, .baudrate = SPI_DEFAULT_BAUD_RATE, .pol = SCPOL_INACTIVE_IS_LOW, .pha = SCPH_TOGGLES_IN_MIDDLE },
};

STATIC mp_obj_t spi_send0(mp_obj_t self_in, mp_obj_t buf_in) {
    spi_obj_t *self = self_in;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf_in, &bufinfo, MP_BUFFER_READ);
    spi_master_write_stream(&(self->obj), bufinfo.buf, bufinfo.len);
    return mp_obj_new_int(bufinfo.len);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(spi_send_obj, spi_send0);

STATIC mp_obj_t spi_recv0(mp_obj_t self_in, mp_obj_t size_in) {
    spi_obj_t *self = self_in;
    vstr_t vstr;

    if (MP_OBJ_IS_INT(size_in)) {
        vstr_init_len(&vstr, mp_obj_get_int(size_in));
    } else {
        mp_buffer_info_t bufinfo;
        mp_get_buffer_raise(size_in, &bufinfo, MP_BUFFER_WRITE);
        vstr.buf = bufinfo.buf;
        vstr.len = bufinfo.len;
    }

    spi_master_read_stream(&(self->obj), vstr.buf, vstr.len);

    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(spi_recv_obj, spi_recv0);

STATIC mp_obj_t spi_transcation(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum {ARG_buf};
    STATIC const mp_arg_t spi_trans_args[] = {
        { MP_QSTR_buf,     MP_ARG_REQUIRED | MP_ARG_OBJ,  },
    };

    spi_obj_t *self = pos_args[0];
    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(spi_trans_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(args),
            spi_trans_args, args);

    // get the buffer to send from
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[ARG_buf].u_obj, &bufinfo, MP_BUFFER_READ);

    // get the buffer to recv into
    vstr_t vstr;
    vstr_init_len(&vstr, bufinfo.len);

    // start transcation
    spi_master_write_read_stream(&(self->obj), bufinfo.buf, vstr.buf, bufinfo.len);
    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(spi_transcation_obj, 1, spi_transcation);

STATIC mp_obj_t spi_deinit(mp_obj_t self_in) {
    spi_obj_t *self = self_in;
    spi_free(&(self->obj));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(spi_deinit_obj, spi_deinit);

STATIC mp_obj_t spi_init0(mp_obj_t self_in) {

    spi_obj_t *self = self_in;

    spi_init(&(self->obj), self->mosi->id, self->miso->id, self->clk->id, self->cs->id);

    spi_format(&(self->obj), self->bits, ((self->pol << 1) + self->pha), self->mode);

    spi_frequency(&(self->obj), self->baudrate);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(spi_init_obj, spi_init0);

STATIC void spi_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    spi_obj_t *self = self_in;
    qstr mode_qstr;
    uint mode = self->mode;
    if (mode == SPI_SLAVE) {
        mode_qstr = MP_QSTR_SLAVE;
    } else {
        mode_qstr = MP_QSTR_MASTER;
    }
    
    mp_printf(print, "SPI(%d, baudrate=%u, mode=%q, bits=%d, MISO=%q, MOSI=%q, CLK=%q, CS=%q)", self->unit, self->baudrate, mode_qstr, self->bits, self->miso->name, self->mosi->name, self->clk->name, self->cs->name);
}

STATIC mp_obj_t spi_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw,
        const mp_obj_t *all_args) {
    enum { ARG_unit, ARG_baudrate, ARG_mode, ARG_bits, ARG_pol, ARG_pha,
        ARG_miso, ARG_mosi, ARG_clk, ARG_cs };
    const mp_arg_t spi_init_args[] = {
        { MP_QSTR_unit,                      MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_baudrate, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = SPI_DEFAULT_BAUD_RATE} },
        { MP_QSTR_mode,     MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = SPI_MASTER} },
        { MP_QSTR_bits,     MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 8} },
        { MP_QSTR_pol,      MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = SCPOL_INACTIVE_IS_LOW} },
        { MP_QSTR_pha,      MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = SCPH_TOGGLES_IN_MIDDLE} },
        { MP_QSTR_miso,     MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_mosi,     MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_clk,      MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_cs,       MP_ARG_REQUIRED | MP_ARG_OBJ },
    };
    // parse args
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(spi_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), spi_init_args, args);

    pin_obj_t *miso = pin_find(args[ARG_miso].u_obj);
    pin_obj_t *mosi = pin_find(args[ARG_mosi].u_obj);
    pin_obj_t *clk  = pin_find(args[ARG_clk].u_obj);
    pin_obj_t *cs   = pin_find(args[ARG_cs].u_obj);

    PinName pn_miso = (PinName)pinmap_peripheral(miso->id, PinMap_SPI_MISO);
    PinName pn_mosi = (PinName)pinmap_peripheral(mosi->id, PinMap_SPI_MOSI);

    if (pn_miso == NC) 
        mp_raise_ValueError("SPI MISO pin not match");

    if (pn_mosi == NC) 
        mp_raise_ValueError("SPI MOSI pin not match");

    spi_obj_t *self  = &spi_obj[args[ARG_unit].u_int];
    self->baudrate = MIN(MAX(args[ARG_baudrate].u_int, SPI_MIN_BAUD_RATE), SPI_MAX_BAUD_RATE);
    self->mode     = args[ARG_mode].u_int;
    self->bits     = args[ARG_bits].u_int;
    self->pol      = args[ARG_pol].u_int;
    self->pha      = args[ARG_pha].u_int;
    self->miso     = miso;
    self->mosi     = mosi;
    self->clk      = clk;
    self->cs       = cs;

    return (mp_obj_t)self;
}

STATIC const mp_map_elem_t spi_locals_dict_table[] = {
    // instance methods
    { MP_OBJ_NEW_QSTR(MP_QSTR_init),      MP_OBJ_FROM_PTR(&spi_init_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_deinit),    MP_OBJ_FROM_PTR(&spi_deinit_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_send),      MP_OBJ_FROM_PTR(&spi_send_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_recv),      MP_OBJ_FROM_PTR(&spi_recv_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_send_recv), MP_OBJ_FROM_PTR(&spi_transcation_obj) },

    // class constants
    { MP_OBJ_NEW_QSTR(MP_QSTR_MASTER),    MP_OBJ_NEW_SMALL_INT(SPI_MASTER) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SLAVE),     MP_OBJ_NEW_SMALL_INT(SPI_SLAVE) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_MSB),       MP_OBJ_NEW_SMALL_INT(SPI_MSB) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_LSB),       MP_OBJ_NEW_SMALL_INT(SPI_LSB) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_POL_INACTIVE_IS_LOW),    MP_OBJ_NEW_SMALL_INT(SCPOL_INACTIVE_IS_LOW) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_POL_INACTIVE_IS_HIGH),   MP_OBJ_NEW_SMALL_INT(SCPOL_INACTIVE_IS_HIGH) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PHA_TOGGLES_IN_MIDDLE),  MP_OBJ_NEW_SMALL_INT(SCPH_TOGGLES_IN_MIDDLE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PHA_TOGGLES_AT_START),   MP_OBJ_NEW_SMALL_INT(SCPH_TOGGLES_AT_START) },
};
STATIC MP_DEFINE_CONST_DICT(spi_locals_dict, spi_locals_dict_table);

const mp_obj_type_t spi_type = {
    { &mp_type_type },
    .name        = MP_QSTR_SPI,
    .print       = spi_print,
    .make_new    = spi_make_new,
    .locals_dict = (mp_obj_t)&spi_locals_dict,
};
