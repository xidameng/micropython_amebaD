#ifndef  _HRP_GAP_BLE_CMD_TABLE_H_
#define  _HRP_GAP_BLE_CMD_TABLE_H_

#include <string.h>
//#include <hrp_module_id.h>

typedef enum _HRP_GAP_BLE_CMD_GROUP_INDEX
{
    HRP_GAP_BLE_CMD_GROUP_ADV       = 0,
    HRP_GAP_BLE_CMD_GROUP_SCAN      = 1,
    HRP_GAP_BLE_CMD_GROUP_CONN      = 2,
    HRP_GAP_BLE_CMD_GROUP_PAIR      = 3

} HRP_GAP_BLE_CMD_GROUP_INDEX;

typedef enum _HRP_GAP_BLE_CMD_GROUP_ADV_INDEX
{
    HRP_GAP_BLE_CMD_ADV_0000_PERFOAMANCE_CONFIG_REQ = 0,
    HRP_GAP_BLE_CMD_ADV_0000_PERFOAMANCE_CONFIG_RSP,
    HRP_GAP_BLE_CMD_ADV_0000_PERFOAMANCE_START_REQ,
    HRP_GAP_BLE_CMD_ADV_0000_PERFOAMANCE_START_RSP,
    HRP_GAP_BLE_CMD_ADV_0000_PERFOAMANCE_CMPL_INFO,

    HRP_GAP_BLE_CMD_ADV_0001_STRESS_START_STOP_CONFIG_REQ,
    HRP_GAP_BLE_CMD_ADV_0001_STRESS_START_STOP_CONFIG_RSP,
    HRP_GAP_BLE_CMD_ADV_0001_STRESS_START_STOP_START_REQ,
    HRP_GAP_BLE_CMD_ADV_0001_STRESS_START_STOP_START_RSP,
    HRP_GAP_BLE_CMD_ADV_0001_STRESS_START_STOP_CMPL_INFO

} HRP_GAP_BLE_CMD_GROUP_ADV_INDEX;


typedef enum _HRP_GAP_BLE_CMD_GROUP_SCAN_INDEX
{
    HRP_GAP_BLE_CMD_SCAN_0000_PERFOAMANCE_CONFIG_REQ = 0,
    HRP_GAP_BLE_CMD_SCAN_0000_PERFOAMANCE_CONFIG_RSP,
    HRP_GAP_BLE_CMD_SCAN_0000_PERFOAMANCE_START_REQ,
    HRP_GAP_BLE_CMD_SCAN_0000_PERFOAMANCE_START_RSP,
    HRP_GAP_BLE_CMD_SCAN_0000_PERFOAMANCE_CMPL_INFO,

    HRP_GAP_BLE_CMD_SCAN_0001_STRESS_START_STOP_CONFIG_REQ,
    HRP_GAP_BLE_CMD_SCAN_0001_STRESS_START_STOP_CONFIG_RSP,
    HRP_GAP_BLE_CMD_SCAN_0001_STRESS_START_STOP_START_REQ,
    HRP_GAP_BLE_CMD_SCAN_0001_STRESS_START_STOP_START_RSP,
    HRP_GAP_BLE_CMD_SCAN_0001_STRESS_START_STOP_CMPL_INFO,

} HRP_GAP_BLE_CMD_GROUP_SCAN_INDEX;

//add more here


void hrp_gap_ble_handle_req(uint8_t cmd_group, uint8_t cmd_index, uint16_t len,
                            uint8_t *p_param_list);


#endif
