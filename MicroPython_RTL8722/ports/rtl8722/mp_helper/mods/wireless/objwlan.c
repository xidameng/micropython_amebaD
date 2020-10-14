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

// MP include
#include "objwlan.h"

// RTK includes 
#include "main.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#include "lwip_netconf.h"
#include "lwip/err.h"
#include "lwip/api.h"
#include <dhcp/dhcps.h>


extern struct netif xnetif[NET_IF_NUM]; 



/*****************************************************************************
 *                              Local variables
 * ***************************************************************************/
static rtw_network_info_t wifi;
static rtw_ap_info_t ap;
static unsigned char pswd[65] = {0};
const unsigned char *ssid = NULL;

//static const unsigned char *pswd = NULL;


STATIC wlan_obj_t wlan_obj = {
    .base.type      = &wlan_type,
    .security_type  = RTW_SECURITY_WPA2_AES_PSK,
};


STATIC xSemaphoreHandle xSTAConnectAPSema;
rtw_mode_t wifi_mode = 0;


static bool init_wlan = false;
static char connect_result = WL_FAILURE;



//mp_obj_base_t base;

// settings of requested network
uint8_t  _networkCount;
char     _networkSsid[WL_NETWORKS_LIST_MAXNUM][WL_SSID_MAX_LENGTH];
int32_t  _networkRssi[WL_NETWORKS_LIST_MAXNUM];
uint32_t _networkEncr[WL_NETWORKS_LIST_MAXNUM];



/*****************************************************************************
 *                              External function
 * ***************************************************************************/

void wlan_init0(void) {
    // to wlan init here
    //init_event_callback_list();
    struct netif * pnetif = &xnetif[0];

    if (init_wlan == false) {
        init_wlan = true;
        LwIP_Init();
        wifi_on(RTW_MODE_STA);
        wifi_mode = RTW_MODE_STA;
    } else if (init_wlan == true) {
        if (wifi_mode != RTW_MODE_STA) {
            dhcps_deinit();
            wifi_off();
            vTaskDelay(20);
            wifi_on(RTW_MODE_STA);
            dhcps_init(pnetif);
            wifi_mode = RTW_MODE_STA;
        }
    }
}




/*****************************************************************************
 *                              Local functions
 * ***************************************************************************/

static uint8_t getConnectionStatus() {

    wlan_init0();

    if (wifi_is_connected_to_ap() == 0) {
        return WL_CONNECTED;
    } else {
        return WL_DISCONNECTED;
    }
}


static void init_wifi_struct(void) {

    memset(wifi.ssid.val, 0, sizeof(wifi.ssid.val));
    memset(wifi.bssid.octet, 0, ETH_ALEN);
    memset(pswd, 0, sizeof(pswd));
    wifi.ssid.len = 0;
    wifi.password = NULL;
    wifi.password_len = 0;
    wifi.key_id = -1;
    memset(ap.ssid.val, 0, sizeof(ap.ssid.val));
    ap.ssid.len = 0;
    ap.password = NULL;
    ap.password_len = 0;
    ap.channel = 1;
}


void validate_wlan_mode(uint8_t mode) {
    if (mode != RTW_MODE_AP &&
        mode != RTW_MODE_STA &&
        mode != RTW_MODE_STA_AP 
        ) {
            mp_raise_ValueError("Invalid WLAN mode");
    }
}

void validate_ssid(mp_uint_t len) {

    if ((len > WLAN_MAX_SSID_LEN) || (len < WLAN_MIN_SSID_LEN)) {
        mp_raise_ValueError("Invalid SSID length");
    }

}

void validate_pswd(mp_uint_t len) {

    if (len > WLAN_WPA_MAC_PASS_CHAR_LEN ||len < WLAN_WPA_MIN_PASS_CHAR_LEN) {
        mp_raise_ValueError("Invalid password length");
    }
}


STATIC const qstr wlan_scan_info_fields[] = {
        MP_QSTR_ssid, MP_QSTR_bssid, MP_QSTR_rssi,
        MP_QSTR_bss, MP_QSTR_security, MP_QSTR_wps,
        MP_QSTR_channel, MP_QSTR_band
};


