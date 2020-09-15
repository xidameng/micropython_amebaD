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

#include "objpin.h"


#define PIN(p_pin_name, p_pull, p_mode, p_value) \
{ \
    {&pin_type },                     \
    .name   = MP_QSTR_ ## p_pin_name, \
    .id     = (p_pin_name),           \
    .pull   = (p_pull),               \
    .mode   = (p_mode),               \
    .value  = (p_value),              \
}


#define AF(pin_name, af_name, af_index, pull) \
{ \
    .pin        = pin_name,                                 \
    .peripheral = af_name ## af_index,                      \
    .function   = PIN_DATA(pull, PINMUX_FUNCTION_ ## af_name) \
}

const PinMap PinMap_UART_TX[] = {
    AF(PA_7, UART, _0, PullUp),
    AF(PB_19, UART, _2, PullUp),
    AF(PA_18, UART, _3, PullUp),
    
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    AF(PA_8, UART, _0, PullUp),
    AF(PB_18, UART, _2, PullUp),
    AF(PA_19, UART, _3, PullUp),
    
    {NC,    NC,     0}
};


const PinMap PinMap_I2C_SDA[] = {
    AF(PA_26, I2C, _0, PullUp),

    {NC,    NC,     0}
};

const PinMap PinMap_I2C_SCL[] = {
    AF(PA_25, I2C, _0, PullUp),
   
    {NC,    NC,     0}
};

const PinMap PinMap_PWM[] = {
    AF(PA_23, PWM, _2, PullNone),
    AF(PA_24, PWM, _3, PullNone),
    AF(PA_25,  PWM, _4, PullNone),
    AF(PA_26, PWM, _5, PullNone),

    {NC,    NC,     0}
};


#if 0
pin_obj_t pin_PA_30 = PIN(PA_30, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_29 = PIN(PA_29, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_14 = PIN(PA_14, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_15 = PIN(PA_15, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_0  = PIN(PA_0,  PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_12 = PIN(PA_12, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_5  = PIN(PA_5,  PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_18 = PIN(PA_18, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_19 = PIN(PA_19, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_22 = PIN(PA_22, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_23 = PIN(PA_23, PullNone, PIN_OUTPUT, 0);
#endif
pin_obj_t pin_PA_7 = PIN(PA_7, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_8 = PIN(PA_8, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_19 = PIN(PA_19, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PB_18 = PIN(PB_18, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PB_19 = PIN(PB_19, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_25 = PIN(PA_25, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_26 = PIN(PA_26, PullNone, PIN_OUTPUT, 0);

STATIC const mp_map_elem_t pin_board_pins_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_7), MP_OBJ_FROM_PTR(&pin_PA_7) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_8), MP_OBJ_FROM_PTR(&pin_PA_8) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_19), MP_OBJ_FROM_PTR(&pin_PA_19) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PB_18), MP_OBJ_FROM_PTR(&pin_PB_18) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PB_19), MP_OBJ_FROM_PTR(&pin_PB_19) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_25), MP_OBJ_FROM_PTR(&pin_PA_25) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_26), MP_OBJ_FROM_PTR(&pin_PA_26) },
#if 0 
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_14), MP_OBJ_FROM_PTR(&pin_PA_14) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_15), MP_OBJ_FROM_PTR(&pin_PA_15) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_0),  MP_OBJ_FROM_PTR(&pin_PA_0) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_12), MP_OBJ_FROM_PTR(&pin_PA_12) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_5),  MP_OBJ_FROM_PTR(&pin_PA_5) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_18), MP_OBJ_FROM_PTR(&pin_PA_18) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_22), MP_OBJ_FROM_PTR(&pin_PA_22) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_23), MP_OBJ_FROM_PTR(&pin_PA_23) },
#endif
};
MP_DEFINE_CONST_DICT(pin_board_pins_locals_dict, pin_board_pins_locals_dict_table);
