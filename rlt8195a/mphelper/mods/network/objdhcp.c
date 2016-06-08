/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
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

#include "objdhcp.h"

extern struct netif xnetif[NET_IF_NUM];

STATIC mp_obj_t ip_dhcp_request(mp_obj_t self_in, mp_obj_t timeout_in) {
    int8_t  err      = ERR_MEM;
    uint8_t counter  = 0;
    uint16_t timeout = 0;

    timeout = mp_obj_get_int(timeout_in);

    wifi_unreg_event_handler(WIFI_EVENT_BEACON_AFTER_DHCP, wifi_rx_beacon_hdl);

    xnetif[NETIF_STA_ID].ip_addr.addr = 0;
    xnetif[NETIF_STA_ID].netmask.addr = 0;
    xnetif[NETIF_STA_ID].gw.addr      = 0;

    err = dhcp_start(&xnetif[NETIF_STA_ID]);

    if (err != ERR_OK) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_MemoryError, "lwip dhcp memory error"));
    }

    while(timeout) {
        if (xnetif[NETIF_STA_ID].ip_addr.addr != 0) {
            wifi_reg_event_handler(WIFI_EVENT_BEACON_AFTER_DHCP, wifi_rx_beacon_hdl, NULL);
            dhcp_stop(&xnetif[NETIF_STA_ID]);
            return mp_const_none;
        }
        mp_hal_delay_ms(DHCP_FINE_TIMER_MSECS);
        dhcp_fine_tmr();
        counter += DHCP_FINE_TIMER_MSECS;
        if (counter >= DHCP_COARSE_TIMER_SECS*1000) {
            dhcp_coarse_tmr();
            counter = 0;
            timeout--;
        }
    }

    nlr_raise(mp_obj_new_exception_msg(&mp_type_MemoryError, "dhcp request timeout"));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(ip_dhcp_request_method_obj, ip_dhcp_request);
STATIC MP_DEFINE_CONST_CLASSMETHOD_OBJ(ip_dhcp_request_method, (const mp_obj_t)&ip_dhcp_request_method_obj);

STATIC mp_obj_t ip_dhcp_renew(mp_obj_t self_in) {
    int8_t err ;

    err = dhcp_renew(&xnetif[NETIF_STA_ID]);

    if (err != ERR_OK) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_MemoryError, "dhcp renew failed"));
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(ip_dhcp_renew_method, ip_dhcp_renew);

STATIC mp_obj_t ip_dhcp_release(mp_obj_t self_in) {
    int8_t err;
    err = ip_dhcp_release(&xnetif[NETIF_STA_ID]);
    if (err != ERR_OK) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_MemoryError, "dhcp release failed"));
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(ip_dhcp_release_method, ip_dhcp_release);

STATIC mp_obj_t ip_dhcp_inform(mp_obj_t self_in) {
    dhcp_inform(&xnetif[NETIF_STA_ID]);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(ip_dhcp_inform_method, ip_dhcp_inform);

STATIC mp_obj_t ip_dhcp_state(mp_obj_t self_in) {
    int8_t state = 0;
    state = xnetif[NETIF_STA_ID].dhcp->state;
    return MP_OBJ_NEW_SMALL_INT(state);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(ip_dhcp_state_method, ip_dhcp_state);

STATIC mp_obj_t ip_dhcp_stop(mp_obj_t self_in) {
    dhcp_stop(&xnetif[NETIF_STA_ID]);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(ip_dhcp_stop_method, ip_dhcp_stop);

STATIC const mp_map_elem_t ip_dhcp_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_request),     (mp_obj_t)&ip_dhcp_request_method },
    { MP_OBJ_NEW_QSTR(MP_QSTR_renew),       (mp_obj_t)&ip_dhcp_renew_method },
    { MP_OBJ_NEW_QSTR(MP_QSTR_release),     (mp_obj_t)&ip_dhcp_release_method },
    { MP_OBJ_NEW_QSTR(MP_QSTR_inform),      (mp_obj_t)&ip_dhcp_inform_method },
    { MP_OBJ_NEW_QSTR(MP_QSTR_state),       (mp_obj_t)&ip_dhcp_state_method },
    { MP_OBJ_NEW_QSTR(MP_QSTR_stop),        (mp_obj_t)&ip_dhcp_stop_method },
};
STATIC MP_DEFINE_CONST_DICT(ip_dhcp_locals_dict, ip_dhcp_locals_dict_table);

const mp_obj_type_t ip_dhcp_type = {
    { &mp_type_type },
    .name        = MP_QSTR_dhcp,
    .locals_dict = (mp_obj_t)&ip_dhcp_locals_dict,
};