rtw_result_t wifidrv_scan_result_handler( rtw_scan_handler_result_t* malloced_scan_result)
{   
    rtw_scan_result_t* record;

    if (malloced_scan_result->scan_complete != RTW_TRUE) {
        record = &malloced_scan_result->ap_details;
        record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */

        if (_networkCount < WL_NETWORKS_LIST_MAXNUM) {
            strcpy( _networkSsid[_networkCount], (char *)record->SSID.val);
            _networkRssi[_networkCount] = record->signal_strength;
            _networkEncr[_networkCount] = record->security;
            _networkCount++;
        }
    }

    return RTW_SUCCESS;
}

static void printEncryptionType(uint32_t thisType) {

    switch (thisType) {
        case SECURITY_OPEN:
            printf("Open\n");
            break;
        case SECURITY_WEP_PSK:
            printf("WEP\n");
            break;
        case SECURITY_WPA_TKIP_PSK:
            printf("WPA TKIP\n");
            break;
        case SECURITY_WPA_AES_PSK:
            printf("WPA AES\n");
            break;
        case SECURITY_WPA2_AES_PSK:
            printf("WPA2 AES\n");
            break;
        case SECURITY_WPA2_TKIP_PSK:
            printf("WPA2 TKIP\n");
            break;
        case SECURITY_WPA2_MIXED_PSK:
            printf("WPA2 Mixed\n");
            break;
        case SECURITY_WPA_WPA2_MIXED:
            printf("WPA/WPA2 AES\n");
            break;
    }
}

static int8_t startScanNetworks() {
    _networkCount = 0;
    if (wifi_scan_networks(wifidrv_scan_result_handler, NULL) != RTW_SUCCESS) {
        return WL_FAILURE;
    }
    return WL_SUCCESS;
}


