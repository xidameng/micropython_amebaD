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
#ifndef OBJWLAN_H_
#define OBJWLAN_H_

#include "py/runtime.h"

#include "exception.h"

#include "wifi_conf.h"

#define WLAN_INTERFACE_NUMBER   2
#define WLAN_MIN_SSID_LEN       3
#define WLAN_MAX_SSID_LEN       32
#define WLAN_MAC_LEN            6
#define WLAN_KEY_LEN            65
#define WLAN_MAX_SCAN_NETWORKS  50

extern const mp_obj_type_t wlan_type;

void wlan_init0(void);

typedef struct {
    mp_obj_base_t  base;
    uint8_t        idx;                       // Reserved for RTl8195A cocurrency mode
    uint8_t        mode;                      // Wlan mode, STA / AP / MIX
    int32_t        security_type;             // Wlan security mode, WPA2 / WEP ...
    uint8_t        channel;                   // Wlan channel 0 ~ 11
    int8_t         mac[WLAN_MAC_LEN];         // Wlan mac address
    int8_t         ssid[WLAN_MAX_SSID_LEN+1]; // Wlan ssid in *AP mode*
    int8_t         bssid[WLAN_MAC_LEN];       // Wlan bssid in *AP mode*
    int8_t         key[WLAN_KEY_LEN];         // Wlan key in *AP mode*
} wlan_obj_t;

#endif  // OBJWLAN_H_
