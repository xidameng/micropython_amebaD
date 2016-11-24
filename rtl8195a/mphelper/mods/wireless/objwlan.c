/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
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

#include "objwlan.h"
#include "modnetwork.h"
#include "objnetif.h"

#if 0 // TODO(Chester) remove xnetif
extern struct netif xnetif[NET_IF_NUM];
#endif

/*****************************************************************************
 *                              Local variables
 * ***************************************************************************/
uint8_t  ScanNetworkCounter = 0;
int8_t   ScanNetworkSsidCache[WLAN_MAX_SCAN_NETWORKS][WLAN_MAX_SSID_LEN] = {0};
int8_t   ScanNetworkBssidCache[WLAN_MAX_SCAN_NETWORKS][WLAN_MAC_LEN]     = {0};
int8_t   ScanNetworkBssCache[WLAN_MAX_SCAN_NETWORKS]                     = {0};
int8_t   ScanNetworkSecurityCache[WLAN_MAX_SCAN_NETWORKS]                = {0};
int8_t   ScanNetworkWpsCache[WLAN_MAX_SCAN_NETWORKS]                     = {0};
uint8_t  ScanNetworkChannelCache[WLAN_MAX_SCAN_NETWORKS]                 = {0};
uint8_t  ScanNetworkBandCache[WLAN_MAX_SCAN_NETWORKS]                    = {0};
int16_t  ScanNetworkRssiCache[WLAN_MAX_SCAN_NETWORKS]                    = {0};

// Two global wlan object, RTL8195A has 2 WLAN interface.
STATIC wlan_obj_t wlan_obj_0 = {
    .base.type      = &wlan_type,
    .index          = 0,
    .mode           = RTW_MODE_AP,
    .security_type  = RTW_SECURITY_OPEN,
    .channel        = 6,
    .ifname         = "wlan0", 
    .ssid           = MICROPY_WLAN_AP_DEFAULT_SSID, 
    .key            = MICROPY_WLAN_AP_DEFAULT_PASS,
    .netif          = 0,
};

STATIC wlan_obj_t wlan_obj_1 = {
    .base.type      = &wlan_type,
    .index          = 1,
    .mode           = RTW_MODE_AP,
    .security_type  = RTW_SECURITY_OPEN,
    .channel        = 6,
    .ifname         = "wlan1",
    .ssid           = MICROPY_WLAN_AP_DEFAULT_SSID, 
    .key            = MICROPY_WLAN_AP_DEFAULT_PASS,
    .netif          = 1,
};

/*****************************************************************************
 *                              Local functions
 * ***************************************************************************/
void validate_wlan_mode(uint8_t mode) {
    if (mode != RTW_MODE_AP &&
        mode != RTW_MODE_STA &&
        mode != RTW_MODE_STA_AP &&
        mode != RTW_MODE_PROMISC &&
        mode != RTW_MODE_P2P) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid WLAN mode"));
    }
}

void validate_ssid(int8_t **ssid, uint8_t *len, mp_obj_t obj) {
    uint8_t ssid_len = 0;

    if (obj != MP_OBJ_NULL) {
        *ssid = mp_obj_str_get_data(obj, &ssid_len);
        *len = ssid_len;
        if (ssid_len > WLAN_MAX_SSID_LEN || ssid_len < WLAN_MIN_SSID_LEN)
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Wrong SSID length"));
    } else {
        *ssid = NULL;
    }
}

