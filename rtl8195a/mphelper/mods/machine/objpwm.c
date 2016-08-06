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
#include "objpwm.h"

pwmout_t pwm_channel0;
pwmout_t pwm_channel1;
pwmout_t pwm_channel2;
pwmout_t pwm_channel3;

STATIC void pwm_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    pwm_obj_t *self = self_in;
    mp_printf(print, "PWM(%d)", self->id);
}

STATIC mp_obj_t pwm_period(mp_obj_t self_in, mp_obj_t msec_in) {
    pwm_obj_t *self = self_in;
    pwmout_period_ms((pwmout_t *)self->obj, mp_obj_new_int(msec_in));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(pwm_period_obj, pwm_period);

STATIC mp_obj_t pwm_plusewidth(mp_obj_t self_in, mp_obj_t msec_in) {
    pwm_obj_t *self = self_in;
    pwmout_pulsewidth_ms((pwmout_t *)self->obj, mp_obj_new_int(msec_in));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(pwm_plusewidth_obj, pwm_plusewidth);

STATIC mp_obj_t pwm_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {
    const mp_arg_t pwm_init_args[] = {
        { MP_QSTR_id,                          MP_ARG_INT, {.u_int = 0} },
    };

    mp_uint_t PinPWM;
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(pwm_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), pwm_init_args, args);

    if ((args[0].u_int < 0) || (args[0].u_int > 3)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, mpexception_os_resource_not_avaliable));
    }

    pwm_obj_t *self = m_new_obj(pwm_obj_t);
    self->base.type = &pwm_type;
    self->id        = args[0].u_int;

    switch(self->id) {
        case 0:
            self->obj = (void *)&pwm_channel0;
            PinPWM = PC_0;
            break;
        case 1:
            self->obj = (void *)&pwm_channel1;
            PinPWM = PC_1;
            break;
        case 2:
            self->obj = (void *)&pwm_channel2;
            PinPWM = PC_2;
            break;
        case 3:
            self->obj = (void *)&pwm_channel3;
            PinPWM = PC_3;
            break;
        default:
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, mpexception_os_resource_not_avaliable));
            break;

    }
    pwmout_init((pwmout_t *)self->obj, PinPWM);

    return (mp_obj_t)self;
}

STATIC const mp_map_elem_t pwm_locals_dict_table[] = {
    // instance methods
    { MP_OBJ_NEW_QSTR(MP_QSTR_period),            (mp_obj_t)&pwm_period_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_plusewidth),        (mp_obj_t)&pwm_plusewidth_obj },
};
STATIC MP_DEFINE_CONST_DICT(pwm_locals_dict, pwm_locals_dict_table);

const mp_obj_type_t pwm_type = {
    { &mp_type_type },
    .name        = MP_QSTR_PWM,
    .print       = pwm_print,
    .make_new    = pwm_make_new,
    .locals_dict = (mp_obj_t)&pwm_locals_dict,
};
