/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file tcu_mng_le.h
*
* @brief Contains definitions and function declarations for the low-energy management (MNG_LE) part of the TCU API.
*
* All functions may return the following, common return codes.
*
* @return    TCU_FUNC_SUCCESS Completed succesfully
* @return    TCU_FUNC_OPCODE Op-code missmatch
* @return    TCU_FUNC_SERVICEID Sservice ID mismatch
*/
#ifndef __TCU_MNG_LE_H__
#define __TCU_MNG_LE_H__

#include "tcu_api.h"

/*! TCU_LE_ACCEPT status enumeration */
enum tcu_mng_le_accept_status {
  LE_SUCCESS = 0x00,         /*!< Success */
  LE_PARAM_ERR = 0x01,       /*!< The abnormalities in a parameter */
  LE_NOT_INIT_ERR = 0x02,    /*!< Device Is Not -Initialized */
  LE_ALREDY_INIT = 0x03,     /*!< Device Already Initialized */
  LE_MNG_PROCESSING = 0x04,  /*!< MNG Processing in progress */
  LE_NO_ACL_LINK = 0x05,     /*!< No ACL Link */
  LE_DEV_ROLE_SLAVE = 0x06,  /*!< Device Role Slave */
  LE_GATT_IN_PROGRESS = 0x07 /*!< GATT Processing in Progress */
};

/*! TCU_MNG_LE_INIT_RESP status enumeration */
enum tcu_mng_le_init_response_status {
  LE_MNG_INIT_SUCCESS = 0x00,     /*!< Success */
  LE_PARAM_ABN_ERR = 0x86,        /*!< The abnormalities in a parameter */
  LE_MNG_INIT_ALREADY_DONE = 0x89 /*!< Device Is Already -Initialized */
};

/*! Contains TCU_LE_ACCEPT data */
struct tcu_le_accept
{
  enum tcu_mng_le_accept_status le_accept_status; /*!< status of request */
  uint16_t service_id;                            /*!< service id of request */
  uint8_t opcode;                                 /*!< opcode of request */
};

/*! Contains TCU_MNG_LE_INIT_REQ */
struct tcu_mng_le_init_request
{
  uint8_t device_name_len;    /*!< length of device name */
  const uint8_t *device_name; /*!< device name */
};

/*! Contains TCU_MNG_LE_INIT_RESP */
struct tcu_mng_le_init_response
{
  enum tcu_mng_le_init_response_status status; /*!< status */
  uint8_t bd_addr[TCU_SIZE_BDADDR];            /*!< Bluetooth device address */
};

/*! Contains generic HCI command error code */
enum hci_command_error_code {
  HCI_SUCCESS = 0x00,
  HCI_DEVICE_NOT_INITIALIZED = 0x81,         /*!< */
  HCI_REQUEST_IN_PROGRESS = 0x82,            /*!< */
  HCI_DISCONNECT_ERROR = 0x84,               /*!< */
  HCI_INVALID_PARAMETER_LENGTH_ERROR = 0x86, /*!< */
  HCI_ALREADY_ADVERTISING_ERROR = 0xa4       /*!< */
};

/*! Contains TCU_MNG_LE_DISCONNECT_EVENT */
struct tcu_mng_le_disconnect_event
{
  uint16_t connection_handle;         /*!< connection handle */
  enum hci_command_error_code status; /*!< status of the disconnect event */
  enum hci_command_error_code reason; /*!< reason for the disconnect event */
};

/*! Device role enumeration */
enum tcu_mng_le_device_role {
  LE_MASTER = 0x00, /*!< Master role */
  LE_SLAVE = 0x01   /*!< Slave role */
};

