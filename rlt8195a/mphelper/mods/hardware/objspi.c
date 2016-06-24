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
#include "py/mpstate.h"
#include "py/runtime.h"
#include "py/mphal.h"

#include "exception.h"

#include "bufhelper.h"
#include "objspi.h"

#define SPI0_MOSI  PC_2
#define SPI0_MISO  PC_3
#define SPI0_SCLK  PC_1
#define SPI0_CS    PC_0

#define SPI1_MOSI  PB_6
#define SPI1_MISO  PB_7
#define SPI1_SCLK  PB_5
#define SPI1_CS    PB_4

#define SPI2_MOSI  PA_1
#define SPI2_MISO  PA_0
#define SPI2_SCLK  PA_2
#define SPI2_CS    PA_4


spi_t spi_channel0;     
spi_t spi_channel1;     
spi_t spi_channel2;     

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
    size_t i = 0;
    char *ptr = 0;
    ptr = bufinfo.buf;
    for (i = 0; i < bufinfo.len; i++) {
        vstr.buf[i] = spi_master_write((spi_t *)self->obj, ptr[i]);
    }

    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(spi_transcation_obj, 1, spi_transcation);

STATIC void spi_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    spi_obj_t *self = self_in;
    qstr mode_qstr;
    uint mode = self->spi_mode;
    if (mode == SPI_SLAVE) {
        mode_qstr = MP_QSTR_SLAVE;
    } else {
        mode_qstr = MP_QSTR_MASTER;
    }
    
    mp_printf(print, "SPI(%d, baudrate=%u, mode=%q, bits=%d)", self->id, self->baudrate, mode_qstr, self->bits);
}

STATIC mp_obj_t spi_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {
    const mp_arg_t spi_init_args[] = {
        { MP_QSTR_id,                          MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_baudrate,  MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = SPI_DEFAULT_BAUD_RATE} },
        { MP_QSTR_mode,      MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = SPI_MASTER} },
        { MP_QSTR_bits,      MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 8} },
        { MP_QSTR_ctrl,      MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 0} },
    };
    // parse args
    mp_map_t kw_args;
    mp_uint_t PinMOSI;
    mp_uint_t PinMISO;
    mp_uint_t PinSCLK;
    mp_uint_t PinCS;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(spi_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), spi_init_args, args);

    if ((args[0].u_int < 0) || (args[0].u_int > 2)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, mpexception_os_resource_not_avaliable));
    }   

    spi_obj_t *self  = m_new_obj(spi_obj_t);
    self->base.type  = &spi_type;
    self->id         = args[0].u_int;
    self->baudrate   = MIN(MAX(args[1].u_int, SPI_MIN_BAUD_RATE), SPI_MAX_BAUD_RATE);
    self->spi_mode   = args[2].u_int;
    self->bits       = args[3].u_int;
    self->trans_mode = args[4].u_int;

    switch(self->id) {
        case 0:
            self->obj = (void *)&spi_channel0;
            PinMOSI = SPI0_MOSI;
            PinMISO = SPI0_MISO;
            PinSCLK = SPI0_SCLK;
            PinCS   = SPI0_CS;
            break;
        case 1:
            self->obj = (void *)&spi_channel1;
            PinMOSI = SPI1_MOSI;
            PinMISO = SPI1_MISO;
            PinSCLK = SPI1_SCLK;
            PinCS   = SPI1_CS;
            break;
        case 2:
            self->obj = (void *)&spi_channel2;
            PinMOSI = SPI2_MOSI;
            PinMISO = SPI2_MISO;
            PinSCLK = SPI2_SCLK;
            PinCS   = SPI2_CS;
            break;
        default:
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, mpexception_os_resource_not_avaliable));
            break;
    }

    // TODO: should check alternative pins 
    spi_init((spi_t *)self->obj, PinMISO, PinMISO, PinSCLK, PinCS);

    serial_baud((spi_t *)self->obj, self->baudrate);

    spi_format((spi_t *)self->obj, self->bits, self->trans_mode, self->spi_mode);

    spi_frequency((spi_t *)self->obj, self->baudrate);

    return (mp_obj_t)self;
}


STATIC const mp_map_elem_t spi_locals_dict_table[] = {
    // instance methods
    { MP_OBJ_NEW_QSTR(MP_QSTR_send_recv),               (mp_obj_t)&spi_transcation_obj },

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
    { MP_OBJ_NEW_QSTR(MP_QSTR_SCLK_TOGGLE_START),      MP_OBJ_NEW_SMALL_INT(SPI_SCLK_TOGGLE_START) },
};
STATIC MP_DEFINE_CONST_DICT(spi_locals_dict, spi_locals_dict_table);

const mp_obj_type_t spi_type = {
    { &mp_type_type },
    .name        = MP_QSTR_SPI,
    .print       = spi_print,
    .make_new    = spi_make_new,
    .locals_dict = (mp_obj_t)&spi_locals_dict,
};
