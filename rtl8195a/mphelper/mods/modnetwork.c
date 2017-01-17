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
#include "modnetwork.h"
#include "objnetif.h"
#include "objpacket.h"

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/

/*****************************************************************************
 *                              Internal variables
 * ***************************************************************************/
STATIC sys_mbox_t mbox;
STATIC sys_mutex_t network_core_lock;
extern StackType_t mpNetworkCoreStack[];
#if 0
SECTION(".sdram.bss") 
#endif
StackType_t mpFTPDStack[MICROPY_NETWORK_CORE_STACK_DEPTH];
TaskHandle_t xFTPDHandle;

/*****************************************************************************
 *                              Local functions
 * ***************************************************************************/

STATIC void 
modnetwork_thread (void *arg) {
    struct network_msg *msg;
    sys_mutex_lock(&network_core_lock);
    while (1) {
        sys_mutex_unlock(&network_core_lock);
        // Fetch mailbox
        sys_timeouts_mbox_fetch(&mbox, (void **)&msg);
        sys_mutex_lock(&network_core_lock);
        switch (msg->type) {
            case NETWORK_MSG_INPKT:
                if (msg->msg.inp.netif->flags &
                        (NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET)) {
                    ethernet_input(msg->msg.inp.p, msg->msg.inp.netif);
                } else {
                    ip_input(msg->msg.inp.p, msg->msg.inp.netif);
                }
                memp_free(MEMP_TCPIP_MSG_INPKT, msg);
            break;
        }
    }
    sys_mutex_unlock(&network_core_lock);
    vTaskDelete(NULL);
}

err_t
modnetwork_input(struct pbuf *p, struct netif *inp) {
    struct network_msg *msg;

    if (!sys_mbox_valid(&mbox)) {
        return ERR_VAL;
    }

    msg = (struct network_msg *)memp_malloc(MEMP_TCPIP_MSG_INPKT);
    if (msg == NULL) {
        return ERR_MEM;
    }

    msg->type = NETWORK_MSG_INPKT;
    msg->msg.inp.p = p;
    msg->msg.inp.netif = inp;

    if (sys_mbox_trypost(&mbox, msg) != ERR_OK) {
        memp_free(MEMP_TCPIP_MSG_INPKT, msg);
        return ERR_MEM;
    }
    return ERR_OK;
}

void modnetwork_init(void) {
    // Init modnetwork here
    mp_obj_list_init(&MP_STATE_PORT(netif_list_obj), 0);
    
    // RTL8195A / RTL8711AM use lwip as network core
    lwip_init();

    if(sys_mbox_new(&mbox, TCPIP_MBOX_SIZE) != ERR_OK) {
        mp_printf(&mp_plat_print, "Create network core mailbox failed");
    }

    if (sys_mutex_new(&network_core_lock) != ERR_OK) {
        mp_printf(&mp_plat_print, "Create network core lock failed");
    }

    TaskHandle_t xReturn = xTaskGenericCreate(modnetwork_thread,
            (signed char *)MICROPY_NETWORK_CORE_STACK_NAME,
            MICROPY_NETWORK_CORE_STACK_DEPTH,
            NULL,
            MICROPY_NETWORK_CORE_STACK_PRIORITY,
            NULL,
            mpNetworkCoreStack,
            NULL);

     if (xReturn != pdTRUE)
        mp_printf(&mp_plat_print, "Create network core thread failed");

     netif_init0();
}

STATIC mp_obj_t netif_iflist(void) {
    return &MP_STATE_PORT(netif_list_obj);
}
MP_DEFINE_CONST_FUN_OBJ_0(netif_iflist_obj, netif_iflist);


void ftpd_task(void const *arg) {
    ftpd_init();
    for(;;);
}

mp_obj_t modnetwork_ftpd(mp_obj_t enable_in) {

    bool enable = mp_obj_is_true(enable_in);

    if (enable) {
    
        // Create MicroPython ftpd task
        xFTPDHandle = xTaskGenericCreate(ftpd_task,
                (signed char *)"FTPD", 
                MICROPY_NETWORK_CORE_STACK_DEPTH, 
                NULL,
                MICROPY_TASK_PRIORITY,
                NULL,           // No arguments to pass to mp thread
                mpFTPDStack,    // Use user define stack memory to make it predictable.
                NULL);

    } else {
        vTaskDelete(xFTPDHandle);
    }

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(modnetwork_ftpd_obj, modnetwork_ftpd);

STATIC const mp_map_elem_t mp_module_network_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__),     MP_OBJ_NEW_QSTR(MP_QSTR_network) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_ftpd),         MP_OBJ_FROM_PTR(&modnetwork_ftpd_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_iflist),       MP_OBJ_FROM_PTR(&netif_iflist_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_NETIF),        MP_OBJ_FROM_PTR(&netif_type) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PKT),          MP_OBJ_FROM_PTR(&packet_type) },
};
STATIC MP_DEFINE_CONST_DICT(mp_module_network_globals, mp_module_network_globals_table);

const mp_obj_module_t mp_network_module = {
    .base     = { &mp_type_module },
    .globals  = (mp_obj_dict_t*)&mp_module_network_globals,
};
