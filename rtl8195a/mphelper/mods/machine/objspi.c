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

#include "objspi.h"

spi_t spi_obj[3];

STATIC mp_obj_t spi_transcation(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    STATIC const mp_arg_t spi_send_args[] = {
        { MP_QSTR_buf,     MP_ARG_REQUIRED | MP_ARG_OBJ,  },
    };

    spi_obj_t *self = pos_args[0];
    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(spi_send_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(args), spi_send_args, args);

    // get the buffer to send from
    mp_buffer_info_t bufinfo;
    uint8_t data[1];
    pyb_buf_get_for_send(args[0].u_obj, &bufinfo, data);

    // get the buffer to recv into
    vstr_t vstr;
    pyb_buf_get_for_recv(mp_obj_new_int(bufinfo.len), &vstr);

    // send the data
    char *ptr = 0;
    ptr = bufinfo.buf;
    spi_master_write_read_stream(self->obj, ptr, &vstr.buf[0], bufinfo.len);
    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(spi_transcation_obj, 1, spi_transcation);

STATIC mp_obj_t spi_send0(mp_obj_t self_in, mp_obj_t buf_in) {
    spi_obj_t *self = self_in;
    mp_buffer_info_t bufinfo;
    uint8_t data;
    char *ptr = NULL;
    ptr = bufinfo.buf;
    pyb_buf_get_for_send(buf_in, &bufinfo, &data);
    spi_master_write_stream(self->obj, ptr, bufinfo.len);
    return mp_obj_new_int(bufinfo.len);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(spi_send_obj, spi_send0);

STATIC mp_obj_t spi_recv0(mp_obj_t self_in, mp_obj_t size_in) {
    spi_obj_t *self = self_in;
    vstr_t vstr;
    pyb_buf_get_for_recv(size_in, &vstr);
    spi_master_read_stream(self->obj, &vstr.buf[0], vstr.len);
    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(spi_recv_obj, spi_recv0);

STATIC mp_obj_t spi_free0(mp_obj_t self_in) {
    spi_obj_t *self = self_in;
    spi_free(self->obj);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(spi_free_obj, spi_free0);

STATIC void spi_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    spi_obj_t *self = self_in;
    qstr mode_qstr;
    uint mode = self->spi_mode;
    if (mode == SPI_SLAVE) {
        mode_qstr = MP_QSTR_SLAVE;
    } else {
        mode_qstr = MP_QSTR_MASTER;
    }
    
    mp_printf(print, "SPI(%d, baudrate=%u, mode=%q, bits=%d, CLK=%q, MISO=%q, MOSI=%q, CS=%q)", self->unit, self->baudrate, mode_qstr, self->bits, self->clk->name, self->miso->name, self->mosi->name, self->cs->name);
}

STATIC mp_obj_t spi_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {

    mp_obj_t tuple[4];
    tuple[0] = &pin_PC_1;
    tuple[1] = &pin_PC_3;
    tuple[2] = &pin_PC_2;
    tuple[3] = &pin_PC_0;

    const mp_arg_t spi_init_args[] = {
        { MP_QSTR_unit,                       MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_baudrate,  MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = SPI_DEFAULT_BAUD_RATE} },
        { MP_QSTR_mode,      MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = SPI_MASTER} },
        { MP_QSTR_bits,      MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 8} },
        { MP_QSTR_ctrl,      MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_pins,      MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = mp_obj_new_tuple(4, tuple)} },
    };
    // parse args
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(spi_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), spi_init_args, args);

    mp_obj_t *pins;

    mp_obj_get_array_fixed_n(args[5].u_obj, 4, &pins);  

    pin_obj_t *clk  = pin_find(pins[0]);
    pin_obj_t *miso = pin_find(pins[1]);
    pin_obj_t *mosi = pin_find(pins[2]);
    pin_obj_t *cs   = pin_find(pins[3]);

    if (pin_obj_find_af(clk, PIN_FN_SPI, args[0].u_int, PIN_TYPE_SPI_CLK) < 0) 
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid CLK pin"));

    if (pin_obj_find_af(miso, PIN_FN_SPI, args[0].u_int, PIN_TYPE_SPI_MISO) < 0) 
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid MISO pin"));

    if (pin_obj_find_af(mosi, PIN_FN_SPI, args[0].u_int, PIN_TYPE_SPI_MOSI) < 0) 
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid MISO pin"));

    // TODO chip select with GPIO
    if (pin_obj_find_af(cs, PIN_FN_SPI, args[0].u_int, PIN_TYPE_SPI_CS0) < 0) 
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid CS pin"));

    spi_obj_t *self  = m_new_obj(spi_obj_t);
    self->base.type  = &spi_type;
    self->unit       = args[0].u_int;
    self->obj        = &spi_obj[self->unit];
    self->baudrate   = MIN(MAX(args[1].u_int, SPI_MIN_BAUD_RATE), SPI_MAX_BAUD_RATE);
    self->spi_mode   = args[2].u_int;
    self->bits       = args[3].u_int;
    self->trans_mode = args[4].u_int;

    self->clk = clk;
    self->miso = miso;
    self->mosi = mosi;
    self->cs = cs;

    spi_init(self->obj, self->mosi->id, self->miso->id, self->mosi->id, self->cs->id);

    spi_format(self->obj, self->bits, self->trans_mode, self->spi_mode);

    spi_frequency(self->obj, self->baudrate);

    return (mp_obj_t)self;
}


STATIC const mp_map_elem_t spi_locals_dict_table[] = {
    // instance methods
    { MP_OBJ_NEW_QSTR(MP_QSTR___del__),                 (mp_obj_t)&spi_free_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_send_recv),               (mp_obj_t)&spi_transcation_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_send),                    (mp_obj_t)&spi_send_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_recv),                    (mp_obj_t)&spi_recv_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_format),                  (mp_obj_t)&spi_recv_obj },

    // class constants
    { MP_OBJ_NEW_QSTR(MP_QSTR_MASTER),                  MP_OBJ_NEW_SMALL_INT(SPI_MASTER) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SLAVE),                   MP_OBJ_NEW_SMALL_INT(SPI_SLAVE) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_MSB),                     MP_OBJ_NEW_SMALL_INT(SPI_MSB) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_LSB),                     MP_OBJ_NEW_SMALL_INT(SPI_LSB) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_SCLK_IDLE_LOW),           MP_OBJ_NEW_SMALL_INT(SPI_SCLK_IDLE_LOW) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SCLK_IDLE_HIGH),          MP_OBJ_NEW_SMALL_INT(SPI_SCLK_IDLE_HIGH) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_CS_TOGGLE_EVERY_FRAME),   MP_OBJ_NEW_SMALL_INT(SPI_CS_TOGGLE_EVERY_FRAME) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_CS_TOGGLE_START_STOP),    MP_OBJ_NEW_SMALL_INT(SPI_CS_TOGGLE_START_STOP) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_SCLK_TOGGLE_MIDDLE),      MP_OBJ_NEW_SMALL_INT(SPI_SCLK_TOGGLE_MIDDLE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SCLK_TOGGLE_START),       MP_OBJ_NEW_SMALL_INT(SPI_SCLK_TOGGLE_START) },
};
STATIC MP_DEFINE_CONST_DICT(spi_locals_dict, spi_locals_dict_table);

const mp_obj_type_t spi_type = {
    { &mp_type_type },
    .name        = MP_QSTR_SPI,
    .print       = spi_print,
    .make_new    = spi_make_new,
    .locals_dict = (mp_obj_t)&spi_locals_dict,
};