/*! Contains TCU_MNG_LE_CONNECTION_COMPLETE_EVENT data */
struct tcu_mng_le_connection_complete_event
{
  enum hci_command_error_code status;          /*!< status of the disconnect event */
  uint16_t connection_handle;                  /*!< connection handle */
  enum tcu_mng_le_device_role device_role;     /*!< device role */
  enum tcu_smp_address_type peer_address_type; /*!< public device address */
  uint8_t peer_address[TCU_SIZE_BDADDR];       /*!< peer address */
  uint16_t connection_interval;                /*!< connection interval */
  uint16_t connection_latency;                 /*!< connection latency */
  uint16_t supervision_timeout;                /*!< supervision timeout */
  uint8_t master_clock_accuracy;               /*!< master clock accuracy */
};

/*! Contains TCU_MNG_LE_CONNECTION_UPDATE_EVENT */
struct tcu_mng_le_connection_update_event
{
  enum hci_command_error_code status; /*!< status of the update event */
  uint16_t connection_handle;         /*!< connection handle */
  uint16_t connection_interval;       /*!< connection interval */
  uint16_t connection_latency;        /*!< connection latency */
  uint16_t supervision_timeout;       /*!< supervision timeout */
};

/*! Advertising type enumeration (TCU_MNG_LE_START_ADVERTISE_REQ) */
enum tcu_mng_le_advertising_type {
  LE_ADV_IND = 0x00,     /*!< Connectable undirected advertising */
  ADV_DIRECT_IND = 0x01, /*!< Connectable directed advertising */
  ADV_SCAN_IND = 0x02,   /*!< Scannable undirected advertising */
  ADV_NONCONN_IND = 0x03 /*!< Non connectable undirected advertising */
};

/*! Advertising filter policy enumeration (TCU_MNG_LE_START_ADVERTISE_REQ) */
enum tcu_mng_le_advertising_filter_policy {
  LE_SCAN_ANY_CONN_ANY = 0x00,     /*!< Allow Scan Request from Any, Allow Connect Request from Any (default). */
  ADV_SCAN_WLIST_CONN_ANY = 0x01,  /*!< Allow Scan Request from White List Only, Allow Connect Request from Any. */
  ADV_SCAN_ANY_CONN_WLIST = 0x02,  /*!< Allow Scan Request from Any, Allow Connect Request from White List Only. */
  ADV_SCAN_WLIST_CONN_WLIST = 0x03 /*!< Allow Scan Request from White List Only, Allow Connect Request from White List only. */
};

/*! Enumeration for advertising flags (TCU_MNG_LE_START_ADVERTISE_REQ)  */
enum tcu_mng_le_advertising_flag {
  ADV_TYPE_FLAGS = 0x01,                    /*!< AD Flags. */
  ADV_TYPE_16_BIT_MORE = 0x02,              /*!< More 16-bit UUIDs available. */
  ADV_TYPE_16_BIT_COMPLETE = 0x03,          /*!< Complete list of 16-bit UUIDs available. */
  ADV_TYPE_32_BIT_MORE = 0x04,              /*!< More 32-bit UUIDs available. */
  ADV_TYPE_32_BIT_COMPLETE = 0x05,          /*!< Complete list of 32-bit UUIDs available. */
  ADV_TYPE_128_BIT_MORE = 0x06,             /*!< More 128-bit UUIDs available. */
  ADV_TYPE_128_BIT_COMPLETE = 0x07,         /*!< Complete list of 128-bit UUIDs available. */
  ADV_TYPE_SHORT_NAME = 0x08,               /*!< Shortened local name. */
  ADV_TYPE_COMPLETE_NAME = 0x09,            /*!< Complete local name. */
  ADV_TYPE_TXPOWER_LEVEL = 0x0A,            /*!< TX Power Level (1 byte). */
  ADV_TYPE_CLASS_OF_DEV = 0x0D,             /*!< Class of device, Format defined in Assigned Numbers. */
  ADV_TYPE_SIMPLE_PAIRING_HC = 0x0E,        /*!< Simple Pairing Hash C (16 octets)Format defined in [Vol. 2], Part H Section 7.2.2. */
  ADV_TYPE_SIMPLE_PAIRING_RAND = 0x0F,      /*!< Simple Pairing Randomizer R(16 octets)Format defined in[Vol. 2], Part H Section 7.2.2. */
  ADV_TYPE_SM_TK_VALUE = 0x10,              /*!< TK Value Value as used in pairing over LE Physical channel. Format defined in [Vol. 3], Part H Section 2.3. */
  ADV_TYPE_SM_OOB_FLAGS = 0x11,             /*!< Security Manager OOB Flags (1 octet). */
  ADV_TYPE_SLV_CONN_INT_RANGE = 0x12,       /*!< Slave Connection Interval Range. */
  ADV_TYPE_SVC_SOLICITATION_16_BIT = 0x14,  /*!< Service Solicitation: Service UUIDs List of 16 bit Service UUIDs. */
  ADV_TYPE_SVC_SOLICITATION_128_BIT = 0x15, /*!< Service Solicitation: Service UUIDs List of 128 bit Service UUID. */
  ADV_TYPE_SERVICE_DATA = 0x16,             /*!< Service Data (2 or more octets) The first 2 octets contain the 16 bit Service UUID followed by additional service data. */
  ADV_TYPE_APPEARANCE_DATA = 0x19,          /*!< Appearance data */
  ADV_TYPE_MANUF_SPECIFIC_DATA = 0xFF       /*!< Manufacturer Specific Data (2 or more octets). The first 2 octets contain the Company Identifier Code followed by additional manufacturer specific data. */
};