//STATIC mp_obj_t wlan_scan(mp_obj_t self_in) {
STATIC mp_obj_t wlan_scan() {
    uint8_t attempts = 10;
    uint8_t numOfNetworks = 0;
    uint8_t conStatus = WL_DISCONNECTED;

    conStatus = getConnectionStatus();
    printf("%d\n", conStatus);

    if(conStatus != WL_NO_SHIELD) {

        if (startScanNetworks() == WL_FAILURE) {
            printf("\n## WiFi scan network returned FAIL\n");
            mp_raise_ValueError("Scan error!");
            return mp_const_none;
        }

        do {
             mp_hal_delay_ms(2000);
             numOfNetworks = _networkCount;
        } while ((numOfNetworks == 0) && (--attempts > 0));

    } else {
        printf("WiFi shield not present\n");
        mp_raise_ValueError("Scan error!");
    }


    if (numOfNetworks == -1) { //if scan fail
        printf("Could not find available wifi\n");
    } else { // if scan success
        printf("The number of networks found: ");
        printf("%d\n", numOfNetworks);
//int thisNet = 0;
        for (int thisNet = 0; thisNet < numOfNetworks; thisNet++) {
            printf("%d", thisNet);
            printf(") ");
            printf("%s", _networkSsid[thisNet]);
            printf("\tSignal: ");
            printf("%d", _networkRssi[thisNet]);;
            printf(" dBm");
            printf("\tEncryption: ");
            printEncryptionType(_networkEncr[thisNet]);
        }
    }
    
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(wlan_scan_obj, wlan_scan);

#if 0
STATIC mp_obj_t wlan_start_ap(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_ssid, ARG_auth};
    STATIC const mp_arg_t allowed_args[] = {
        { MP_QSTR_ssid, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_auth, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };
    int16_t ret = RTW_ERROR;

    //wlan_obj_t *self = pos_args[0];

    if (self->mode != RTW_MODE_AP && self->mode != RTW_MODE_STA_AP) {
        mp_raise_ValueError("Only AP and STA_AP can start ap");
    }

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    int8_t *ssid = NULL;
    mp_uint_t ssid_len = 0;

    ssid = mp_obj_str_get_data(args[ARG_ssid].u_obj, &ssid_len);

    validate_ssid(ssid_len);

    int8_t   *key = NULL;
    uint32_t  security_type = 0;
    mp_uint_t key_len;

    validate_key((uint8_t *)&key, &key_len, &security_type, args[ARG_auth].u_obj);

    if (key != NULL) {
        self->security_type = security_type;
    }

    if (is_promisc_enabled()) {
        promisc_set(0, NULL, 0);
    }

    const char *ifname = WLAN0_NAME;

    if (self->mode == RTW_MODE_STA_AP)
        ifname = WLAN1_NAME;

    ret = wext_set_mode(ifname, IW_MODE_MASTER);

    if (ret != RTW_SUCCESS) {
        mp_raise_OSError("[WLAN] Set master mode error");
    }
    ret = wext_set_channel(ifname, self->channel);

    if (ret != RTW_SUCCESS) {
        mp_raise_OSError("[WLAN] Set channel error");
    }

    switch (self->security_type) {
        case RTW_SECURITY_OPEN:
            ret = RTW_SUCCESS;
            break;
        case RTW_SECURITY_WPA2_AES_PSK:
            ret = wext_set_auth_param(ifname, IW_AUTH_80211_AUTH_ALG, IW_AUTH_ALG_OPEN_SYSTEM);
            if (ret == RTW_SUCCESS)
                ret = wext_set_key_ext(ifname, IW_ENCODE_ALG_CCMP, NULL, 0, 0, 0, 0, NULL, 0);

            if (ret == RTW_SUCCESS) 
                ret = wext_set_passphrase(ifname, key, key_len);
            break;
        default:
            ret = RTW_ERROR;
            break;
    }

    if (ret != RTW_SUCCESS)
        mp_raise_OSError("[WLAN] Set passphrase error");

    ret = wext_set_ap_ssid(ifname, ssid, ssid_len);

    if (ret != RTW_SUCCESS) 
      mp_raise_OSError("[WLAN] Set ssid error");

    dhcps_deinit();

    if (self->mode == RTW_MODE_AP) {
        dhcps_init(&xnetif[0]);
    } else if(self->mode == RTW_MODE_STA_AP) {
        dhcps_init(&xnetif[1]);
    } else {
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(wlan_start_ap_obj, 0, wlan_start_ap);
#endif

STATIC mp_obj_t wlan_connect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    
    enum { ARG_ssid, ARG_pswd };
    
    STATIC const mp_arg_t allowed_args[] = {
        { MP_QSTR_ssid,  MP_ARG_REQUIRED | MP_ARG_OBJ  ,{.u_obj = mp_const_none}},
        { MP_QSTR_pswd,  MP_ARG_REQUIRED | MP_ARG_OBJ  ,{.u_obj = mp_const_none}},
    };
    
    //wlan_obj_t *self = pos_args[0];

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    uint8_t status = WL_IDLE_STATUS;
    unsigned char ssid_len = 0;
    int pswd_len = 0;
    int ret = RTW_ERROR;
    uint8_t dhcp_result;
    
    // init wifi drivers
    wlan_init0();

    if (args[ARG_ssid].u_obj != mp_const_none) {
        ssid = mp_obj_str_get_data(args[ARG_ssid].u_obj, &ssid_len);
        validate_ssid(ssid_len);
        memset(wifi.bssid.octet, 0, ETH_ALEN);
        memcpy(wifi.ssid.val, ssid, ssid_len);
        wifi.ssid.len = ssid_len;
    }

    wifi.security_type = RTW_SECURITY_WPA2_AES_PSK;
    char* xmpassword = NULL;
    if (args[ARG_pswd].u_obj != mp_const_none) {
        memset(pswd, 0, sizeof(pswd));
        xmpassword = mp_obj_str_get_data(args[ARG_pswd].u_obj, &pswd_len);
        memcpy(pswd, xmpassword, pswd_len);
        validate_pswd(pswd_len);
        wifi.password = pswd;
        wifi.password_len = pswd_len;
        wifi.key_id = 0;
    }

    ret = wifi_connect((char*)wifi.ssid.val, wifi.security_type, (char*)wifi.password, wifi.ssid.len, wifi.password_len, wifi.key_id, NULL);

#if 0
    switch(security_type) {
        case RTW_SECURITY_OPEN:
            ret = wext_set_key_ext(WLAN0_NAME, IW_ENCODE_ALG_NONE, NULL, 0, 0, 0, 0, NULL, 0);
            break;
        case RTW_SECURITY_WEP_PSK:
        case RTW_SECURITY_WEP_SHARED:
            ret = wext_set_auth_param(WLAN0_NAME, IW_AUTH_80211_AUTH_ALG,
                    IW_AUTH_ALG_SHARED_KEY);
            break;
        case RTW_SECURITY_WPA_TKIP_PSK:
        case RTW_SECURITY_WPA2_TKIP_PSK:
            ret = wext_set_auth_param(WLAN0_NAME, IW_AUTH_80211_AUTH_ALG,
                    IW_AUTH_ALG_OPEN_SYSTEM);
            if (ret == RTW_SUCCESS) 
                ret = wext_set_key_ext(WLAN0_NAME, IW_ENCODE_ALG_TKIP, NULL,
                        0, 0, 0, 0, NULL, 0);
            if (ret == RTW_SUCCESS) 
                ret = wext_set_passphrase(WLAN0_NAME, key, key_len);
            break;
        case RTW_SECURITY_WPA_AES_PSK:
        case RTW_SECURITY_WPA2_AES_PSK:
        case RTW_SECURITY_WPA2_MIXED_PSK:
        case RTW_SECURITY_WPA_WPA2_MIXED:
            ret = wext_set_auth_param(WLAN0_NAME, IW_AUTH_80211_AUTH_ALG,
                    IW_AUTH_ALG_OPEN_SYSTEM);
            if (ret == RTW_SUCCESS) 
                ret = wext_set_key_ext(WLAN0_NAME, IW_ENCODE_ALG_CCMP, NULL,
                        0, 0, 0, 0, NULL, 0);
            if (ret == RTW_SUCCESS)
                ret = wext_set_passphrase(WLAN0_NAME, key, key_len);
            break;
        default:
            ret = RTW_ERROR;
            break;
    }
#endif

    if (ret == RTW_SUCCESS) {

        dhcp_result = LwIP_DHCP(0, DHCP_START);

        init_wifi_struct();

        if ( dhcp_result == DHCP_ADDRESS_ASSIGNED ) {
            connect_result = WL_SUCCESS;
        } else {
            wifi_disconnect();
            connect_result = WL_FAILURE;
        }
    } else {
        init_wifi_struct();
        connect_result = WL_FAILURE;
    }

    uint8_t wifi_status = WL_IDLE_STATUS;

    if (connect_result != WL_FAILURE) {
        wifi_status = getConnectionStatus();
    } else {
        wifi_status= WL_CONNECT_FAILED;
    }

    if (wifi_status == WL_CONNECTED) {
        printf("WiFi is connected to %s\n", ssid);
    } else {
        mp_raise_OSError("WiFi connect failed.");
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(wlan_connect_obj, 1, wlan_connect);

#if 0
STATIC mp_obj_t wlan_disconnect(mp_obj_t self_in) {
    const uint8_t null_bssid[ETH_ALEN + 2] = {0x00, 0x00, 0x00, 0x00,
        0x00, 0x01, 0x00, 0x00};

    //wlan_obj_t *self = self_in;

    if (self->mode != RTW_MODE_STA)
        mp_raise_OSError("Disconnect only support STA mode");

    int16_t ret = wext_set_bssid(WLAN0_NAME, null_bssid);

    if (ret != RTW_SUCCESS) {
        mp_raise_msg(&mp_type_OSError, "Disconnect error");
    }
    
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wlan_disconnect_obj, wlan_disconnect);

STATIC mp_obj_t wlan_on(mp_uint_t n_args, const mp_obj_t *args) {
    int16_t ret = RTW_ERROR;

    //wlan_obj_t *self = args[0];

    if (rltk_wlan_running(WLAN0_IDX)) {
        mp_raise_msg(&mp_type_OSError, "WLAN has already running, please turn it off first.");
    }

    int mode = self->mode;

    if (n_args > 1) {
        mode = mp_obj_get_int(args[1]);
        self->mode = mode;
        wifi_mode = mode;
    }

    // Init WLAN0 first
    ret = rltk_wlan_init(WLAN0_IDX, self->mode);

    // Init WLAN1 second if mode is RTW_MODE_STA_AP
    if (self->mode == RTW_MODE_STA_AP) {
        ret = rltk_wlan_init(WLAN1_IDX, self->mode);
    }

    if (ret != RTW_SUCCESS) {
        mp_raise_msg(&mp_type_OSError, "WLAN init error");
    }

    rltk_wlan_start(WLAN0_IDX);

    if (self->mode == RTW_MODE_STA_AP) {
        rltk_wlan_start(WLAN1_IDX);
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(wlan_on_obj, 1, 2, wlan_on);

STATIC mp_obj_t wlan_off(mp_obj_t self_in) {

    //wlan_obj_t *self = self_in;

    rltk_wlan_deinit();

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wlan_off_obj, wlan_off);

STATIC mp_obj_t wlan_wifi_is_running(mp_obj_t self_in, mp_obj_t idx_in) {

    mp_uint_t index = mp_obj_get_int(idx_in);

    if (index > 1) 
        mp_raise_ValueError("Invalid WLAN index");

    //wlan_obj_t *self = self_in;

    if (rltk_wlan_running(index))
        return mp_const_true;
    
    return mp_const_false;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(wlan_wifi_is_running_obj, wlan_wifi_is_running);



STATIC mp_obj_t wlan_is_connect_to_ap(mp_obj_t self_in) {

    //wlan_obj_t *self = self_in;

    int16_t ret = rltk_wlan_is_connected_to_ap();

    if (ret != RTW_SUCCESS)
        return mp_const_false;

    return mp_const_true;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wlan_is_connect_to_ap_obj, wlan_is_connect_to_ap);

void wifi_event_scan_result_report_hdl (char *buf, int buf_len, int flags,
        void *userfunc) {

    mp_obj_t func = MP_OBJ_FROM_PTR(userfunc);

    if (func != mp_const_none) {
        nlr_buf_t nlr;
        if (nlr_push(&nlr) == 0) {

            //TODO Should gc_lock ?
            
            /*
             *  Make a new tuple and send it to callback function, I decide not to do
             *  gc_lock because we need to pass new data to outside of callback
             */
            rtw_scan_result_t** ptr = (rtw_scan_result_t**) buf;

            mp_obj_t tuple[8];
            mp_obj_t attrtuple;

            tuple[0] = mp_obj_new_str((*ptr)->SSID.val, (*ptr)->SSID.len);
            tuple[1] = mp_obj_new_bytes((*ptr)->BSSID.octet, ETH_ALEN);
            tuple[2] = mp_obj_new_int((*ptr)->signal_strength);
            tuple[3] = mp_obj_new_int((*ptr)->bss_type);
            tuple[4] = mp_obj_new_int((*ptr)->security);
            tuple[5] = mp_obj_new_int((*ptr)->wps_type);
            tuple[6] = mp_obj_new_int((*ptr)->channel);
            tuple[7] = mp_obj_new_int((*ptr)->band);

            attrtuple = mp_obj_new_attrtuple(wlan_scan_info_fields, 8, tuple);

            mp_call_function_1(func, attrtuple);

            nlr_pop();
        } else {
            mp_printf(&mp_plat_print, "Uncaught exception in callback handler");
            if (nlr.ret_val != MP_OBJ_NULL) {
                mp_obj_print_exception(&mp_plat_print, nlr.ret_val);
            }
        }
    }
}

#endif

STATIC void wlan_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    //wlan_obj_t *self = self_in;
    qstr wlan_qstr;
    qstr security_qstr;
    switch (wifi_mode) {
        case RTW_MODE_AP:
            wlan_qstr = MP_QSTR_AP;
            break;
        case RTW_MODE_STA:
            wlan_qstr = MP_QSTR_STA;
            break;
        case RTW_MODE_STA_AP:
            wlan_qstr = MP_QSTR_STA_AP;
            break;
    }

    switch(wifi.security_type) {
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
    mp_printf(print, "mode=%q", wlan_qstr);
    mp_printf(print, ", ssid=%s", ssid); 
    mp_printf(print, ", security_type=%q", security_qstr);
}

STATIC mp_obj_t wlan_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {
    enum {ARG_mode};
    STATIC const mp_arg_t wlan_init_args[] = {
        { MP_QSTR_mode,   MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(wlan_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args),
            wlan_init_args, args);

    wlan_obj_t *self = &wlan_obj;

    self->mode = args[ARG_mode].u_int;
    wifi_mode = args[ARG_mode].u_int;

    validate_wlan_mode(wifi_mode);

    //memset(self->ssid, 0, sizeof(self->ssid));

    //wifi_mode = self->mode;

    return (mp_obj_t)self;
}

STATIC const mp_map_elem_t wlan_locals_dict_table[] = {
    // instance methods

    { MP_OBJ_NEW_QSTR(MP_QSTR_connect),          MP_OBJ_FROM_PTR(&wlan_connect_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_scan),             MP_OBJ_FROM_PTR(&wlan_scan_obj) },
    /*
    { MP_OBJ_NEW_QSTR(MP_QSTR_start_ap),         MP_OBJ_FROM_PTR(&wlan_start_ap_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_disconnect),       MP_OBJ_FROM_PTR(&wlan_disconnect_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_on),               MP_OBJ_FROM_PTR(&wlan_on_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_off),              MP_OBJ_FROM_PTR(&wlan_off_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_wifi_is_running),  MP_OBJ_FROM_PTR(&wlan_wifi_is_running_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_is_connect_to_ap), MP_OBJ_FROM_PTR(&wlan_is_connect_to_ap_obj) },
    */
    // class constants
    { MP_OBJ_NEW_QSTR(MP_QSTR_CONNECTED),        MP_OBJ_NEW_SMALL_INT(WL_CONNECTED) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_DISCONNECTED),     MP_OBJ_NEW_SMALL_INT(WL_DISCONNECTED) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_IDLE),             MP_OBJ_NEW_SMALL_INT(WL_IDLE_STATUS) },

    // WLAN mode 
    { MP_OBJ_NEW_QSTR(MP_QSTR_STA),              MP_OBJ_NEW_SMALL_INT(RTW_MODE_STA) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_AP),               MP_OBJ_NEW_SMALL_INT(RTW_MODE_AP) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_STA_AP),           MP_OBJ_NEW_SMALL_INT(RTW_MODE_STA_AP) },

    // SECURITY MODE
    { MP_OBJ_NEW_QSTR(MP_QSTR_OPEN),             MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_OPEN) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WEP_PSK),          MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WEP_PSK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WEP_SHARED),       MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WEP_SHARED) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPA_TKIP_PSK),     MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPA_TKIP_PSK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPA_AES_PSK),      MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPA_AES_PSK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPA2_TKIP_PSK),    MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPA2_TKIP_PSK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPA2_AES_PSK),     MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPA2_AES_PSK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPA2_MIXED_PSK),   MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPA2_MIXED_PSK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_WPA_WPA2_MIXED),   MP_OBJ_NEW_SMALL_INT(RTW_SECURITY_WPA_WPA2_MIXED) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_CONNECT),                 MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_CONNECT) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_DISCONNECT),              MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_DISCONNECT) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_FOURWAY_HANDSHAKE_DONE),  MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_FOURWAY_HANDSHAKE_DONE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_SCAN_RESULT_REPORT),      MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_SCAN_RESULT_REPORT) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_SCAN_DONE),               MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_SCAN_DONE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_RECONNECTION_FAIL),       MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_RECONNECTION_FAIL) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_SEND_ACTION_DONE),        MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_SEND_ACTION_DONE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_RX_MGNT),                 MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_RX_MGNT) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_STA_ASSOC),               MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_STA_ASSOC) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_STA_DISASSOC),            MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_STA_DISASSOC) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_WPS_FINISH),              MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_WPS_FINISH) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_EAPOL_RECVD),             MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_EAPOL_RECVD) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_NO_NETWORK),              MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_NO_NETWORK) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EVENT_BEACON_AFTER_DHCP),       MP_OBJ_NEW_SMALL_INT(WIFI_EVENT_BEACON_AFTER_DHCP) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_SCAN_TYPE_ACTIVE),              MP_OBJ_NEW_SMALL_INT(RTW_SCAN_TYPE_ACTIVE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SCAN_TYPE_PASSIVE),             MP_OBJ_NEW_SMALL_INT(RTW_SCAN_TYPE_PASSIVE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SCAN_TYPE_PROHIBITED_CHANNELS), MP_OBJ_NEW_SMALL_INT(RTW_SCAN_TYPE_PROHIBITED_CHANNELS) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_BSS_TYPE_INFRASTRUCTURE),       MP_OBJ_NEW_SMALL_INT(RTW_BSS_TYPE_INFRASTRUCTURE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_BSS_TYPE_ADHOC),                MP_OBJ_NEW_SMALL_INT(RTW_BSS_TYPE_ADHOC) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_BSS_TYPE_ANY),                  MP_OBJ_NEW_SMALL_INT(RTW_BSS_TYPE_ANY) },
};
STATIC MP_DEFINE_CONST_DICT(wlan_locals_dict, wlan_locals_dict_table);

const mp_obj_type_t wlan_type = {
    { &mp_type_type },
    .name        = MP_QSTR_WLAN,
    .print       = wlan_print,
    .make_new    = wlan_make_new,
    .locals_dict = (mp_obj_t)&wlan_locals_dict,
};
