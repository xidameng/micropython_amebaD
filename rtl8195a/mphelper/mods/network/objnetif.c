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

/*****************************************************************************
 *                              Header includes
 * ***************************************************************************/
#include "objnetif.h"

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/
extern struct netif xnetif[NET_IF_NUM];

STATIC netif_obj_t netif_obj_0 = {
    .base.type      = &netif_type,
    .index          = 0,
};

STATIC netif_obj_t netif_obj_1 = {
    .base.type      = &netif_type,
    .index          = 1,
};

/*****************************************************************************
 *                              Local functions
 * ***************************************************************************/

void netif_init0(void) {
    struct ip_addr ipaddr;   
    struct ip_addr netmask;   
    struct ip_addr gateway;   

    IP4_ADDR(&ipaddr, DEFAULT_AP_IP_ADDR0, DEFAULT_AP_IP_ADDR1, DEFAULT_AP_IP_ADDR2, DEFAULT_AP_IP_ADDR3);
    IP4_ADDR(&netmask, DEFAULT_AP_NETMASK_ADDR0, DEFAULT_AP_NETMASK_ADDR1, DEFAULT_AP_NETMASK_ADDR2, DEFAULT_AP_NETMASK_ADDR3);
    IP4_ADDR(&gateway, DEFAULT_AP_GW_ADDR0, DEFAULT_AP_GW_ADDR1, DEFAULT_AP_GW_ADDR2, DEFAULT_AP_GW_ADDR3);

    netif_add(&xnetif[netif_obj_0.index], &ipaddr, &netmask, &gateway, NULL, &ethernetif_init, &modnetwork_input);
    netif_set_up(&xnetif[netif_obj_0.index]);

    netif_set_default(&xnetif[netif_obj_0.index]);

    IP4_ADDR(&ipaddr, DEFAULT_IP_ADDR0, DEFAULT_IP_ADDR1, DEFAULT_IP_ADDR2, DEFAULT_IP_ADDR3);
    IP4_ADDR(&netmask, DEFAULT_NETMASK_ADDR0, DEFAULT_NETMASK_ADDR1, DEFAULT_NETMASK_ADDR2, DEFAULT_NETMASK_ADDR3);
    IP4_ADDR(&gateway, DEFAULT_GW_ADDR0, DEFAULT_GW_ADDR1, DEFAULT_GW_ADDR2, DEFAULT_GW_ADDR3);

    netif_add(&xnetif[netif_obj_1.index], &ipaddr, &netmask, &gateway, NULL, &ethernetif_init, &modnetwork_input);
    netif_set_up(&xnetif[netif_obj_1.index]);
}

STATIC void netif_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    netif_obj_t *self = self_in;
    struct ip_addr ipaddr;   
    struct ip_addr netmask;   
    struct ip_addr gateway;
    ipaddr  = xnetif[self->index].ip_addr;
    netmask = xnetif[self->index].netmask;
    gateway = xnetif[self->index].gw;
    mp_printf(print, "NETIF(%d, ", self->index);
    mp_printf(print, "ip=%s ,", ip_ntoa(&ipaddr));
    mp_printf(print, "netmask=%s ,", ip_ntoa(&netmask));
    mp_printf(print, "gateway=%s)", ip_ntoa(&gateway));
}