void validate_key(int8_t **key, uint8_t *key_len, uint32_t *sec_type, mp_obj_t obj) {
    mp_obj_t *sec;

    if (obj != mp_const_none) {

        mp_obj_get_array_fixed_n(obj, 2, &sec);

        *sec_type = mp_obj_get_int(sec[0]);
        *key = mp_obj_str_get_data(sec[1], key_len);

        if (*sec_type != RTW_SECURITY_OPEN &&
            *sec_type != RTW_SECURITY_WEP_PSK &&
            *sec_type != RTW_SECURITY_WEP_SHARED &&
            *sec_type != RTW_SECURITY_WPA_TKIP_PSK &&
            *sec_type != RTW_SECURITY_WPA_AES_PSK &&
            *sec_type != RTW_SECURITY_WPA2_TKIP_PSK &&
            *sec_type != RTW_SECURITY_WPA2_AES_PSK &&
            *sec_type != RTW_SECURITY_WPA2_MIXED_PSK &&
            *sec_type != RTW_SECURITY_WPA_WPA2_MIXED &&
            *sec_type != RTW_SECURITY_WPS_OPEN &&
            *sec_type != RTW_SECURITY_WPS_SECURE) {
                nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid security type"));
        }

        if (*sec_type == RTW_SECURITY_WPA_TKIP_PSK ||
            *sec_type == RTW_SECURITY_WPA_AES_PSK ||
            *sec_type == RTW_SECURITY_WPA2_TKIP_PSK ||
            *sec_type == RTW_SECURITY_WPA2_AES_PSK ||
            *sec_type == RTW_SECURITY_WPA2_MIXED_PSK ||
            *sec_type == RTW_SECURITY_WPA_WPA2_MIXED) {
            if (*key_len > WLAN_WPA_MAC_PASS_CHAR_LEN ||
                *key_len < WLAN_WPA_MIN_PASS_CHAR_LEN) {
                nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Wrong WPA passphrase length"));
            }
        }

        if (*sec_type == RTW_SECURITY_WEP_SHARED || 
            *sec_type == RTW_SECURITY_WEP_PSK) { 
            int8_t *key_temp = *key;

            if (*key_len > WLAN_WEP_MAX_PASS_CHAR_LEN ||
                *key_len < WLAN_WEP_MIN_PASS_CHAR_LEN) {
                nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Wrong WEP key length"));
            }

            for (uint8_t i=0; i<*key_len; i++) {
                //TODO(Chester) WEP key is digital char only?
                if (!unichar_isdigit(key_temp[i])) {
                    nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Wrong WEP key value"));
                }
            }

            //TODO(Chester) Should consider generate WEP key in bytearray format
            uint8_t wep_key[WLAN_WEP_MAX_PASS_CHAR_LEN];
            memset(wep_key, 0x0, sizeof(wep_key));
            for (uint8_t i=0, idx=0; i < strlen(key_temp); i++) {
                wep_key[idx] += unichar_xdigit_value(key_temp[i]);
                if ((i % 2) == 0) {
                    wep_key[idx] *= 16;
                } else {
                    idx++;
                }
            }
            memcpy(*key, wep_key, sizeof(wep_key));
            *key_len /= 2;
        }
    } else {
        *key = NULL;
        *key_len = 0;
    }
}

void validate_channel(uint8_t channel) {
    if (channel < 1 || channel > 11) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid WLAN channel"));
    }
}

void wlan_init0(void) {
    // to wlan init here
    init_event_callback_list();
}

rtw_result_t wlan_scan_handler(rtw_scan_handler_result_t *malloced_scan_result) {
    rtw_scan_result_t *record;
    if (malloced_scan_result->scan_complete != RTW_TRUE) {
        record = &malloced_scan_result->ap_details;
        record->SSID.val[record->SSID.len] = 0;
        if (ScanNetworkCounter < WLAN_MAX_SCAN_NETWORKS) {
            memcpy(ScanNetworkSsidCache[ScanNetworkCounter], record->SSID.val, record->SSID.len);
            memcpy(ScanNetworkBssidCache[ScanNetworkCounter], record->BSSID.octet, WLAN_MAC_LEN);
            ScanNetworkRssiCache[ScanNetworkCounter]     = record->signal_strength;
            ScanNetworkBssCache[ScanNetworkCounter]      = record->bss_type;
            ScanNetworkSecurityCache[ScanNetworkCounter] = record->security;
            ScanNetworkWpsCache[ScanNetworkCounter]      = record->wps_type;
            ScanNetworkChannelCache[ScanNetworkCounter]  = record->channel;
            ScanNetworkBandCache[ScanNetworkCounter]     = record->band;
            ScanNetworkCounter++;
        }
    }
}

