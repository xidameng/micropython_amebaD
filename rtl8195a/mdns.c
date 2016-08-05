#include "py/obj.h"
#include "cmsis_os.h"
#include "osdep_api.h"
#include <mDNS/mDNS.h>

osThreadId mdns_tid;

void mdns_task(void *arg) {
    DNSServiceRef dnsServiceRef = NULL;
    TXTRecordRef txtRecord;
    unsigned char txt_buf[100];
    if (mDNSResponderInit() == 0) {
        TXTRecordCreate(&txtRecord, sizeof(txt_buf), txt_buf);
        TXTRecordSetValue(&txtRecord, "text1", strlen("text1_content"), "text1_content");
        TXTRecordSetValue(&txtRecord, "text2", strlen("text2_content"), "text2_content");
        dnsServiceRef = mDNSRegisterService("ameba", "_service1._tcp", "local", 5000, &txtRecord);
        TXTRecordDeallocate(&txtRecord);
        vTaskDelay(30*1000);
        DiagPrintf("wait for 30s ... \n");
        if (dnsServiceRef)
            mDNSDeregisterService(dnsServiceRef);
        mDNSResponderDeinit();
        DiagPrintf("mdns deinit\r\n");
    } else {
        DiagPrintf("init failed\r\n");
    }
    vTaskDelete(NULL);
}

void mdns_init0(void) {
    osThreadDef(mdns_task, osPriorityBelowNormal, 1, 1024);
    mdns_tid = osThreadCreate(osThread(mdns_task), NULL);
}

mp_obj_t mp_builtin_mdns(mp_obj_t enable_in) {
    bool enable = mp_obj_is_true(enable_in);
    mdns_init0();
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_builtin_mdns_obj, mp_builtin_mdns);