bool dhcp_request_func(uint8_t id, u_int timeout) {
    int8_t  err      = ERR_MEM;
    uint8_t counter  = 0;
    wifi_unreg_event_handler(WIFI_EVENT_BEACON_AFTER_DHCP, wifi_rx_beacon_hdl);
    xnetif[id].ip_addr.addr = 0;
    xnetif[id].netmask.addr = 0;
    xnetif[id].gw.addr      = 0;

    err = dhcp_start(&xnetif[id]);
    if (err != ERR_OK) {
        return false;
    }
    while(timeout) {
        if (xnetif[id].ip_addr.addr != 0) {
            wifi_reg_event_handler(WIFI_EVENT_BEACON_AFTER_DHCP, wifi_rx_beacon_hdl, NULL);

            dhcp_stop(&xnetif[id]);
            return true;
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
    return false;
}

STATIC mp_obj_t ip_get(mp_obj_t self_in) {
    netif_obj_t *self = self_in;
    mp_obj_t tuple[8];
    struct ip_addr ipaddr;   
    struct ip_addr netmask;   
    struct ip_addr gateway;
    ipaddr  = xnetif[self->index].ip_addr;
    netmask = xnetif[self->index].netmask;
    gateway = xnetif[self->index].gw;
    tuple[0] = mp_obj_new_str(ip_ntoa(&ipaddr), strlen(ip_ntoa(&ipaddr)), false);
    tuple[1] = mp_obj_new_str(ip_ntoa(&netmask), strlen(ip_ntoa(&netmask)), false);
    tuple[2] = mp_obj_new_str(ip_ntoa(&gateway), strlen(ip_ntoa(&gateway)), false);
    return mp_obj_new_tuple(3, tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(ip_get_obj, ip_get);

STATIC mp_obj_t dhcp_request0(mp_obj_t self_in, mp_obj_t timeout_in) {
    netif_obj_t *self = self_in;
    bool ret = false;

    uint16_t timeout = 0;

    timeout = mp_obj_get_int(timeout_in);
    ret = dhcp_request_func(self->index, timeout);
    if (ret == false)
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_TimeoutError, "NETIF(%d) DHCP request timeout", self->index));
    else 
        return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(dhcp_request_obj, dhcp_request0);

STATIC mp_obj_t dhcp_renew0(mp_obj_t self_in) {
    int8_t err ;
    netif_obj_t *self = self_in;
    err = dhcp_renew(&xnetif[self->index]);
    if (err != ERR_OK) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "NETIF(%d) DHCP renew failed", self->index));
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(dhcp_renew_obj, dhcp_renew0);

STATIC mp_obj_t dhcp_release0(mp_obj_t self_in) {
    int8_t err;
    netif_obj_t *self = self_in;
    err = dhcp_release(&xnetif[self->index]);
    if (err != ERR_OK) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "NETIF(%d) DHCP release failed", self->index));
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(dhcp_release_obj, dhcp_release0);

STATIC mp_obj_t dhcp_inform0(mp_obj_t self_in) {
    netif_obj_t *self = self_in;
    dhcp_inform(&xnetif[self->index]);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(dhcp_inform_obj, dhcp_inform0);

STATIC mp_obj_t dhcp_state0(mp_obj_t self_in) {
    int8_t state = 0;
    netif_obj_t *self = self_in;
    state = xnetif[self->index].dhcp->state;
    return mp_obj_new_int(state);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(dhcp_state_obj, dhcp_state0);

STATIC mp_obj_t dhcp_stop0(mp_obj_t self_in) {
    netif_obj_t *self = self_in;
    dhcp_stop(&xnetif[self->index]);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(dhcp_stop_obj, dhcp_stop0);

STATIC mp_obj_t netif_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {
    STATIC const mp_arg_t netif_init_args[] = {
        { MP_QSTR_index,       MP_ARG_REQUIRED | MP_ARG_INT,  {.u_int = 0} },
    };

    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(netif_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), netif_init_args, args);

    netif_obj_t *self;

    switch(args[0].u_int) {
        case 0:
            self = &netif_obj_0;
            break;
        case 1:
            self = &netif_obj_1;
            break;
        default:
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid NETIF index"));
            break;
    }

    return (mp_obj_t)self;
}

STATIC const mp_map_elem_t netif_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_ip),           (mp_obj_t)&ip_get_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dhcp_request), (mp_obj_t)&dhcp_request_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dhcp_renew),   (mp_obj_t)&dhcp_renew_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dhcp_release), (mp_obj_t)&dhcp_release_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dhcp_inform),  (mp_obj_t)&dhcp_inform_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dhcp_state),   (mp_obj_t)&dhcp_state_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_dhcp_stop),    (mp_obj_t)&dhcp_stop_obj },
};
STATIC MP_DEFINE_CONST_DICT(netif_locals_dict, netif_locals_dict_table);

const mp_obj_type_t netif_type = {
    { &mp_type_type },
    .name        = MP_QSTR_NETIF,
    .print       = netif_print,
    .make_new    = netif_make_new,
    .locals_dict = (mp_obj_t)&netif_locals_dict,
};