STATIC mp_obj_t wlan_scan_network(mp_obj_t self_in) {
    STATIC const qstr wlan_scan_info_fields[] = {
        MP_QSTR_ssid, MP_QSTR_bssid, MP_QSTR_rssi, MP_QSTR_bss, MP_QSTR_security, MP_QSTR_wps, MP_QSTR_channel, MP_QSTR_band
    };
    mp_obj_t nets = mp_obj_new_list(0, NULL);
    uint8_t cnt = 0;
    
    ScanNetworkCounter = 0;

    wifi_scan_networks(wlan_scan_handler, NULL);

    for (cnt=0; cnt<ScanNetworkCounter; cnt++) {
        mp_obj_t tuple[8];
        tuple[0] = mp_obj_new_str((const char *)ScanNetworkSsidCache[cnt], strlen(ScanNetworkSsidCache[cnt]), false);
        tuple[1] = mp_obj_new_bytes((const char *)ScanNetworkBssidCache[cnt], WLAN_MAC_LEN);
        tuple[2] = mp_obj_new_int(ScanNetworkRssiCache[cnt]);
        tuple[3] = mp_obj_new_int(ScanNetworkBssCache[cnt]);
        tuple[4] = mp_obj_new_int(ScanNetworkSecurityCache[cnt]);
        tuple[5] = mp_obj_new_int(ScanNetworkWpsCache[cnt]);
        tuple[6] = mp_obj_new_int(ScanNetworkChannelCache[cnt]);
        tuple[7] = mp_obj_new_int(ScanNetworkBandCache[cnt]);
        mp_obj_list_append(nets, mp_obj_new_attrtuple(wlan_scan_info_fields, 8, tuple));
    }
    return nets;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wlan_scan_obj, wlan_scan_network);

STATIC mp_obj_t wlan_start_ap(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    STATIC const mp_arg_t allowed_args[] = {
        { MP_QSTR_ssid,     MP_ARG_KW_ONLY  | MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL} },
        { MP_QSTR_auth,     MP_ARG_KW_ONLY  | MP_ARG_OBJ, {.u_obj  = mp_const_none} },
    };
    int16_t ret = RTW_ERROR;
    wlan_obj_t *self = pos_args[0];

    if (self->mode != RTW_MODE_AP && self->mode != RTW_MODE_STA_AP) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Only AP / STA_AP mode can start ap"));
    }

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    int8_t *ssid = NULL;
    uint8_t ssid_len = 0;
    validate_ssid(&ssid, &ssid_len, args[0].u_obj);
    if (ssid != NULL) {
        memcpy(self->ssid, ssid, ssid_len);
    }

    int8_t   *key = NULL;
    uint32_t  security_type = 0;
    mp_uint_t key_len;
    validate_key((uint8_t *)&key, &key_len, &security_type, args[1].u_obj);
    if (key != NULL) {
        self->security_type = security_type;
        memcpy(self->key, key, key_len);
    }

#if 0 //TODO (Chester) remove xnetif
    dhcps_deinit();


    dhcps_init(&xnetif[self->netif]);
#endif
    
    if (is_promisc_enabled()) {
        promisc_set(0, NULL, 0);
    }
#if 0  //TODO(Chester) not used
    wifi_reg_event_handler(WIFI_EVENT_STA_ASSOC, wifi_ap_sta_assoc_hdl, NULL);
    wifi_reg_event_handler(WIFI_EVENT_STA_DISASSOC, wifi_ap_sta_disassoc_hdl, NULL);
