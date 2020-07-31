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

#ifdef AMEBA1
#define AF(pin_name, af_name, af_index, af_sel) \
{ \
    .pin            = pin_name,                                 \
    .peripheral     = RTL_PIN_PERI(af_name ## af_index, af_index, af_sel),  \
    .function       = RTL_PIN_FUNC(af_name ## af_index, af_sel)             \
}
const PinMap PinMap_UART_TX[] = {
    AF(PC_3, UART, 0, S0),
    AF(PE_0, UART, 0, S1),
    AF(PA_7, UART, 0, S2),

    AF(PD_3, UART, 1, S0),
    AF(PE_4, UART, 1, S1),
    AF(PB_5, UART, 1, S2),

    AF(PA_4, UART, 2, S0),
    AF(PC_9, UART, 2, S1),
    AF(PD_7, UART, 2, S2),

    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    AF(PC_0, UART, 0, S0),
    AF(PE_3, UART, 0, S1),
    AF(PA_6, UART, 0, S2),

    AF(PD_0, UART, 1, S0),
    AF(PE_7, UART, 1, S1),
    AF(PB_4, UART, 1, S2),

    AF(PA_0, UART, 2, S0),
    AF(PC_6, UART, 2, S1),
    AF(PD_4, UART, 2, S2),

    {NC,    NC,     0}
};


const PinMap PinMap_I2C_SDA[] = {
    AF(PD_4, I2C, 0, S0),
    AF(PH_1, I2C, 0, S1),
    AF(PC_8, I2C, 0, S2),
    AF(PE_7, I2C, 0, S3),

    AF(PC_4, I2C, 1, S0),
    AF(PH_3, I2C, 1, S1),
    AF(PD_7, I2C, 1, S2),

    AF(PB_7, I2C, 2, S0),
    AF(PE_1, I2C, 2, S1),
    AF(PC_7, I2C, 2, S2),

    AF(PB_3, I2C, 3, S0),
    AF(PE_3, I2C, 3, S1),
    AF(PE_5, I2C, 3, S2),
    AF(PD_9, I2C, 3, S3),

    {NC,    NC,     0}
};

const PinMap PinMap_I2C_SCL[] = {
    AF(PD_5, I2C, 0, S0),
    AF(PH_0, I2C, 0, S1),
    AF(PC_9, I2C, 0, S2),
    AF(PE_6, I2C, 0, S3),

    AF(PC_5, I2C, 1, S0),
    AF(PH_2, I2C, 1, S1),
    AF(PD_6, I2C, 1, S2),

    AF(PB_6, I2C, 2, S0),
    AF(PE_0, I2C, 2, S1),
    AF(PC_6, I2C, 2, S2),

    AF(PB_2, I2C, 3, S0),
    AF(PE_2, I2C, 3, S1),
    AF(PE_4, I2C, 3, S2),
    AF(PD_8, I2C, 3, S3),

    {NC,    NC,     0}
};

const PinMap PinMap_SPI_MOSI[] = {
    AF(PE_2, SPI, 0, S0),
    AF(PC_2, SPI, 0, S1),

    AF(PA_1, SPI, 1, S0),
    AF(PB_6, SPI, 1, S1),
    AF(PD_6, SPI, 1, S2),

    AF(PG_2, SPI, 2, S0),
    AF(PE_6, SPI, 2, S1),
    AF(PD_2, SPI, 2, S2),

    {NC,    NC,     0}
};

const PinMap PinMap_SPI_MISO[] = {
    AF(PE_3, SPI, 0, S0),
    AF(PC_3, SPI, 0, S1),

    AF(PA_0, SPI, 1, S0),
    AF(PB_7, SPI, 1, S1),
    AF(PD_7, SPI, 1, S2),

    AF(PG_3, SPI, 2, S0),
    AF(PE_7, SPI, 2, S1),
    AF(PD_3, SPI, 2, S2),

    {NC,    NC,     0}
};

const PinMap PinMap_PWM[] = {
    AF(PB_4, PWM, 0, S0),
    AF(PB_5, PWM, 1, S0),
    AF(PB_6, PWM, 2, S0),
    AF(PB_7, PWM, 3, S0),

    AF(PC_0, PWM, 0, S1),
    AF(PC_1, PWM, 1, S1),
    AF(PC_2, PWM, 2, S1),
    AF(PC_3, PWM, 3, S1),

    AF(PD_3, PWM, 0, S2),
    AF(PD_4, PWM, 1, S2),
    AF(PD_5, PWM, 2, S2),
    AF(PD_6, PWM, 3, S2),

    AF(PE_0, PWM, 0, S3),
    AF(PE_1, PWM, 1, S3),
    AF(PE_2, PWM, 2, S3),
    AF(PE_3, PWM, 3, S3),

    {NC,    NC,     0}
};

pin_obj_t pin_PA_0 = PIN(PA_0, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_1 = PIN(PA_1, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_2 = PIN(PA_2, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_3 = PIN(PA_3, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_4 = PIN(PA_4, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_5 = PIN(PA_5, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_6 = PIN(PA_6, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PA_7 = PIN(PA_7, PullNone, PIN_OUTPUT, 0);

pin_obj_t pin_PB_0 = PIN(PB_0, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PB_1 = PIN(PB_1, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PB_2 = PIN(PB_2, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PB_3 = PIN(PB_3, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PB_4 = PIN(PB_4, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PB_5 = PIN(PB_5, PullNone, PIN_OUTPUT, 0);

pin_obj_t pin_PC_0 = PIN(PC_0, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PC_1 = PIN(PC_1, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PC_2 = PIN(PC_2, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PC_3 = PIN(PC_3, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PC_4 = PIN(PC_4, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PC_5 = PIN(PC_5, PullNone, PIN_OUTPUT, 0);

pin_obj_t pin_PD_4 = PIN(PD_4, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PD_5 = PIN(PD_5, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PD_6 = PIN(PD_6, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PD_7 = PIN(PD_7, PullNone, PIN_OUTPUT, 0);

pin_obj_t pin_PE_0 = PIN(PE_0, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PE_1 = PIN(PE_1, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PE_2 = PIN(PE_2, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PE_3 = PIN(PE_3, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PE_4 = PIN(PE_4, PullNone, PIN_OUTPUT, 0);
pin_obj_t pin_PE_5 = PIN(PE_5, PullNone, PIN_OUTPUT, 0);

STATIC const mp_map_elem_t pin_board_pins_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_0), MP_OBJ_FROM_PTR(&pin_PA_0) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_1), MP_OBJ_FROM_PTR(&pin_PA_1) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_2), MP_OBJ_FROM_PTR(&pin_PA_2) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_3), MP_OBJ_FROM_PTR(&pin_PA_3) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_4), MP_OBJ_FROM_PTR(&pin_PA_4) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_5), MP_OBJ_FROM_PTR(&pin_PA_5) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_6), MP_OBJ_FROM_PTR(&pin_PA_6) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_7), MP_OBJ_FROM_PTR(&pin_PA_7) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_PB_0), MP_OBJ_FROM_PTR(&pin_PB_0) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PB_1), MP_OBJ_FROM_PTR(&pin_PB_1) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PB_2), MP_OBJ_FROM_PTR(&pin_PB_2) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PB_3), MP_OBJ_FROM_PTR(&pin_PB_3) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PB_4), MP_OBJ_FROM_PTR(&pin_PB_4) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PB_5), MP_OBJ_FROM_PTR(&pin_PB_5) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_PC_0), MP_OBJ_FROM_PTR(&pin_PC_0) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PC_1), MP_OBJ_FROM_PTR(&pin_PC_1) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PC_2), MP_OBJ_FROM_PTR(&pin_PC_2) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PC_3), MP_OBJ_FROM_PTR(&pin_PC_3) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PC_4), MP_OBJ_FROM_PTR(&pin_PC_4) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PC_5), MP_OBJ_FROM_PTR(&pin_PC_5) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_PD_4), MP_OBJ_FROM_PTR(&pin_PD_4) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PD_5), MP_OBJ_FROM_PTR(&pin_PD_5) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PD_6), MP_OBJ_FROM_PTR(&pin_PD_6) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PD_7), MP_OBJ_FROM_PTR(&pin_PD_7) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_PE_0), MP_OBJ_FROM_PTR(&pin_PE_0) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PE_1), MP_OBJ_FROM_PTR(&pin_PE_1) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PE_2), MP_OBJ_FROM_PTR(&pin_PE_2) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PE_3), MP_OBJ_FROM_PTR(&pin_PE_3) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PE_4), MP_OBJ_FROM_PTR(&pin_PE_4) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PE_5), MP_OBJ_FROM_PTR(&pin_PE_5) },
};
MP_DEFINE_CONST_DICT(pin_board_pins_locals_dict, pin_board_pins_locals_dict_table);