/*! Describes advertising channel map channel 37 */
#define ADVERTISING_CHANNEL_MAP_CHANNEL_37 0x01
/*! Describes advertising channel map channel 38 */
#define ADVERTISING_CHANNEL_MAP_CHANNEL_38 0x02
/*! Describes advertising channel map channel 39 */
#define ADVERTISING_CHANNEL_MAP_CHANNEL_39 0x04

/*! Contains TCU_MNG_LE_START_ADVERTISE_REQ data */
struct tcu_mng_le_start_advertise_request
{
  uint16_t advertising_interval_minimum;                               /*!< minimum advertising interval */
  uint16_t advertising_interval_maximum;                               /*!< maximum advertising interval  */
  enum tcu_mng_le_advertising_type advertising_type;                   /*!< advertising type */
  enum tcu_smp_address_type own_address_type;                          /*!< own address type */
  enum tcu_smp_address_type direct_address_type;                       /*!< direct address type */
  uint8_t direct_address[TCU_SIZE_BDADDR];                             /*!< direct address */
  uint8_t advertising_channel_map;                                     /*!< channel map - use ADVERTISING_CHANNEL_MAP_CHANNEL_xxx defines */
  enum tcu_mng_le_advertising_filter_policy advertising_filter_policy; /*!< advertising filter policy */
  uint8_t advertising_data_length;                                     /*!< length of advertising data */
  uint8_t advertising_data[31];                                        /*!< advertising data */
  uint8_t scan_response_data_length;                                   /*!< length of scan response data */
  uint8_t scan_response_data[31];                                      /*!< scan response data */
};

/*! Contains TCU_MNG_LE_START_ADVERTISE_RESP data */
struct tcu_mng_le_start_advertising_response
{
  enum hci_command_error_code status; /*!< status of start advertising request */
};

/*! Contains TCU_MNG_LE_STOP_ADVERTISE_RESP data */
struct tcu_mng_le_disable_advertising_response
{
  enum hci_command_error_code status; /*!< status of stop advertising request */
};

/*! Contains TCU_MNG_LE_DISCONNECT_REQ data */
struct tcu_mng_le_disconnect_request
{
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< device to disconnect from */
};

/*! Contains TCU_MNG_LE_RESOLVE_BDADDR_REQ data */
struct tcu_mng_le_resolve_bdaddr_request
{
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< connected remote device */
  uint8_t number_of_irks;           /*!< number of IRKs to follow */
  uint8_t *irks;                    /*!< pointer to array of IRKs */
};