#endif
    ret = wext_set_mode(self->ifname, IW_MODE_MASTER);
    if (ret != RTW_SUCCESS) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "%s set IW_MODE_MASTER failed", self->ifname));
    }
    ret = wext_set_channel(self->ifname, self->channel);
    if (ret != RTW_SUCCESS) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "%s set channel failed", self->ifname));
    }

    switch(self->security_type) {
        case RTW_SECURITY_OPEN:
            ret = RTW_SUCCESS;
            break;
        case RTW_SECURITY_WPA2_AES_PSK:
            ret = wext_set_auth_param(self->ifname, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_OPEN_SYSTEM);
            if (ret == 0) {
                ret = wext_set_key_ext(self->ifname, IW_ENCODE_ALG_CCMP, NULL, 0, 0, 0, 0, NULL, 0);
            }
            if (ret == 0) {
                ret = wext_set_passphrase(self->ifname, self->key, strlen(self->key));
            }
            break;
        default:
            ret = RTW_ERROR;
            break;
    }

    if (ret != RTW_SUCCESS) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "%s set passphrase failed", self->ifname));
    }
    ret = wext_set_ap_ssid(self->ifname, self->ssid, strlen(self->ssid));
    if (ret != RTW_SUCCESS) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "%s set ssid failed", self->ifname));
    }
    return mp_const_none;

}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(wlan_start_ap_obj, 0, wlan_start_ap);

STATIC mp_obj_t wlan_rssi(mp_obj_t self_in) {
    int16_t ret = RTW_ERROR;
    int16_t rssi = 0;
    wlan_obj_t *self = self_in;

    ret = wext_get_rssi(self->ifname, &rssi);

    if (ret == RTW_SUCCESS) {
        return mp_obj_new_int(rssi);
    } else {
        return mp_const_none;
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wlan_rssi_obj, wlan_rssi);

STATIC mp_obj_t wlan_mac(mp_uint_t n_args, const mp_obj_t *args) {
    wlan_obj_t *self = args[0];
    int8_t rtl_cmd[32];
    int8_t mac_str[17];
    uint8_t mac_str_len = 0;
    int16_t ret = RTW_ERROR;

    if (n_args == 1) {
        sprintf(rtl_cmd, "read_mac");
        ret = wext_private_command_with_retval(self->ifname, rtl_cmd, mac_str, sizeof(mac_str));
        if (ret == RTW_SUCCESS) {
            return mp_obj_new_str(mac_str, sizeof(mac_str), false);
        }
        else {
            return mp_const_none;
        }
    }
    else {
        //TODO(Chester) Should parse MAC string here
        int8_t *mac_ptr = mp_obj_str_get_data(args[1], &mac_str_len);

        // format is 00:00:00:00:00:00 = 6 * 2 + 5 = 17 chars
        if (mac_str_len > (WLAN_MAC_LEN * 2 + 5)) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Wrong MAC address length"));
        }
        sprintf(rtl_cmd, "write_mac %s", mac_ptr);
        ret = wext_private_command(self->ifname, rtl_cmd, 0);
        if (ret == RTW_SUCCESS) {
            return mp_const_none;
        } else {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Set MAC address error"));
        }
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(wlan_mac_obj, 1, 2, wlan_mac);

STATIC mp_obj_t wlan_rf(mp_obj_t self_in, mp_obj_t enable_in) {
    uint8_t ret = RTW_ERROR;
    uint16_t timeout = 0;
    wlan_obj_t *self = self_in;
    if (enable_in == mp_const_true) {
        rltk_wlan_rf_on();
    } else {
        rltk_wlan_rf_off();
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(wlan_rf_obj, wlan_rf);

STATIC mp_obj_t wlan_channel(mp_uint_t n_args, const mp_obj_t *args) {
    wlan_obj_t *self = args[0];
    int16_t ret = RTW_ERROR;
    uint8_t channel = 0;

    if (n_args == 1) {
        ret = wext_get_channel(self->index, &channel);
        if (ret == RTW_SUCCESS) {
            return mp_obj_new_int(channel);
        }
        else {
            return mp_const_none;
        }
    }
    else {
        channel = mp_obj_get_int(args[1]);
        validate_channel(channel);
        ret = wext_set_channel(self->index, channel);
        self->channel = channel;
        if (ret == RTW_SUCCESS) {
            return mp_const_none;
        } else {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Set channel error"));
        }
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(wlan_channel_obj, 1, 2, wlan_channel);

STATIC mp_obj_t wlan_connect(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    STATIC const mp_arg_t allowed_args[] = {
        { MP_QSTR_ssid,     MP_ARG_KW_ONLY  | MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL} },
        { MP_QSTR_auth,     MP_ARG_KW_ONLY  | MP_ARG_OBJ, {.u_obj  = mp_const_none} },
        { MP_QSTR_timeout,  MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int  = 5} },
        { MP_QSTR_dhcp,     MP_ARG_KW_ONLY  | MP_ARG_BOOL,{.u_bool = false} },
    };
    
    wlan_obj_t *self = pos_args[0];

    if (self->mode != RTW_MODE_STA) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Only STA mode can connect"));
    }

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    int8_t *ssid = NULL;
    uint8_t ssid_len = 0;
    validate_ssid(&ssid, &ssid_len, args[0].u_obj);
    if (ssid != NULL) {
        memcpy(self->ssid, ssid, ssid_len);
    }

    int8_t   *key = NULL;
    uint32_t  security_type = 0;
    mp_uint_t key_len;
    validate_key((uint8_t *)&key, &key_len, &security_type, args[1].u_obj);
    if (key != NULL) {
        self->security_type = security_type;
        memcpy(self->key, key, key_len);
    }

    // Override the key and ken_len in open security type
    if (security_type == RTW_SECURITY_OPEN) {
        key = NULL;
        key_len = 0;
    }

    int32_t timeout = -1;
    if (args[2].u_obj != mp_const_none) {
        timeout = mp_obj_get_int(args[2].u_obj);
    }
    
    int16_t ret = -1;
    ret = wifi_connect(self->ssid, self->security_type, self->key, strlen(self->ssid), strlen(self->key), 0, NULL);

    if (ret == RTW_SUCCESS) {
        if (args[3].u_bool == true) {
            if (dhcp_request_func(self->netif, timeout) == false)
                nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "%s connect to AP failed", self->ifname));
        }
        return mp_const_none;;
    }
    else {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "%s connect to AP failed", self->ifname));
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(wlan_connect_obj, 1, wlan_connect);

STATIC mp_obj_t wlan_disconnect(mp_obj_t self_in) {
    const uint8_t null_bssid[WLAN_MAC_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    uint8_t ret = RTW_ERROR;
    wlan_obj_t *self = self_in;

    ret = wext_set_bssid(self->index, null_bssid);

    if (ret != RTW_SUCCESS) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_OSError, "Disconnect %s failed", self->ifname));
    }
    
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wlan_disconnect_obj, wlan_disconnect);