#elif defined(MP_AMEBAZ)

#define AF(pin_name, af_name, af_index, pull) \
{ \
    .pin        = pin_name,                                 \
    .peripheral = af_name ## af_index,                      \
    .function   = PIN_DATA(pull, PINMUX_FUNCTION_ ## af_name) \
}

const PinMap PinMap_UART_TX[] = {
    AF(PA_23, UART, _0, PullUp),
    AF(PA_30, UART, _2, PullUp),

    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    AF(PA_18, UART, _0, PullUp),
    AF(PA_29, UART, _2, PullUp),

    {NC,    NC,     0}
};

const PinMap PinMap_I2C_SDA[] = {
    AF(PA_19, I2C, _0, PullUp),
    AF(PA_23, I2C, _1, PullUp),

    {NC,    NC,     0}
};

const PinMap PinMap_I2C_SCL[] = {
    AF(PA_22, I2C, _0, PullUp),
    AF(PA_18, I2C, _1, PullUp),
   
    {NC,    NC,     0}
};

const PinMap PinMap_PWM[] = {
    AF(PA_14, PWM, _0, PullNone),
    AF(PA_15, PWM, _1, PullNone),
    AF(PA_0,  PWM, _2, PullNone),
    AF(PA_12, PWM, _3, PullNone),
    AF(PA_5,  PWM, _4, PullNone),
    AF(PA_23, PWM, _5, PullNone),

    {NC,    NC,     0}
};

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

STATIC const mp_map_elem_t pin_board_pins_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_30), MP_OBJ_FROM_PTR(&pin_PA_30) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_29), MP_OBJ_FROM_PTR(&pin_PA_29) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_14), MP_OBJ_FROM_PTR(&pin_PA_14) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_15), MP_OBJ_FROM_PTR(&pin_PA_15) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_0),  MP_OBJ_FROM_PTR(&pin_PA_0) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_12), MP_OBJ_FROM_PTR(&pin_PA_12) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_5),  MP_OBJ_FROM_PTR(&pin_PA_5) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_18), MP_OBJ_FROM_PTR(&pin_PA_18) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_19), MP_OBJ_FROM_PTR(&pin_PA_19) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_22), MP_OBJ_FROM_PTR(&pin_PA_22) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_23), MP_OBJ_FROM_PTR(&pin_PA_23) },
};
MP_DEFINE_CONST_DICT(pin_board_pins_locals_dict, pin_board_pins_locals_dict_table);