/*! Contains TCU_MNG_LE_RESOLVE_BDADDR_RESP data */
struct tcu_mng_le_resolve_bdaddr_response
{
  enum hci_command_error_code status;  /*!< status of request */
  uint8_t bd_addr[TCU_SIZE_BDADDR];    /*!< remote device */
  uint8_t local_irk[TCU_SIZE_LINKKEY]; /*!< local irk */
};

/*! Contains TCU_MNG_LE_CREATE_CONNECTION_REQ data */
struct tcu_mng_le_create_connection_request
{
  uint16_t le_scan_interval;             /*!< scan interval */
  uint16_t le_scan_window;               /*!< scan window*/
  uint8_t initiator_filter_policy;       /*!< initiator filter policy*/
  uint8_t peer_address_type;             /*!< peer address type */
  uint8_t peer_address[TCU_SIZE_BDADDR]; /*!< peer address */
  uint16_t connection_interval_min;      /*!< min. connection interval */
  uint16_t connection_interval_max;      /*!< max. connection interval */
  uint16_t connection_latency;           /*!< connection latency */
  uint16_t supervision_timeout;          /*!< supervision timeout */
  uint16_t min_ce_length;                /*!< min. ce length */
  uint16_t max_ce_length;                /*!< max. ce length */
  uint8_t own_address_type;              /*!< own address type*/
};

/*! Contains TCU_MNG_LE_CREATE_CONNECTION_CANCEL_RESP data */
struct tcu_mng_le_create_connection_cancel_response
{
  enum hci_command_error_code status; /*!< status */
};

/*! Contains TCU_MNG_LE_SET_SCAN_ENABLE_REQ data */
struct tcu_mng_le_set_scan_enable_request
{
  uint16_t le_scan_interval;      /*!< scan interval */
  uint16_t le_scan_window;        /*!< scan window */
  uint8_t le_scan_type;           /*!< scan type */
  uint8_t own_address_type;       /*!< own address type */
  uint8_t scanning_filter_policy; /*!< scanning filter policy */
  uint8_t le_scan_enable;         /*!< scan enable flag */
  uint8_t filter_duplicates;      /*!< duplicate filter enable flag */
};

/*! Contains TCU_MNG_LE_ADV_REPORT_EVENT */
struct tcu_mng_le_adv_report_event
{
  uint8_t unused; /*!< placeholder for later addition */
};

/*! Contains TCU_MNG_LE_SET_SCAN_ENABLE_RESP data */
struct tcu_mng_le_set_scan_enable_response
{
  enum hci_command_error_code status; /*!< status of the scanning request */
};

/*! Contains TCU_MNG_LE_CONNECTION_UPDATE_REQUEST data */
struct tcu_mng_le_connection_update_request
{
  uint16_t connection_handle;       /*!< connection handle */
  uint16_t connection_interval_min; /*!< min. connection interval */
  uint16_t connection_interval_max; /*!< max. connection interval */
  uint16_t connection_latency;      /*!< connection latency */
  uint16_t supervision_timeout;     /*!< supervision timeout */
  uint16_t min_ce_length;           /*!< min. ce length */
  uint16_t max_ce_length;           /*!< max. ce length */
};

/*! Contains TCU_MNG_LE_L2CAP_CONNECTION_UPDATE_RESP_EVENT data */
struct tcu_mng_le_l2cap_connection_update_resp_event
{
  uint16_t connection_handle; /*!< connection handle */
  uint16_t status;            /*!< status */
};