STATIC mp_obj_t wlan_getnetif(mp_obj_t self_in) {
    wlan_obj_t *self = self_in;

    netif_obj_t *netif = m_new_obj(netif_obj_t);
    netif->base.type = &netif_type;
    netif->index = self->netif;
    
    return (mp_obj_t)netif;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wlan_getnetif_obj, wlan_getnetif);

STATIC mp_obj_t wlan_on(mp_obj_t self_in, mp_obj_t timeout_in) {
    uint8_t ret = RTW_ERROR;
    uint16_t timeout = 0;
    wlan_obj_t *self = self_in;

    if (MP_OBJ_IS_INT(timeout_in)) {
        timeout = mp_obj_new_int(timeout_in);
    }

    if (rltk_wlan_running(self->index)) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "%s is still running", self->ifname));
    }

    ret = rltk_wlan_init(self->index, self->mode);

    if (ret != RTW_SUCCESS) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "Init %s error", self->ifname));
    }

    rltk_wlan_start(self->index);

    for (;;) {
        if (rltk_wlan_running(self->index)) {
            break;
        }
        if (timeout == 0) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_TimeoutError, "Start wlan timeout"));
            break;
        }

        vTaskDelay(1 * configTICK_RATE_HZ);
        timeout --;
    }
    
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(wlan_on_obj, wlan_on);

STATIC mp_obj_t wlan_off(mp_obj_t self_in, mp_obj_t timeout_in) {
    uint8_t ret = RTW_ERROR;
    uint16_t timeout = 0;
    wlan_obj_t *self = self_in;

    if (rltk_wlan_running(self->index) == 0) {
        return mp_const_none;
    }

    rltk_wlan_deinit();

    for (;;) {
        if (rltk_wlan_running(self->index) == 0) {
            break;
        }
        if (timeout == 0) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_TimeoutError, "Stop wlan timeout"));
            break;
        }

        vTaskDelay(1 * configTICK_RATE_HZ);
        timeout --;
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(wlan_off_obj, wlan_off);

