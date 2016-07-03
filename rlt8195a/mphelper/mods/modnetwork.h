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

#ifndef MODNETWORK_H_
#define MODNETWORK_H_
#include "py/runtime.h"

#include "exception.h"

#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/init.h"
#include "lwip/lwip_timers.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include "ethernetif.h"

#include "lwip/ip_addr.h"

#include "lwip_netconf.h"

#include "wifi_ind.h"

#include "lwip/dhcp.h"

#define NETIF_STA_ID                0
#define NETIF_AP_ID                 1

#define STATIC_IP                   0 
#define DHCP_IP                     1

#define DEFAULT_IP_ADDR0            192
#define DEFAULT_IP_ADDR1            168
#define DEFAULT_IP_ADDR2            3
#define DEFAULT_IP_ADDR3            2

#define DEFAULT_NETMASK_ADDR0       255
#define DEFAULT_NETMASK_ADDR1       255
#define DEFAULT_NETMASK_ADDR2       255
#define DEFAULT_NETMASK_ADDR3       0

#define DEFAULT_GW_ADDR0            192
#define DEFAULT_GW_ADDR1            168
#define DEFAULT_GW_ADDR2            3
#define DEFAULT_GW_ADDR3            1

#define DEFAULT_AP_IP_ADDR0         192
#define DEFAULT_AP_IP_ADDR1         168
#define DEFAULT_AP_IP_ADDR2         4
#define DEFAULT_AP_IP_ADDR3         1

#define DEFAULT_AP_NETMASK_ADDR0    255
#define DEFAULT_AP_NETMASK_ADDR1    255
#define DEFAULT_AP_NETMASK_ADDR2    255
#define DEFAULT_AP_NETMASK_ADDR3    0

#define DEFAULT_AP_GW_ADDR0         192
#define DEFAULT_AP_GW_ADDR1         168
#define DEFAULT_AP_GW_ADDR2         4
#define DEFAULT_AP_GW_ADDR3         1

#endif