/*! Definition of TCU_LE_ACCEPT */
#define TCU_LE_ACCEPT (CMD_CONCAT(0xD1, 0xF1, 0x0003))
/*! Definition of TCU_LE_NOT_ACCEPT */
#define TCU_LE_NOT_ACCEPT (CMD_CONCAT(0xD1, 0xF2, 0x0002))
/*! Definition of TCU_LE_FATAL_ERROR */
#define TCU_LE_FATAL_ERROR (CMD_CONCAT(0xD1, 0xFE, 0x0001))
/*! Definition of TCU_MNG_LE_INIT_REQ */
#define TCU_MNG_LE_INIT_REQ (CMD_CONCAT(0xD1, 0x01, 0x00F9))
/*! Definition of TCU_MNG_LE_INIT_RESP */
#define TCU_MNG_LE_INIT_RESP (CMD_CONCAT(0xD1, 0x81, 0x0007))
/*! Definition of TCU_MNG_LE_SET_SCAN_ENABLE_REQ */
#define TCU_MNG_LE_SET_SCAN_ENABLE_REQ (CMD_CONCAT(0xD1, 0x0A, 0x0009))
/*! Definition of TCU_MNG_LE_SET_SCAN_ENABLE_RESP */
#define TCU_MNG_LE_SET_SCAN_ENABLE_RESP (CMD_CONCAT(0xD1, 0x8A, 0x0001))
/*! Definition of TCU_MNG_LE_ADV_REPORT_EVENT */
#define TCU_MNG_LE_ADV_REPORT_EVENT (CMD_CONCAT(0xD1, 0xC1, LEN_VAR))
/*! Definition of TCU_MNG_LE_CREATE_CONNECTION_REQ */
#define TCU_MNG_LE_CREATE_CONNECTION_REQ (CMD_CONCAT(0xD1, 0x0C, 0x0019))
/*! Definition of TCU_MNG_LE_CONNECTION_COMPLETE_EVENT */
#define TCU_MNG_LE_CONNECTION_COMPLETE_EVENT (CMD_CONCAT(0xD1, 0x4C, 0x0012))
/*! Definition of TCU_MNG_LE_L2CAP_CONNECTION_UPDATE_RESP_EVENT */
#define TCU_MNG_LE_L2CAP_CONNECTION_UPDATE_RESP_EVENT (CMD_CONCAT(0xD1, 0x50, 0x0004))
/*! Definition of TCU_MNG_LE_CONNECTION_UPDATE_REQ */
#define TCU_MNG_LE_CONNECTION_UPDATE_REQ (CMD_CONCAT(0xD1, 0x0E, 0x000E))
/*! Definition of TCU_MNG_LE_CONNECTION_UPDATE_EVENT */
#define TCU_MNG_LE_CONNECTION_UPDATE_EVENT (CMD_CONCAT(0xD1, 0x4E, 0x0009))
/*! Definition of TCU_MNG_LE_CREATE_CONNECTION_CANCEL_REQ */
#define TCU_MNG_LE_CREATE_CONNECTION_CANCEL_REQ (CMD_CONCAT(0xD1, 0x0D, NO_PARAM))
/*! Definition of TCU_MNG_LE_CREATE_CONNECTION_CANCEL_RESP */
#define TCU_MNG_LE_CREATE_CONNECTION_CANCEL_RESP (CMD_CONCAT(0xD1, 0x8D, 0x0001))
/*! Definition of TCU_MNG_LE_DISCONNECT_REQ */
#define TCU_MNG_LE_DISCONNECT_REQ (CMD_CONCAT(0xD1, 0x13, 0x0006))
/*! Definition of TCU_MNG_LE_DISCONNECT_EVENT */
#define TCU_MNG_LE_DISCONNECT_EVENT (CMD_CONCAT(0xD1, 0x93, 0x0002))
/*! Definition of TCU_MNG_LE_SET_SCAN_DISABLE_REQ */
#define TCU_MNG_LE_SET_SCAN_DISABLE_REQ (CMD_CONCAT(0xD1, 0x0B, NO_PARAM))
/*! Definition of TCU_MNG_LE_SET_SCAN_DISABLE_RESP */
#define TCU_MNG_LE_SET_SCAN_DISABLE_RESP (CMD_CONCAT(0xD1, 0x8B, 0x0001))
/*! Definition of TCU_MNG_LE_START_ADVERTISE_REQ */
#define TCU_MNG_LE_START_ADVERTISE_REQ (CMD_CONCAT(0xD1, 0x08, 0x004f))
/*! Definition of TCU_MNG_LE_START_ADVERTISE_RESP */
#define TCU_MNG_LE_START_ADVERTISE_RESP (CMD_CONCAT(0xD1, 0x88, 0x0001))
/*! Definition of TCU_MNG_LE_DISABLE_ADVERTISE_REQ */
#define TCU_MNG_LE_DISABLE_ADVERTISE_REQ (CMD_CONCAT(0xD1, 0x09, 0x0000))
/*! Definition of TCU_MNG_LE_DISABLE_ADVERTISE_RESP */
#define TCU_MNG_LE_DISABLE_ADVERTISE_RESP (CMD_CONCAT(0xD1, 0x89, 0x0001))
/*! Definition of TCU_MNG_LE_RESOLVE_BDADDR_REQ */
#define TCU_MNG_LE_RESOLVE_BDADDR_REQ (CMD_CONCAT(0xD1, 0x18, LEN_VAR))
/*! Definition of TCU_MNG_LE_RESOLVE_BDADDR_RESP */
#define TCU_MNG_LE_RESOLVE_BDADDR_RESP (CMD_CONCAT(0xD1, 0x55, LEN_VAR))

