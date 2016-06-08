/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 * Copyright (c) 2015 Galen Hazelwood
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

#include "objip.h"

extern struct netif xnetif[NET_IF_NUM];

void ip_init0(void) {
    struct ip_addr ipaddr;   
    struct ip_addr netmask;   
    struct ip_addr gateway;   

    // It can only be init once 
    tcpip_init(NULL, NULL);

    IP4_ADDR(&ipaddr, DEFAULT_IP_ADDR0, DEFAULT_IP_ADDR1, DEFAULT_IP_ADDR2, DEFAULT_IP_ADDR3);
    IP4_ADDR(&netmask, DEFAULT_NETMASK_ADDR0, DEFAULT_NETMASK_ADDR1, DEFAULT_NETMASK_ADDR2, DEFAULT_NETMASK_ADDR3);
    IP4_ADDR(&gateway, DEFAULT_GW_ADDR0, DEFAULT_GW_ADDR1, DEFAULT_GW_ADDR2, DEFAULT_GW_ADDR3);
    netif_add(&xnetif[NETIF_STA_ID], &ipaddr, &netmask, &gateway, NULL, &ethernetif_init, &tcpip_input);

    IP4_ADDR(&ipaddr, DEFAULT_AP_IP_ADDR0, DEFAULT_AP_IP_ADDR1, DEFAULT_AP_IP_ADDR2, DEFAULT_AP_IP_ADDR3);
    IP4_ADDR(&netmask, DEFAULT_AP_NETMASK_ADDR0, DEFAULT_AP_NETMASK_ADDR1, DEFAULT_AP_NETMASK_ADDR2, DEFAULT_AP_NETMASK_ADDR3);
    IP4_ADDR(&gateway, DEFAULT_AP_GW_ADDR0, DEFAULT_AP_GW_ADDR1, DEFAULT_AP_GW_ADDR2, DEFAULT_AP_GW_ADDR3);
    netif_add(&xnetif[NETIF_AP_ID], &ipaddr, &netmask, &gateway, NULL, &ethernetif_init, &tcpip_input);
}

STATIC void ip_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    lwip_obj_t *self = self_in;
    struct ip_addr ipaddr;   
    struct ip_addr netmask;   
    struct ip_addr gateway;
    ipaddr  = xnetif[NETIF_STA_ID].ip_addr;
    netmask = xnetif[NETIF_STA_ID].netmask;
    gateway = xnetif[NETIF_STA_ID].gw;
    mp_printf(print, "lwip(ip=%s, netmask=%s, gateway=%s)", ip_ntoa(&ipaddr), ip_ntoa(&netmask), ip_ntoa(&gateway));
}

STATIC mp_obj_t ip_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {
    STATIC const mp_arg_t ip_init_args[] = {
#if 0
        { MP_QSTR_ip,         MP_ARG_KW_ONLY  | MP_ARG_OBJ,  {.u_obj = mp_const_none} },
#endif
    };

    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(ip_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), ip_init_args, args);

    lwip_obj_t *self = m_new_obj(lwip_obj_t);
    memset(self, 0x0, sizeof(*self));

    self->base.type = &ip_type;
    netif_set_up(&xnetif[NETIF_STA_ID]);

    return (mp_obj_t)self;
}


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
STATIC MP_DEFINE_CONST_FUN_OBJ_2(ip_dhcp_request_method, ip_dhcp_request);

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

STATIC const mp_map_elem_t ip_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_dhcp_request),     (mp_obj_t)&ip_dhcp_request_method },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dhcp_renew),       (mp_obj_t)&ip_dhcp_renew_method },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dhcp_release),     (mp_obj_t)&ip_dhcp_release_method },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dhcp_inform),      (mp_obj_t)&ip_dhcp_inform_method },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dhcp_state),       (mp_obj_t)&ip_dhcp_state_method },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dhcp_stop),        (mp_obj_t)&ip_dhcp_stop_method },

};
STATIC MP_DEFINE_CONST_DICT(ip_locals_dict, ip_locals_dict_table);

const mp_obj_type_t ip_type = {
    { &mp_type_type },
    .name        = MP_QSTR_ip,
    .print       = ip_print,
    .make_new    = ip_make_new,
    .locals_dict = (mp_obj_t)&ip_locals_dict,
};