STATIC mp_obj_t wlan_is_connect_to_ap(mp_obj_t self_in) {
    uint8_t ret = RTW_ERROR;
    mp_obj_t result = mp_const_false;
    ret = rltk_wlan_is_connected_to_ap();

    if (ret == RTW_SUCCESS) {
        result = mp_const_true;
    } else {
        result = mp_const_false;;
    }

    return result;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wlan_is_connect_to_ap_obj, wlan_is_connect_to_ap);

STATIC void wlan_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    wlan_obj_t *self = self_in;
    qstr wlan_qstr;
    qstr security_qstr;
    switch (self->mode) {
        case RTW_MODE_AP:
            wlan_qstr = MP_QSTR_AP;
            break;
        case RTW_MODE_STA:
            wlan_qstr = MP_QSTR_STA;
            break;
        case RTW_MODE_STA_AP:
            wlan_qstr = MP_QSTR_STA_AP;
            break;
        case RTW_MODE_P2P:
            wlan_qstr = MP_QSTR_P2P;
            break;
        case RTW_MODE_PROMISC:
            wlan_qstr = MP_QSTR_PROMISC;
            break;
    }

    switch(self->security_type) {
        case RTW_SECURITY_OPEN:
            security_qstr = MP_QSTR_OPEN;
            break;
        case RTW_SECURITY_WEP_PSK:
            security_qstr = MP_QSTR_WEP_PSK;
            break;
        case RTW_SECURITY_WEP_SHARED:
            security_qstr = MP_QSTR_WEP_SHARED;
            break;
        case RTW_SECURITY_WPS_OPEN:
            security_qstr = MP_QSTR_WPS_OPEN;
            break;
        case RTW_SECURITY_WPS_SECURE:
            security_qstr = MP_QSTR_WPS_SECURE;
            break;
        case RTW_SECURITY_WPA_AES_PSK:
            security_qstr = MP_QSTR_WPA_AES_PSK;
            break;
        case RTW_SECURITY_WPA2_AES_PSK:
            security_qstr = MP_QSTR_WPA2_AES_PSK;
            break;
        case RTW_SECURITY_WPA_TKIP_PSK:
            security_qstr = MP_QSTR_WPA_TKIP_PSK;
            break;
        case RTW_SECURITY_WPA2_TKIP_PSK:
            security_qstr = MP_QSTR_WPA2_TKIP_PSK;
            break;
        case RTW_SECURITY_WPA2_MIXED_PSK:
            security_qstr = MP_QSTR_WPA2_MIXED_PSK;
            break;
        case RTW_SECURITY_WPA_WPA2_MIXED:
            security_qstr = MP_QSTR_WPA_WPA2_MIXED;
            break;
    }
    mp_printf(print, "%s(", self->ifname);
    mp_printf(print, "mode=%q", wlan_qstr);
    mp_printf(print, ", ssid=%s", self->ssid); 
    mp_printf(print, ", security_type=%q", security_qstr);
    mp_printf(print, ", channel=%d)", self->channel);
}

STATIC mp_obj_t wlan_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {
    STATIC const mp_arg_t wlan_init_args[] = {
        { MP_QSTR_mode,         MP_ARG_REQUIRED | MP_ARG_INT,  {.u_int = RTW_MODE_AP} },
    };

    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(wlan_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), wlan_init_args, args);

    wlan_obj_t *self;

    self = &wlan_obj_0;

    self->mode = args[0].u_int;
    validate_wlan_mode(self->mode);

    wlan_on((wlan_obj_t *)self, mp_obj_new_int(100));

    if (args[0].u_int == RTW_MODE_STA_AP) {
        // Prepare for the second wlan object
        mp_obj_t tuple[2];
        wlan_obj_t *self_2 = &wlan_obj_1;
        tuple[0] = self;
        tuple[1] = self_2;

        wlan_on((wlan_obj_t *)self_2, mp_obj_new_int(100));

        self->mode = RTW_MODE_STA;

        self->netif = 0;

        self_2->mode = RTW_MODE_AP;

        self_2->netif = 1;

        return mp_obj_new_tuple(2, tuple);
    } else {
        return (mp_obj_t)self;
    }
}