/**
* @brief       Event parser. Parses TCU_LE_ACCEPT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuLeAccept(uint8_t *data, struct tcu_le_accept *event);

/**
* @brief Construct and send TCU_MNG_LE_INIT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngLeInit_Request(struct tcu_mng_le_init_request *request);

/**
* @brief       Response parser. Parses TCU_MNG_LE_INIT_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuMngLeInit_Response(uint8_t *data, struct tcu_mng_le_init_response *response);

/**
* @brief Construct and send TCU_MNG_LE_START_ADVERTISE_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngLeStartAdvertise_Request(struct tcu_mng_le_start_advertise_request *request);

/**
* @brief       Response parser. Parses TCU_MNG_LE_START_ADVERTISE_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuMngLeStartAdvertise_Response(uint8_t *data, struct tcu_mng_le_start_advertising_response *response);

/**
* @brief Construct and send TCU_MNG_LE_DISABLE_ADVERTISE_REQ.
*/
enum tcu_func_status TcuMngLeDisableAdvertise_Request(void);

/**
* @brief       Response parser. Parses TCU_MNG_LE_DISABLE_ADVERTISE_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuMngLeDisableAdvertise_Response(uint8_t *data, struct tcu_mng_le_disable_advertising_response *response);

/**
* @brief Construct and send TCU_MNG_LE_CREATE_CONNECTION_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngLeCreateConnection_Request(struct tcu_mng_le_create_connection_request *request);

/**
* @brief       Event parser. Parses TCU_MNG_LE_CONNECTION_COMPLETE_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuMngLeConnectionComplete_Event(uint8_t *data, struct tcu_mng_le_connection_complete_event *event);

/**
* @brief Construct and send TCU_MNG_LE_CREATE_CONNECTION_CANCEL_REQ.
*/
enum tcu_func_status TcuMngLeCreateConnectionCancel_Request(void);

