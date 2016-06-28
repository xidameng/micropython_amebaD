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

#include "objpin.h"
#include "objadc.h"

analogin_t adc_channel0;
analogin_t adc_channel1;
analogin_t adc_channel2;

STATIC mp_obj_t adc_read(mp_obj_t self_in) {
    mp_obj_t tuple[2];
    adc_obj_t *self = self_in;
    float readFloat = analogin_read((analogin_t *)self->obj);
    uint16_t readInt = analogin_read_u16((analogin_t *)self->obj);
    tuple[0] = mp_obj_new_float(readFloat);
    tuple[1] = mp_obj_new_int(readInt);
    return mp_obj_new_tuple(2, tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(adc_read_obj, adc_read);

STATIC void adc_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    adc_obj_t *self = self_in;
    mp_printf(print, "ADC(%d)", self->id);
}

STATIC mp_obj_t adc_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {
    const mp_arg_t adc_init_args[] = {
        { MP_QSTR_id,                          MP_ARG_INT, {.u_int = 0} },
    };

    mp_uint_t PinADC;
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(adc_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), adc_init_args, args);

    if ((args[0].u_int < 0) || (args[0].u_int > 2)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, mpexception_os_resource_not_avaliable));
    }

    adc_obj_t *self = m_new_obj(adc_obj_t);
    self->base.type = &adc_type;
    self->id        = args[0].u_int;

    switch(self->id) {
        case 0:
            self->obj = (void *)&adc_channel0;
            PinADC = AD_1;
            break;
        case 1:
            self->obj = (void *)&adc_channel0;
            PinADC = AD_2;
            break;
        case 2:
            self->obj = (void *)&adc_channel0;
            PinADC = AD_3;
            break;
        default:
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, mpexception_os_resource_not_avaliable));
            break;

    }
    analogin_init((analogin_t *)self->obj, PinADC);

    return (mp_obj_t)self;
}

STATIC const mp_map_elem_t adc_locals_dict_table[] = {
    // instance methods
    { MP_OBJ_NEW_QSTR(MP_QSTR_read),                (mp_obj_t)&adc_read_obj },
};
STATIC MP_DEFINE_CONST_DICT(adc_locals_dict, adc_locals_dict_table);

const mp_obj_type_t adc_type = {
    { &mp_type_type },
    .name        = MP_QSTR_ADC,
    .print       = adc_print,
    .make_new    = adc_make_new,
    .locals_dict = (mp_obj_t)&adc_locals_dict,
};
