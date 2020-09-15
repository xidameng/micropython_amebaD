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

STATIC spi_obj_t spi_obj[2] = {
    {.base.type = &spi_type, .unit = 0, .bits = 8, .baudrate = SPI_DEFAULT_BAUD_RATE, .pol = SCPOL_INACTIVE_IS_LOW, .pha = SCPH_TOGGLES_IN_MIDDLE },
    {.base.type = &spi_type, .unit = 1, .bits = 8, .baudrate = SPI_DEFAULT_BAUD_RATE, .pol = SCPOL_INACTIVE_IS_LOW, .pha = SCPH_TOGGLES_IN_MIDDLE },
};

STATIC void objspi_init(mp_obj_base_t *self_in, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    spi_obj_t *self = (spi_obj_t*)self_in;
    enum { ARG_baudrate, ARG_polarity, ARG_phase, ARG_bits, ARG_firstbit };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_baudrate, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
        { MP_QSTR_polarity, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
        { MP_QSTR_phase,    MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
        { MP_QSTR_bits,     MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
        { MP_QSTR_firstbit, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    if (args[ARG_baudrate].u_int != -1) 
        self->baudrate = args[ARG_baudrate].u_int;   

    if (args[ARG_polarity].u_int != -1)
        self->pol = args[ARG_polarity].u_int;

    if (args[ARG_phase].u_int != -1)
        self->pha = args[ARG_phase].u_int;

    if (args[ARG_bits].u_int != -1)
        self->bits = args[ARG_bits].u_int;

    //spi_init(&(self->obj), self->mosi->id, self->miso->id, self->sck->id, 0);
    switch(self->unit) {
        case 0:
            ACTCK_SPI0_CCTRL(ON);
            SLPCK_SPI0_CCTRL(ON);
            PinCtrl(SPI0, self->ssi_pinmux, ON);
            SPI0_FCTRL(ON);
            break;
        case 1:
            ACTCK_SPI1_CCTRL(ON);
            SLPCK_SPI1_CCTRL(ON);
            PinCtrl(SPI1, self->ssi_pinmux, ON);
            SPI1_FCTRL(ON);
            break;
        default:
            break;
    }

    HalSsiSetDeviceRoleRtl8195a(NULL, 1);

    HAL_SSI_WRITE32(self->unit, REG_DW_SSI_SSIENR, 0);

    SPI0_MULTI_CS_CTRL(ON);
    HAL_SSI_WRITE32(self->unit, REG_DW_SSI_SER, BIT_SER_SER(2));

    uint32_t ctrlr0_val = 0;
    
    ctrlr0_val |= BIT_CTRLR0_SCPH(self->pha);
    ctrlr0_val |= BIT_CTRLR0_SCPOL(self->pol);
    ctrlr0_val |= BIT_CTRLR0_DFS((self->bits - 1));
    ctrlr0_val |= BIT_CTRLR0_SLV_OE(0);

    HAL_SSI_WRITE32(self->unit, REG_DW_SSI_CTRLR0, ctrlr0_val);

    HAL_SSI_WRITE32(self->unit, REG_DW_SSI_MWCR, 2);
    HAL_SSI_WRITE32(self->unit, REG_DW_SSI_TXFTLR, 8);
    HAL_SSI_WRITE32(self->unit, REG_DW_SSI_RXFTLR, 7);
    

}

STATIC void objspi_deinit(mp_obj_base_t *self_in) {
    spi_obj_t *self = (spi_obj_t*)self_in;
    SPI0_MULTI_CS_CTRL(OFF);
    switch(self->unit) {
        case 0:
            ACTCK_SPI0_CCTRL(OFF);
            SLPCK_SPI0_CCTRL(OFF);
            PinCtrl(SPI0, self->ssi_pinmux, OFF);
            SPI0_FCTRL(OFF);
            break;
        case 1:
            ACTCK_SPI1_CCTRL(OFF);
            SLPCK_SPI1_CCTRL(OFF);
            PinCtrl(SPI1, self->ssi_pinmux, OFF);
            SPI1_FCTRL(OFF);
            break;
        case 2:
            ACTCK_SPI2_CCTRL(OFF);
            SLPCK_SPI2_CCTRL(OFF);
            PinCtrl(SPI2, self->ssi_pinmux, OFF);
            SPI2_FCTRL(OFF);
            break;
        default:
            break;
    }
    HAL_SSI_WRITE32(self->unit, REG_DW_SSI_SSIENR, 0);
}

STATIC void objspi_transfer(mp_obj_base_t *self_in, size_t len, const uint8_t *src, uint8_t *dest) {
    spi_obj_t *self = (spi_obj_t*)self_in;
    if (dest == NULL) {
        HAL_SSI_WRITE32(self->unit, REG_DW_SSI_SSIENR, 0);
        // tmod == 0 when send ??? weird 
        HAL_SSI_WRITE32(self->unit, REG_DW_SSI_CTRLR0, 0x07);
        HAL_SSI_WRITE32(self->unit, REG_DW_SSI_SSIENR, 1);
        for (uint i = 0; i < len; i++) {
            // Wait until SSI is not busy
            while (HAL_SSI_READ32(self->unit, REG_DW_SSI_SR) & BIT_SR_BUSY);
            // Wait until SSI TX's FIFO is not empty
            while (!(HAL_SSI_READ32(self->unit, REG_DW_SSI_SR) & BIT_SR_TFNF));
            HAL_SSI_WRITE8(self->unit ,REG_DW_SSI_DR ,src[i]);
        }
        // Wait until TX's FIFO is zero
        while (HAL_SSI_READ32(self->unit, REG_DW_SSI_TXFLR));

        // Wait until TX's FIFO is zero
        while (!(HAL_SSI_READ32(self->unit, REG_DW_SSI_SR) & BIT_SR_TFE));
    } else {
        HAL_SSI_WRITE32(self->unit, REG_DW_SSI_SSIENR, 0);
        // tmod == 0 when send ??? weird 
        HAL_SSI_WRITE32(self->unit, REG_DW_SSI_CTRLR0, 0x07);
        HAL_SSI_WRITE32(self->unit, REG_DW_SSI_SSIENR, 1);
        uint rx_fifo_count = 0;
        uint timeout_count = 1000000;
        for (uint i = 0; i < len; i++) {
            // Wait until SSI is not busy
            while (HAL_SSI_READ32(self->unit, REG_DW_SSI_SR) & BIT_SR_BUSY);
            // Wait until SSI TX's FIFO is not empty
            while (!(HAL_SSI_READ32(self->unit, REG_DW_SSI_SR) & BIT_SR_TFNF));
            HAL_SSI_WRITE8(self->unit ,REG_DW_SSI_DR ,src[i]);

            while (HAL_SSI_READ32(self->unit, REG_DW_SSI_RXFLR)) {
                dest[rx_fifo_count++] = HAL_SSI_READ8(self->unit, REG_DW_SSI_DR);
            }
        }

        if (rx_fifo_count < len) {
            while((timeout_count != 0) && (rx_fifo_count < len)) {
                while (HAL_SSI_READ32(self->unit, REG_DW_SSI_RXFLR)) {
                    dest[rx_fifo_count++] = HAL_SSI_READ8(self->unit, REG_DW_SSI_DR);
                }
                timeout_count--;
            }
        }
    }
}

STATIC void spi_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    spi_obj_t *self = self_in;
    mp_printf(print, "SPI(%d, baudrate=%u, bits=%d, MISO=%q, MOSI=%q, SCK=%q)", self->unit,
            self->baudrate, self->bits, self->miso->name, self->mosi->name, self->sck->name);
}

STATIC mp_obj_t spi_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw,
        const mp_obj_t *all_args) {
    enum { ARG_unit, ARG_baudrate, ARG_pol, ARG_pha, ARG_bits, ARG_firstbit,
        ARG_miso, ARG_mosi, ARG_sck };
    const mp_arg_t spi_init_args[] = {
        { MP_QSTR_unit,     MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_baudrate, MP_ARG_INT,                  {.u_int = SPI_DEFAULT_BAUD_RATE} },
        { MP_QSTR_polarity, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = SCPOL_INACTIVE_IS_LOW} },
        { MP_QSTR_phase,    MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = SCPH_TOGGLES_IN_MIDDLE} },
        { MP_QSTR_bits,     MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 8} },
        { MP_QSTR_firstbit, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = MICROPY_PY_MACHINE_SPI_MSB} },
        { MP_QSTR_miso,     MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_mosi,     MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_sck,      MP_ARG_REQUIRED | MP_ARG_OBJ },
    };
    // parse args
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(spi_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), spi_init_args, args);

    if (args[ARG_unit].u_int > 2)
        mp_raise_ValueError("Invalid SPI unit");

    pin_obj_t *miso = pin_find(args[ARG_miso].u_obj);
    pin_obj_t *mosi = pin_find(args[ARG_mosi].u_obj);
    pin_obj_t *sck  = pin_find(args[ARG_sck].u_obj);

    PinName pn_miso = (PinName)pinmap_peripheral(miso->id, PinMap_SPI_MISO);
    PinName pn_mosi = (PinName)pinmap_peripheral(mosi->id, PinMap_SPI_MOSI);

    if (pn_miso == NC) 
        mp_raise_ValueError("SPI MISO pin not match");

    if (pn_mosi == NC) 
        mp_raise_ValueError("SPI MOSI pin not match");

    uint32_t ssi_peri = pinmap_merge(pn_mosi, pn_miso);

    if (unlikely(ssi_peri == NC)) 
        mp_raise_ValueError("Invalid SPI pin");

    uint8_t ssi_idx = RTL_GET_PERI_IDX(ssi_peri);
    uint8_t ssi_pinmux = RTL_GET_PERI_SEL(ssi_peri);

    /* Pinmux workaround */
    if ((ssi_idx == 0) && (ssi_pinmux == SSI0_MUX_TO_GPIOC)) {
        EEPROM_PIN_CTRL(OFF);
    }

    spi_obj_t *self  = &spi_obj[args[ARG_unit].u_int];
    self->baudrate = args[ARG_baudrate].u_int;
    self->bits     = args[ARG_bits].u_int;
    self->pol      = args[ARG_pol].u_int;
    self->pha      = args[ARG_pha].u_int;
    self->miso     = miso;
    self->mosi     = mosi;
    self->sck      = sck;
    self->ssi_idx  = ssi_idx;
    self->ssi_pinmux = ssi_pinmux;

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