/**
* @brief       Response parser. Parses TCU_MNG_LE_CREATE_CONNECTION_CANCEL_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuMngLeCreateConnectionCancel_Response(uint8_t *data, struct tcu_mng_le_create_connection_cancel_response *response);

/**
* @brief Construct and send TCU_MNG_LE_DISCONNECT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngLeDisconnect_Request(struct tcu_mng_le_disconnect_request *request);

/**
* @brief       Event parser. Parses TCU_MNG_LE_DISCONNECT_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuMngLeDisconnect_Event(uint8_t *data, struct tcu_mng_le_disconnect_event *event);

/**
* @brief       Event parser. Parses TCU_MNG_LE_CONNECTION_UPDATE_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuMngLeConnectionUpdate_Event(uint8_t *data, struct tcu_mng_le_connection_update_event *event);

/**
* @brief Construct and send TCU_MNG_LE_RESOLVE_BDADDR_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngLeResolveBdaddr_Request(struct tcu_mng_le_resolve_bdaddr_request *request);

/**
* @brief       Response parser. Parses TCU_MNG_LE_RESOLVE_BDADDR_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuMngLeResolveBdaddr_Response(uint8_t *data, struct tcu_mng_le_resolve_bdaddr_response *response);

/**
* @brief Construct and send TCU_MNG_LE_SET_SCAN_ENABLE_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngLeSetScanEnable_Request(struct tcu_mng_le_set_scan_enable_request *request);

/**
* @brief       Response parser. Parses TCU_MNG_LE_SET_SCAN_ENABLE_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuMngLeSetScanEnable_Response(uint8_t *data, struct tcu_mng_le_set_scan_enable_response *response);

/**
* @brief       Event parser. Parses TCU_MNG_LE_ADV_REPORT_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuMngLeAdvReport_Event(uint8_t *data, struct tcu_mng_le_adv_report_event *event);

/**
* @brief Construct and send TCU_MNG_LE_CONNECTION_UPDATE_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngLeConnectionUpdate_Request(struct tcu_mng_le_connection_update_request *request);

/**
* @brief       Event parser. Parses TCU_MNG_LE_L2CAP_CONNECTION_UPDATE_RESP_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuMngLeL2CapConnectionUpdateResp_Event(uint8_t *data, struct tcu_mng_le_l2cap_connection_update_resp_event *event);

#endif

/************************** BEGIN of DISCLAIMER   **************************

- TOSHIBA is continually working to improve the quality and reliability of
  its products. Nevertheless, semiconductor devices in general can
  malfunction or fail due to their inherent electrical sensitivity and
  vulnerability to physical stress. It is the responsibility of the buyer,
  when utilizing TOSHIBA products, to comply with the standards of safety
  in making a safe design for the entire system, and to avoid situations in
  which a malfunction or failure of such TOSHIBA products could cause loss of
  human life, bodily injury or damage to property.

  In developing your designs, please ensure that TOSHIBA products are used
  within specified operating ranges as set forth in the most recent TOSHIBA
  products specifications.
  Also, please keep in mind the precautions and conditions set forth in the
  "Handling Guide for Semiconductor Devices," or
  "TOSHIBA Semiconductor Reliability Handbook" etc..

- The Toshiba products listed in this document are intended for usage in
  general electronics applications (computer, personal equipment, office
  equipment, measuring equipment, industrial robotics, domestic appliances,
  etc.).
  These Toshiba products are neither intended nor warranted for usage in
  equipment that requires extraordinarily high quality and/or reliability or
  a malfunction or failure of which may cause loss of human life or bodily
  injury ("Unintended Usage"). Unintended Usage include atomic energy control
  instruments, airplane or spaceship instruments, transportation instruments,
  traffic signal instruments, combustion control instruments, medical
  instruments, all types of safety devices, etc..
  Unintended Usage of Toshiba products listed in this document shall be
  made at the customer's own risk.

- Toshiba assumes no liability for any damage or losses (including but not
  limited to, loss of business profit,business interruption, loss of business
  information,and other pecuniary losses) occurring
  from the use of, or inability to use, this product.

- The products described in this document are subject to the foreign exchange
  and foreign trade laws.

- The products described in this document contain components made in the
  United States and subject to export control of the U.S. authorities.
  Diversion contrary to the U.S. law is prohibited.

- The information contained herein is presented only as a guide for the
  applications of our products.
  No responsibility is assumed by TOSHIBA CORPORATION for any infringements
  of intellectual property or other rights of the third parties which may
  result from its use.
  No license is granted by implication or otherwise under any intellectual
  property or other rights of TOSHIBA CORPORATION or others.

- The information contained herein is subject to change without notice.

****************************  END of DISCLAIMER ****************************/