STATIC const mp_map_elem_t wlan_locals_dict_table[] = {
    // instance methods
    { MP_OBJ_NEW_QSTR(MP_QSTR_scan),                            (mp_obj_t)&wlan_scan_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_start_ap),                        (mp_obj_t)&wlan_start_ap_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_rssi),                            (mp_obj_t)&wlan_rssi_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_mac),                             (mp_obj_t)&wlan_mac_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_rf),                              (mp_obj_t)&wlan_rf_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_channel),                         (mp_obj_t)&wlan_channel_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_connect),                         (mp_obj_t)&wlan_connect_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_disconnect),                      (mp_obj_t)&wlan_disconnect_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_getnetif),                        (mp_obj_t)&wlan_getnetif_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_on),                              (mp_obj_t)&wlan_on_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_off),                             (mp_obj_t)&wlan_off_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_is_connect_to_ap),                (mp_obj_t)&wlan_is_connect_to_ap_obj },

    // class constants
    
    // WLAN mode 
    { MP_OBJ_NEW_QSTR(MP_QSTR_STA),                             MP_OBJ_NEW_SMALL_INT(RTW_MODE_STA) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_AP),                              MP_OBJ_NEW_SMALL_INT(RTW_MODE_AP) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_STA_AP),                          MP_OBJ_NEW_SMALL_INT(RTW_MODE_STA_AP) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PROMISC),                         MP_OBJ_NEW_SMALL_INT(RTW_MODE_PROMISC) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_P2P),                             MP_OBJ_NEW_SMALL_INT(RTW_MODE_P2P) },

    // SECURITY MODE
    { MP_OBJ_NEW_QSTR(MP_QSTR_OPEN),                            MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_OPEN) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WEP_PSK),                         MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WEP_PSK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WEP_SHARED),                      MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WEP_SHARED) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPA_TKIP_PSK),                    MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPA_TKIP_PSK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPA_AES_PSK),                     MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPA_AES_PSK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPA2_TKIP_PSK),                   MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPA2_TKIP_PSK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPA2_AES_PSK),                    MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPA2_AES_PSK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPA2_MIXED_PSK),                  MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPA2_MIXED_PSK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPA_WPA2_MIXED),                  MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPA_WPA2_MIXED) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPS_OPEN),                        MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPS_OPEN) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPS_SECURE),                      MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPS_SECURE) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_CONNECT),                   MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_CONNECT) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_DISCONNECT),                MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_DISCONNECT) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_FOURWAY_HANDSHAKE_DONE),    MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_FOURWAY_HANDSHAKE_DONE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_SCAN_RESULT_REPORT),        MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_SCAN_RESULT_REPORT) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_SCAN_DONE),                 MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_SCAN_DONE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_RECONNECTION_FAIL),         MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_RECONNECTION_FAIL) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_SEND_ACTION_DONE),          MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_SEND_ACTION_DONE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_RX_MGNT),                   MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_RX_MGNT) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_STA_ASSOC),                 MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_STA_ASSOC) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_STA_DISASSOC),              MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_STA_DISASSOC) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_WPS_FINISH),                MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_WPS_FINISH) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_EAPOL_RECVD),               MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_EAPOL_RECVD) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_NO_NETWORK),                MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_NO_NETWORK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_BEACON_AFTER_DHCP),         MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_BEACON_AFTER_DHCP) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_MAX),                       MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_MAX) },
};

STATIC MP_DEFINE_CONST_DICT(wlan_locals_dict, wlan_locals_dict_table);

const mp_obj_type_t wlan_type = {
    { &mp_type_type },
    .name        = MP_QSTR_WLAN,
    .print       = wlan_print,
    .make_new    = wlan_make_new,
    .locals_dict = (mp_obj_t)&wlan_locals_dict,
};

