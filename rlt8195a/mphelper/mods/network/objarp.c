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

#include "objarp.h"

//TODO
extern  arp_table[ARP_TABLE_SIZE];

STATIC mp_obj_t ip_arp_table(mp_obj_t self_in) {
    STATIC const qstr arp_table_info_fields[] = {
        MP_QSTR_ssid, MP_QSTR_mac
    };
    mp_obj_t arp_table_list = mp_obj_new_list(0, NULL);
    uint8_t cnt = 0;

    for (cnt=0;cnt<ARP_TABLE_SIZE;cnt++) {
        mp_obj_t tuple[2];
#if 0
        tuple[0] = mp_obj_new_str((const char *)ScanNetworkSsidCache[cnt], strlen(ScanNetworkSsidCache[cnt]), false);
        tuple[1] = mp_obj_new_bytes((const char *)ScanNetworkBssidCache[cnt], WLAN_MAC_LEN);
#endif
        mp_obj_list_append(arp_table_list, mp_obj_new_attrtuple(arp_table_info_fields, 2, tuple));
    }
    return arp_table_list;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(ip_arp_table_obj, ip_arp_table);


STATIC const mp_map_elem_t ip_arp_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_table),         (mp_obj_t)&ip_arp_table_obj },
};
STATIC MP_DEFINE_CONST_DICT(ip_arp_locals_dict, ip_arp_locals_dict_table);

STATIC void arp_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    
}

const mp_obj_type_t ip_arp_type = {
    { &mp_type_type },
    .name        = MP_QSTR_arp,
    .print       = arp_print,
    .locals_dict = (mp_obj_t)&ip_arp_locals_dict,
};
