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

#include "objspi.h"

STATIC spi_obj_t spi_obj =
{ .base.type = &spi_type, .bits = 8, .baudrate = SPI_DEFAULT_BAUD_RATE,
  .pol = SCPOL_INACTIVE_IS_LOW, .pha = SCPH_TOGGLES_IN_MIDDLE };

STATIC void objspi_init(mp_obj_base_t *self_in, size_t n_args, const mp_obj_t *pos_args,
    mp_map_t *kw_args) {
    enum { ARG_baudrate, ARG_polarity, ARG_phase, ARG_bits, ARG_firstbit };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_baudrate, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
        { MP_QSTR_polarity, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
        { MP_QSTR_phase,    MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
        { MP_QSTR_bits,     MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
        { MP_QSTR_firstbit, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    };
    spi_obj_t *self = (spi_obj_t*)self_in;
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), 
        allowed_args, args);

    if (args[ARG_baudrate].u_int != -1) 
        self->baudrate = args[ARG_baudrate].u_int;   

    if (args[ARG_polarity].u_int != -1)
        self->pol = args[ARG_polarity].u_int;

    if (args[ARG_phase].u_int != -1)
        self->pha = args[ARG_phase].u_int;

    if (args[ARG_bits].u_int != -1)
        self->bits = args[ARG_bits].u_int;

    SSI_Cmd(SPI_DEV_TABLE[1].SPIx, DISABLE);   
    SSI_SetSclkPhase(SPI_DEV_TABLE[1].SPIx, self->pha);
    SSI_SetSclkPolarity(SPI_DEV_TABLE[1].SPIx, self->pol);
    SSI_SetDataFrameSize(SPI_DEV_TABLE[1].SPIx, (self->bits - 1));
    SSI_SetBaud(SPI_DEV_TABLE[1].SPIx, self->baudrate, CPU_ClkGet(FALSE)/2);
    SSI_Cmd(SPI_DEV_TABLE[1].SPIx, ENABLE);
}

STATIC void objspi_deinit(mp_obj_base_t *self_in) {
    spi_obj_t *self = (spi_obj_t*)self_in;
    SSI_Cmd(SPI_DEV_TABLE[1].SPIx, DISABLE);
}

static void spi_dma_tx_irq(void *arg) {
    spi_obj_t *self = (spi_obj_t*)arg;
    PGDMA_InitTypeDef gdma_init_struct;
    gdma_init_struct = &(self->SSITxGdmaInitStruct);
    GDMA_ClearINT(gdma_init_struct->GDMA_Index, gdma_init_struct->GDMA_ChNum);
    GDMA_Cmd(gdma_init_struct->GDMA_Index, gdma_init_struct->GDMA_ChNum, DISABLE);
    
}

STATIC void objspi_transfer(mp_obj_base_t *self_in, size_t len, const uint8_t *src, uint8_t *dest) {
    spi_obj_t *self = (spi_obj_t*)self_in;
    if (dest == NULL) {
       
    } else if (src == NULL) {

    } else {

    }
}

STATIC void spi_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    spi_obj_t *self = self_in;
    mp_printf(print, "SPI(%d, baudrate=%u, bits=%d, pol=%d, pha=%d)",
        self->unit, self->baudrate, self->bits, self->pol, self->pha);
}

STATIC mp_obj_t spi_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw,
        const mp_obj_t *all_args) {
    enum { ARG_baudrate, ARG_pol, ARG_pha, ARG_bits, ARG_firstbit };
    const mp_arg_t spi_init_args[] = {
        { MP_QSTR_baudrate, MP_ARG_INT,                  {.u_int = SPI_DEFAULT_BAUD_RATE} },
        { MP_QSTR_polarity, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = SCPOL_INACTIVE_IS_LOW} },
        { MP_QSTR_phase,    MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = SCPH_TOGGLES_IN_MIDDLE} },
        { MP_QSTR_bits,     MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 8} },
        { MP_QSTR_firstbit, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = MICROPY_PY_MACHINE_SPI_MSB} },
    };
    // parse args
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(spi_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), spi_init_args, args);

    spi_obj_t *self  = &spi_obj;
    self->baudrate = args[ARG_baudrate].u_int;
    self->bits     = args[ARG_bits].u_int;
    self->pol      = args[ARG_pol].u_int;
    self->pha      = args[ARG_pha].u_int;

    // There's only one set of pins which is specific for SPI (master or slave)
    RCC_PeriphClockCmd(APBPeriph_SPI1, APBPeriph_SPI1_CLOCK, ENABLE);
    Pinmux_Config(PA_23, PINMUX_FUNCTION_SPIM);
    Pinmux_Config(PA_22, PINMUX_FUNCTION_SPIM);
    Pinmux_Config(PA_18, PINMUX_FUNCTION_SPIM);

    SSI_InitTypeDef ssi_init_struct;
    
    ssi_init_struct.SPI_Role = SSI_MASTER;
    ssi_init_struct.SPI_DataFrameFormat = FRF_MOTOROLA_SPI;
    ssi_init_struct.SPI_DataFrameSize = (self->bits - 1);
    ssi_init_struct.SPI_SclkPhase = self->pha;
    ssi_init_struct.SPI_SclkPolarity = self->pol;
    ssi_init_struct.SPI_TransferMode = TMOD_TR;
    ssi_init_struct.SPI_SlaveSelectEnable = 0;

    SSI_StructInit(&ssi_init_struct);
    // To ensure struct init as the master
    ssi_init_struct.SPI_Role = SSI_MASTER;

    SSI_Init(SPI_DEV_TABLE[1].SPIx, &ssi_init_struct);
    SSI_Cmd(SPI_DEV_TABLE[1].SPIx, ENABLE);
    return (mp_obj_t)self;
}

STATIC const mp_machine_spi_p_t spi_protocol_p = {
    .init     = objspi_init,
    .deinit   = objspi_deinit,
    .transfer = objspi_transfer,
};

const mp_obj_type_t spi_type = {
    { &mp_type_type },
    .name        = MP_QSTR_SPI,
    .print       = spi_print,
    .make_new    = spi_make_new,
    .protocol    = &spi_protocol_p,
    .locals_dict = (mp_obj_t)&mp_machine_spi_locals_dict,
};