#elif defined(AMEBAD)

#define AF(pin_name, af_name, af_index, pull) \
{ \
    .pin        = pin_name,                                 \
    .peripheral = af_name ## af_index,                      \
    .function   = PIN_DATA(pull, PINMUX_FUNCTION_ ## af_name) \
}

const PinMap PinMap_UART_TX[] = {
    AF(PA_7, UART, _0, PullUp),
    //AF(PA_30, UART, _2, PullUp),

    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    AF(PA_8, UART, _0, PullUp),
    //AF(PA_29, UART, _2, PullUp),

    {NC,    NC,     0}
};

#if 0
const PinMap PinMap_I2C_SDA[] = {
    AF(PA_19, I2C, _0, PullUp),
    AF(PA_23, I2C, _1, PullUp),

    {NC,    NC,     0}
};

const PinMap PinMap_I2C_SCL[] = {
    AF(PA_22, I2C, _0, PullUp),
    AF(PA_18, I2C, _1, PullUp),
   
    {NC,    NC,     0}
};

const PinMap PinMap_PWM[] = {
    AF(PA_14, PWM, _0, PullNone),
    AF(PA_15, PWM, _1, PullNone),
    AF(PA_0,  PWM, _2, PullNone),
    AF(PA_12, PWM, _3, PullNone),
    AF(PA_5,  PWM, _4, PullNone),
    AF(PA_23, PWM, _5, PullNone),

    {NC,    NC,     0}
};

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

STATIC const mp_map_elem_t pin_board_pins_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_7), MP_OBJ_FROM_PTR(&pin_PA_7) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_8), MP_OBJ_FROM_PTR(&pin_PA_8) },
#if 0 
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_14), MP_OBJ_FROM_PTR(&pin_PA_14) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_15), MP_OBJ_FROM_PTR(&pin_PA_15) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_0),  MP_OBJ_FROM_PTR(&pin_PA_0) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_12), MP_OBJ_FROM_PTR(&pin_PA_12) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_5),  MP_OBJ_FROM_PTR(&pin_PA_5) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_18), MP_OBJ_FROM_PTR(&pin_PA_18) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_19), MP_OBJ_FROM_PTR(&pin_PA_19) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_22), MP_OBJ_FROM_PTR(&pin_PA_22) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PA_23), MP_OBJ_FROM_PTR(&pin_PA_23) },
#endif
};
MP_DEFINE_CONST_DICT(pin_board_pins_locals_dict, pin_board_pins_locals_dict_table);
#else

#endif
