/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file tcu_mng.h
*
* @brief Contains definitions and function declarations for the management part (MNG) of the TCU API.
*
* All functions may return the following, common return codes.
*
* @return  TCU_FUNC_SUCCESS Completed succesfully
* @return  TCU_FUNC_OPCODE Op-code missmatch
* @return  TCU_FUNC_SERVICEID Sservice ID mismatch */
#ifndef __TCU_MNG_H__
#define __TCU_MNG_H__

#include "tcu_api.h"

/*! Definition for default timeout for TCU_MNG commands */
#define TCU_MNG_DEFAULT_CMD_TIMEOUT 100

/*! Definition of TCU_ACCEPT */
#define TCU_ACCEPT (CMD_CONCAT(0xE1, 0xF1, 0x0003))
/*! Definition of TCU_NOT_ACCEPT */
#define TCU_NOT_ACCEPT (CMD_CONCAT(0xE1, 0xF2, 0x0002))
/*! Definition of TCU_SYS_INVALID_COMMAND */
#define TCU_SYS_INVALID_COMMAND (CMD_CONCAT(0xE1, 0xFF, 0x0002))
/*! Definition of TCU_MNG_INIT_REQ */
#define TCU_MNG_INIT_REQ (CMD_CONCAT(0xE1, 0x01, LEN_VAR))
/*! Definition of TCU_MNG_INIT_RESP */
#define TCU_MNG_INIT_RESP (CMD_CONCAT(0xE1, 0x81, 0x0007))
/*! Definition of TCU_MNG_SET_SCAN_REQ */
#define TCU_MNG_SET_SCAN_REQ (CMD_CONCAT(0xE1, 0x0c, 0x0001))
/*! Definition of TCU_MNG_SET_SCAN_RESP */
#define TCU_MNG_SET_SCAN_RESP (CMD_CONCAT(0xE1, 0x8c, 0x0001))
/*! Definition of TCU_MNG_SSP_SET_REQ */
#define TCU_MNG_SSP_SET_REQ (CMD_CONCAT(0xE1, 0x3d, LEN_VAR))
/*! Definition of TCU_MNG_SSP_SET_RESP */
#define TCU_MNG_SSP_SET_RESP (CMD_CONCAT(0xE1, 0xbd, LEN_VAR))
/*! Definition of TCU_MNG_CONNECTION_REQUEST_EVENT */
#define TCU_MNG_CONNECTION_REQUEST_EVENT (CMD_CONCAT(0xE1, 0x55, 0x0009))
/*! Definition of TCU_MNG_CONNECTION_STATUS_EVENT */
#define TCU_MNG_CONNECTION_STATUS_EVENT (CMD_CONCAT(0xE1, 0x47, LEN_VAR))
/*! Definition of TCU_MNG_DISCOVER_REMOTE_DEVICE_RESULT_EVENT */
#define TCU_MNG_DISCOVER_REMOTE_DEVICE_RESULT_EVENT (CMD_CONCAT(0xE1, 0x44, LEN_VAR))
/*! Definition of TCU_MNG_DISCOVER_REMOTE_DEVICE_COMPLETE_EVENT */
#define TCU_MNG_DISCOVER_REMOTE_DEVICE_COMPLETE_EVENT (CMD_CONCAT(0xE1, 0x43, 0x00))
/*! Definition of TCU_MNG_DISCOVER_REMOTE_SERVICE_EVENT */
#define TCU_MNG_DISCOVER_REMOTE_SERVICE_EVENT (CMD_CONCAT(0xE1, 0x45, LEN_VAR))
/*! Definition of TCU_MNG_SSP_INFO_EVENT */
#define TCU_MNG_SSP_INFO_EVENT (CMD_CONCAT(0xE1, 0x7D, LEN_VAR))
/*! Definition of TCU_MNG_PIN_REQUEST_EVENT */
#define TCU_MNG_PIN_REQUEST_EVENT (CMD_CONCAT(0xE1, 0x48, LEN_VAR))
/*! Definition of TCU_MNG_PIN_WRITE_REQ */
#define TCU_MNG_PIN_WRITE_REQ (CMD_CONCAT(0xE1, 0x09, LEN_VAR))
/*! Definition of TCU_MNG_PIN_WRITE_RESP */
#define TCU_MNG_PIN_WRITE_RESP (CMD_CONCAT(0xE1, 0x89, 0x07))
/*! Definition of TCU_MNG_REMOTE_DEVICE_NAME_AUTO_NOTIFY_EVENT */
#define TCU_MNG_REMOTE_DEVICE_NAME_AUTO_NOTIFY_EVENT (CMD_CONCAT(0xE1, 0x6e, LEN_VAR))
/*! Definition of TCU_MNG_CONNECTION_ACCEPT_REQ */
#define TCU_MNG_CONNECTION_ACCEPT_REQ (CMD_CONCAT(0xE1, 0x13, LEN_VAR))
/*! Definition of TCU_MNG_CONNECTION_ACCEPT_RESP */
#define TCU_MNG_CONNECTION_ACCEPT_RESP (CMD_CONCAT(0xE1, 0x93, 0x01))
/*! Definition of TCU_MNG_DISCOVER_REMOTE_DEVICE_REQ */
#define TCU_MNG_DISCOVER_REMOTE_DEVICE_REQ (CMD_CONCAT(0xE1, 0x03, 0x01))
/*! Definition of TCU_MNG_DISCOVER_REMOTE_SERVICE_REQ */
#define TCU_MNG_DISCOVER_REMOTE_SERVICE_REQ (CMD_CONCAT(0xE1, 0x05, LEN_VAR))
/*! Definition of TCU_MNG_READ_RSSI_REQ */
#define TCU_MNG_READ_RSSI_REQ (CMD_CONCAT(0xE1, 0x0D, 0x06))
/*! Definition of TCU_MNG_READ_RSSI_RESP */
#define TCU_MNG_READ_RSSI_RESP (CMD_CONCAT(0xE1, 0x8D, 0x06))
/*! Definition of TCU_MNG_SET_DI_SDP_RECORD_REQ */
#define TCU_MNG_SET_DI_SDP_RECORD_REQ (CMD_CONCAT(0xE1, 0xDC, 0x0B))
/*! Definition of TCU_MNG_SET_DI_SDP_RECORD_RESP */
#define TCU_MNG_SET_DI_SDP_RECORD_RESP (CMD_CONCAT(0xE1, 0xDE, 0x01))
/*! Definition of TCU_MNG_SNIFF_MODE_CONTROL_REQ */
#define TCU_MNG_SNIFF_MODE_CONTROL_REQ (CMD_CONCAT(0xE1, 0xCB, 0x0E))
/*! Definition of TCU_MNG_SNIFF_MODE_CONTROL_RESP */
#define TCU_MNG_SNIFF_MODE_CONTROL_RESP (CMD_CONCAT(0xE1, 0xCD, 0x01))
/*! Definition of TCU_MNG_EXIT_SNIFF_MODE_CONTROL_REQ */
#define TCU_MNG_EXIT_SNIFF_MODE_CONTROL_REQ (CMD_CONCAT(0xE1, 0xCC, 0x06))
/*! Definition of TCU_MNG_EXIT_SNIFF_MODE_CONTROL_RESP */
#define TCU_MNG_EXIT_SNIFF_MODE_CONTROL_RESP (CMD_CONCAT(0xE1, 0xCE, 0x01))
/*! Definition of TCU_MNG_RCV_SNIFF_SUBRATING_EVENT */
#define TCU_MNG_RCV_SNIFF_SUBRATING_EVENT (CMD_CONCAT(0xE1, 0xEB, 0x09))
/*! Definition of TCU_MNG_SET_SNIFF_SUBRATING_PARAM_REQ */
#define TCU_MNG_SET_SNIFF_SUBRATING_PARAM_REQ (CMD_CONCAT(0xE1, 0xE0, 0x06))
/*! Definition of TCU_MNG_SET_SNIFF_SUBRATING_PARAM_RESP */
#define TCU_MNG_SET_SNIFF_SUBRATING_PARAM_RESP (CMD_CONCAT(0xE1, 0xEA, 0x01))

/*! TCU management status enumeration */
enum tcu_mng_status {
  TCU_MNG_SUCCESSFULL = 0x00,                         /*!< Successful */
  TCU_MNG_PARAMETER_FAILURE = 0x01,                   /*!< Parameter Failure */
  TCU_MNG_DEVICE_INITIALISATION_FINISHED = 0x02,      /*!< Device Initialization finished */
  TCU_MNG_NO_DEVICE_INITIALISATION = 0x03,            /*!< No Device Initialization */
  TCU_MNG_ON_DEVICE_SEARCHING = 0x04,                 /*!< On Device Searching */
  TCU_MNG_ON_SERVICE_SEARCHING = 0x05,                /*!< On Service Searching */
  TCU_MNG_NO_CONNECTION = 0x06,                       /*!< No connection*/
  TCU_MNG_NO_SETUP_PROFILE = 0x08,                    /*!< No profile was setup */
  TCU_MNG_ENABLE_SCAN_MODE = 0x09,                    /*!< Enable Scan mode */
  TCU_MNG_ESTABLISH_ACL_CONNECTION = 0x0A,            /*!< Establish ACL connection */
  TCU_MNG_NO_ACL_CONNECTION = 0x0B,                   /*!< No ACL connection */
  TCU_MNG_NOT_CONNECTION_ESTABLISHED_YET = 0x0C,      /*!< Not Connection Established yet */
  TCU_MNG_CONNECTION_WITH_MULTI = 0x0D,               /*!< Connection with Multi-connection restricted device */
  TCU_MNG_UNDER_CONNECTION_SETUP = 0x10,              /*!< Under Connection Setup of other Profile */
  TCU_MNG_SETUP_SPP = 0x40,                           /*!< Setup TCU_MNG_SERVICE_SPP */
  TCU_MNG_NO_SETUP_SPP = 0x41,                        /*!< No setup TCU_MNG_SERVICE_SPP  */
  TCU_MNG_ESTABLISH_SPP_CONNECTION = 0x42,            /*!< Establish TCU_MNG_SERVICE_SPP connection */
  TCU_MNG_ON_RELEASING_SPP_CONNECTION = 0x43,         /*!< On releasing TCU_MNG_SERVICE_SPP connection */
  TCU_MNG_NO_SPP_CONNECTION = 0x44,                   /*!< No TCU_MNG_SERVICE_SPP connection */
  TCU_MNG_DEVICE_ALREADY_EXISTS = 0x7d,               /*!< device already exists */
  TCU_MNG_PAGE_TIMER_OUT = 0x80,                      /*!< Page Timer-out */
  TCU_MNG_LOCAL_DEVICE_CONNECTION_REJECT = 0x81,      /*!< Local device Connection Reject */
  TCU_MNG_LINK_LOSS = 0x82,                           /*!< Link Loss */
  TCU_MNG_PIN_CODE_INPUT_TIMER_OUT = 0x83,            /*!< Pin code Input timer-out */
  TCU_MNG_PIN_CODE_FAILURE = 0x84,                    /*!< Pin code failure */
  TCU_MNG_LOCAL_DEVICE_PIN_CODE_INPUT_REJECT = 0x85,  /*!< Local device Pin code input reject */
  TCU_MNG_REMOTE_DEVICE_PIN_CODE_INPUT_REJECT = 0x86, /*!< Remote device Pin code input reject */
  TCU_MNG_LINK_KEY_FAILURE = 0x87,                    /*!< Link key failure */
  TCU_MNG_NO_SDP_SERVICE_SUPPORTED = 0x8d,            /*!< No SDP service supported */
  TCU_MNG_SNIFF_CANCEL_REQUEST_FAILURE = 0x96,        /*!< sniff cancel request failure */
  TCU_MNG_SPP_CONNECTION_TIME_OUT = 0xd0,             /*!< TCU_MNG_SERVICE_SPP connection timer-out */
  TCU_MNG_SPP_RELEASING_TIME_OUT = 0xd2,              /*!< TCU_MNG_SERVICE_SPP connection release timer-out */
  TCU_MNG_SPP_CONNECTION_FAILURE = 0xd3               /*!< TCU_MNG_SERVICE_SPP connection failure */
};

/*! Contains TCU_ACCEPT data */
struct tcu_accept_event
{
  enum tcu_mng_status status; /*!< status */
  uint16_t service_id;        /*!< service ID */
  uint8_t opcode;             /*!< opcode */
};

/*! Contains TCU_NOT_ACCEPT data */
struct tcu_not_accept_event
{
  uint16_t service_id; /*!< service ID */
  uint8_t opcode;      /*!< opcode */
};

/*! Contains TCU_SYS_INVALID_COMMAND data */
struct tcu_sys_invalid_com_event
{
  uint16_t service_id; /*!< service ID */
  uint8_t opcode;      /*!< opcode */
};

/*! Contains TCU_MNG_INIT_REQ */
struct tcu_mng_init_request
{
  uint8_t device_name_len;    /*!< length of device name */
  const uint8_t *device_name; /*!< device name */
};

/*! Contains TCU_MNG_INIT_RESP data */
struct tcu_mng_init_response
{
  enum tcu_mng_status status;       /*!< status */
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
};

/*! Contains TCU_MNG_SET_SCAN_REQ data */
struct tcu_mng_set_scan_request
{
  uint8_t scan_mode; /*!< scan mode */
};

/*! Contains TCU_MNG_SET_SCAN_RESP data */
struct tcu_mng_set_scan_response
{
  enum tcu_mng_status status; /*!< status */
};

/*! Contains TCU_MNG_CONNECTION_REQUEST_EVENT data */
struct tcu_mng_connection_request_event
{
  uint8_t bd_addr[TCU_SIZE_BDADDR];                  /*!< Bluetooth device address */
  uint8_t class_of_device[TCU_SIZE_CLASS_OF_DEVICE]; /*!< class of device */
};

/*! TCU management connection status enumeration */
enum tcu_mng_connection_status {
  TCU_MNG_CONNECTION_CONNECTED = 0x00,
  TCU_MNG_CONNECTION_DISCONNECTED = 0x01,
  TCU_MNG_CONNECTION_FAILURE = 0x02,
  TCU_MNG_CONNECTION_LINK_KEY = 0x03,
  TCU_MNG_CONNECTION_MODE_CHANGE_ACTIVE = 0x04,
  TCU_MNG_CONNECTION_MODE_CHANGE_HOLD = 0x05,
  TCU_MNG_CONNECTION_MODE_CHANGE_SNIFF = 0x06,
  TCU_MNG_CONNECTION_MODE_CHANGE_PARK = 0x07
};

/*! Contains TCU_MNG_CONNECTION_STATUS_EVENT data */
struct tcu_mng_connection_status_event
{
  enum tcu_mng_status status;                       /*!< status */
  uint8_t bd_addr[TCU_SIZE_BDADDR];                 /*!< Bluetooth device address */
  enum tcu_mng_connection_status connection_status; /*!< connection status enum */
  uint8_t link_key[TCU_SIZE_LINKKEY];               /*!< link key */
  uint8_t link_key_type;                            /*!< link key type */
  uint8_t sniff_interval;                           /*!< sniff interval */
};

/*! Contains TCU_MNG_DISCOVER_REMOTE_DEVICE_REQUEST data */
struct tcu_mng_discover_remote_device_request
{
  uint8_t max_number_of_reports; /*!< max. number of reported devices */
};

/*! Contains TCU_MNG_DISCOVER_REMOTE_DEVICE_RESULT_EVENT data */
struct tcu_mng_discover_remote_dev_result_event
{
  uint8_t bd_addr[TCU_SIZE_BDADDR];                  /*!< Bluetooth device address */
  uint8_t class_of_device[TCU_SIZE_CLASS_OF_DEVICE]; /*!< class of device */
  uint8_t length_of_device_name;                     /*!< Length of remote device name */
  uint8_t *name;                                     /*!< pointer to remote device name  */
};

/*! Contains TCU_MNG_DISCOVER_REMOTE_SERVICE_REQ data */
struct tcu_mng_discover_remote_service_request
{
  uint8_t security_Mode;              /*!< Sequrity Mode */
  uint8_t bd_addr[TCU_SIZE_BDADDR];   /*!< Bluetooth device address */
  uint8_t use_of_link_key;            /*!< use link key or not */
  uint8_t link_key[TCU_SIZE_LINKKEY]; /*!< link key */
};

/*! TCU management service data type enumeration */
enum tcu_mng_service_type {
  TCU_MNG_SERVICE_HFP = 0x01,
  TCU_MNG_SERVICE_OPP = 0x02,
  TCU_MNG_SERVICE_SPP = 0x03,
  TCU_MNG_SERVICE_A2DP = 0x06,
  TCU_MNG_SERVICE_AVRCP = 0x07,
  TCU_MNG_SERVICE_PBAP = 0x08,
  TCU_MNG_SERVICE_DEVICE_ID = 0x0e
};

/*! Contains additional information about device id profile */
struct tcu_mng_profile_information_device_id
{
  uint8_t valid;             /*!< information valid indicator */
  uint16_t specification_id; /*!< Specification ID  */
  uint16_t vendor_id;        /*!< Vendor ID  */
  uint16_t product_id;       /*!< Product ID  */
  uint16_t version;          /*!< version  */
  uint8_t primary_record;    /*!< primary_record */
  uint16_t vendor_id_source; /*!< Vendor ID Source */
};

/*! Contains additional information about spp profile */
struct tcu_mng_profile_information_spp
{
  uint8_t valid;                     /*!< information valid indicator */
  uint8_t number_of_server_channels; /*!< number of TCU_MNG_SERVICE_SPP server channels */
  uint8_t server_channel[10];        /*!< TCU_MNG_SERVICE_SPP server channel number */
};

/*! Contains TCU_MNG_DISCOVER_REMOTE_SERVICE_EVENT data */
struct tcu_mng_discover_remote_service_event
{
  enum tcu_mng_status status;                                         /*!< status */
  uint8_t bd_addr[TCU_SIZE_BDADDR];                                   /*!< Bluetooth device address */
  uint8_t service_count;                                              /*!< number of supported services */
  enum tcu_mng_service_type service_type[6];                          /*!< Supported services */
  uint8_t extension_info_profile;                                     /*!< profile additional information  */
  uint8_t extension_info_length;                                      /*!< length of profile additional information  */
  struct tcu_mng_profile_information_device_id information_device_id; /*!< information about device id profile (if available) */
  struct tcu_mng_profile_information_spp information_spp;             /*!< information about spp profile (if available) */
};

/*! Contains TCU_MNG_PIN_REQUEST_EVENT data */
struct tcu_mng_pin_request_event
{
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
  uint8_t length_of_device_name;    /*!< Length of device name */
  uint8_t *device_name;             /*!<  device name */
};

/*! Contains TCU_MNG_PIN_WRITE_REQ data */
struct tcu_mng_pin_write_request
{
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< pointer to BD addres */
  uint8_t length_of_pin_code;       /*!< Length of PIN code */
  uint8_t pin_code[16];             /*!< PIN code */
};

/*! Contains TCU_MNG_PIN_WRITE_RESP data */
struct tcu_mng_pin_write_response
{
  enum tcu_mng_status status;       /*!< status */
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
};

/*! Contains TCU_MNG_REMOTE_DEVICE_NAME_AUTO_NOTIFY_EVENT data */
struct tcu_mng_remote_device_name_auto_notify_event
{
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
  uint8_t length_of_device_name;    /*!< length of device name */
  uint8_t *device_name;             /*!< pointer to remote device name  */
};

/*! TCU management IO capability enumeration */
enum tcu_mng_smp_io_capability {
  TCU_MNG_DISPLAY_ONLY = 0x00,
  TCU_MNG_DISPLAY_YES_NO = 0x01,
  TCU_MNG_KEYBOARD_ONLY = 0x02,
  TCU_MNG_NO_INPUT_NO_OUTPUT = 0x03,
  TCU_MNG_INVALID_SETTING = 0xff
};

/*! TCU management OOB data presence enumeration */
enum tcu_mng_smp_oob_data_present {
  TCU_MNG_SMP_OOB_DATA_NOT_PRESENT = 0x00,
  TCU_MNG_SMP_OOB_DATA_IS_PRESENT = 0x01
};

/*! TCU management authentication requirement enumeration */
enum tcu_mng_smp_authentication_requirement {
  TCU_MNG_SMP_MITM_PROTECTION_NOT_REQUIRED_NOBONDING = 0x00,
  TCU_MNG_SMP_MITM_PROTECTION_REQUIRED_NOBONDING_IO_CAPABILITIES = 0x01,
  TCU_MNG_SMP_MITM_PROTECTION_NOT_REQUIRED_DEDICATED_BONDING = 0x02,
  TCU_MNG_SMP_MITM_PROTECTION_REQUIRED_DEDICATED_BONDINGIO_CAPABILITIES = 0x03,
  TCU_MNG_SMP_MITM_PROTECTION_NOT_REQUIREDGENERAL_BONDING = 0x04,
  TCU_MNG_SMP_MITM_PROTECTION_REQUIREDGENERAL_BONDING_IO_CAPABILITIES = 0x05
};

/*! TCU management HCI error code enumeration */
enum tcu_mng_hci_error {
  TCU_MNG_HCI_ERROR_BUSY_PAIRING = 0x38
};

/*! Contains HCI_IO_Capability_Request_Reply_Parameter portion of TCU_MNG_SSP_SET_REQ parameter data */
struct HCI_IO_Capability_Request_Reply_Parameter
{
  enum tcu_mng_smp_io_capability io_capability;                           /*!< io capabilities */
  enum tcu_mng_smp_oob_data_present oob_data_present;                     /*!< oob data present */
  enum tcu_mng_smp_authentication_requirement authentication_requirement; /*!< authentication requirement */
};

/*! Contains HCI_IO_Capability_Request_Negative_Reply portion of TCU_MNG_SSP_SET_REQ parameter data */
struct HCI_IO_Capability_Request_Negative_Reply
{
  enum tcu_mng_hci_error reason; /*!< reason for negative reply */
};

/*! Contains HCI_Write_Simple_Pairing_Debug_Mode_Command portion of TCU_MNG_SSP_SET_REQ parameter data */
struct HCI_Write_Simple_Pairing_Debug_Mode_Command
{
  uint8_t debug_mode; /*!< debug mode */
};

/*! Contains HCI_IO_User_Passkey_Request_Reply portion of TCU_MNG_SSP_SET_REQ parameter data */
struct HCI_IO_User_Passkey_Request_Reply
{
  uint32_t pass_key; /*!< pass key */
};

/*! Contains TCU_MNG_SSP_SET_REQ parameter data */
union tcu_mng_ssp_set_request_parameter
{
  /*! HCI_IO_Capability_Request_Reply_Parameter portion */
  struct HCI_IO_Capability_Request_Reply_Parameter HCI_IO_Capability_Request_Reply_Parameter;
  /*! HCI_IO_Capability_Request_Negative_Reply portion */
  struct HCI_IO_Capability_Request_Negative_Reply HCI_IO_Capability_Request_Negative_Reply;
  /*! HCI_Write_Simple_Pairing_Debug_Mode_Command portion */
  struct HCI_Write_Simple_Pairing_Debug_Mode_Command HCI_Write_Simple_Pairing_Debug_Mode_Command;
  /*! HCI_IO_User_Passkey_Request_Reply portion */
  struct HCI_IO_User_Passkey_Request_Reply HCI_IO_User_Passkey_Request_Reply;
};

/*! Contains TCU_MNG_SSP_SET_REQ data */
struct tcu_mng_ssp_set_request
{
  uint16_t opcode;                                   /*!< opcode */
  uint8_t parameter_length;                          /*!< Parameter length */
  uint8_t bd_addr[TCU_SIZE_BDADDR];                  /*!< Bluetooth device address */
  union tcu_mng_ssp_set_request_parameter parameter; /*!< request data */
};

/*! HCI I/O capability request reply opcode */
#define HCI_IO_Capability_Request_Reply_Opcode 0x042B
/*! HCI I/O capability request negative reply opcode */
#define HCI_IO_Capability_Request_Negative_Reply_Opcode 0x0434
/*! HCI user confirmation request reply opcode */
#define HCI_User_Confirmation_Request_Reply_Opcode 0x042C
/*! HCI user confirmation negative reply opcode */
#define HCI_User_Confirmation_Negative_Reply_Opcode 0x042D
/*! HCI I/O user passkey request reply opcode */
#define HCI_IO_User_Passkey_Request_Reply_Opcode 0x042E
/*! HCI I/O user passkey negative request reply opcode */
#define HCI_IO_User_Passkey_Negativ_Request_Reply_Opcode 0x042F
/*! HCI write simple pairing debug mode command opcode */
#define HCI_Write_Simple_Pairing_Debug_Mode_Command_Opcode 0x1804

/*! Contains HCI_IO_Capability_Request_Reply_response replay data */
struct HCI_IO_Capability_Request_Reply_response
{
  uint8_t parameter_length;         /*!< parameter length */
  uint8_t opcode_command_complete;  /*!< opcode data */
  uint8_t event_length;             /*!< event length */
  uint16_t opcode;                  /*!< opcode */
  uint16_t status;                  /*!< status */
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
};

/*! Contains HCI_IO_Capability_Request_Negative_Reply_response data */
struct HCI_IO_Capability_Request_Negative_Reply_response
{
  uint16_t status;                  /*!< status */
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
};

/*! Contains TCU_MNG_SSP_SET_RESP data */
struct tcu_mng_ssp_set_response
{
  enum tcu_mng_status status;       /*!< status */
  uint8_t parameter_length;         /*!< Parameter length */
  uint8_t opcode_command_complete;  /*!< opcode command complete data */
  uint8_t event_length;             /*!< Event length data */
  uint8_t Num_HCI_Command_Packet;   /*!< Num_HCI_Command_Packet  */
  uint16_t opcode;                  /*!< cpcode of command */
  uint8_t command_status;           /*!< comand status */
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
};

/*! TCU management SSP info event opcode enumeration */
enum tcu_mng_ssp_info_event_opcode {
  HCI_Encryption_Key_Refresh_Complete_Event = 0x30,
  HCI_IO_Capability_Request_Event = 0x31,
  HCI_IO_Capability_Response_Event = 0x32,
  HCI_IO_User_Confirmation_Request_Event = 0x33,
  HCI_User_Passkey_Request = 0x34,
  HCI_Simple_Pairing_Complete_Event = 0x36,
  HCI_User_Passkey_Notification = 0x3b
};

/*! Contains HCI_Encryption_Key_Refresh_Complete_Event portion of TCU_MNG_SSP_INFO_EVENT details data */
struct HCI_Encryption_Key_Refresh_Complete_Event
{
  uint8_t status;             /*!< status */
  uint16_t connection_handle; /*!< status */
};

/*! Contains HCI_IO_Capability_Request_Event portion of TCU_MNG_SSP_INFO_EVENT details data */
struct HCI_IO_Capability_Request_Event
{
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
};

/*! Contains HCI_IO_Capability_Response_Event portion of TCU_MNG_SSP_INFO_EVENT details data */
struct HCI_IO_Capability_Response_Event
{
  uint8_t bd_addr[TCU_SIZE_BDADDR];                                       /*!< Bluetooth device address */
  enum tcu_mng_smp_io_capability io_capability;                           /*!< IO_CAPABILITY enum */
  enum tcu_mng_smp_oob_data_present oob_data_present;                     /*!< OOB data present enum */
  enum tcu_mng_smp_authentication_requirement authentication_requirement; /*!< authentication_requirement enum */
};

/*! Contains HCI_IO_User_Confirmation_Request_Event portion of TCU_MNG_SSP_INFO_EVENT details data */
struct HCI_IO_User_Confirmation_Request_Event
{
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
  uint32_t pass_key;                /*!< confirmation of a numeric value */
};

/*! Contains HCI_Simple_Pairing_Complete_Event portion of TCU_MNG_SSP_INFO_EVENT details data */
struct HCI_Simple_Pairing_Complete_Event
{
  uint8_t status;                   /*!< status */
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
};

/*! Contains HCI_User_Passkey_Request_Events portion of TCU_MNG_SSP_INFO_EVENT details data */
struct HCI_User_Passkey_Request_Events
{
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
};

/*! Contains HCI_User_Passkey_Notification_Events portion of TCU_MNG_SSP_INFO_EVENT details data */
struct HCI_User_Passkey_Notification_Events
{
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
  uint32_t pass_key;                /*!< confirmation of a numeric value */
};

/*! Contains TCU_MNG_SSP_INFO_EVENT details data */
union tcu_mng_ssp_info_event_details
{
  /*! HCI_Encryption_Key_Refresh_Complete_Event portion */
  struct HCI_Encryption_Key_Refresh_Complete_Event HCI_Encryption_Key_Refresh_Complete_Event;
  /*! HCI_IO_Capability_Request_Event portion */
  struct HCI_IO_Capability_Request_Event HCI_IO_Capability_Request_Event;
  /*! HCI_IO_Capability_Response_Event portion */
  struct HCI_IO_Capability_Response_Event HCI_IO_Capability_Response_Event;
  /*! HCI_IO_User_Confirmation_Request_Event portion */
  struct HCI_IO_User_Confirmation_Request_Event HCI_IO_User_Confirmation_Request_Event;
  /*! HCI_Simple_Pairing_Complete_Event portion */
  struct HCI_Simple_Pairing_Complete_Event HCI_Simple_Pairing_Complete_Event;
  /*! HCI_User_Passkey_Request_Events portion */
  struct HCI_User_Passkey_Request_Events HCI_User_Passkey_Request_Events;
  /*! HCI_User_Passkey_Notification_Events portion */
  struct HCI_User_Passkey_Notification_Events HCI_User_Passkey_Notification_Events;
};

/*! Contains TCU_MNG_SSP_INFO_EVENT data */
struct tcu_mng_ssp_info_event
{
  enum tcu_mng_ssp_info_event_opcode opcode;    /*!< opcode */
  uint8_t parameter_length;                     /*!< parameter length */
  union tcu_mng_ssp_info_event_details details; /*!< event details */
};

/*! TCU management connection response type enumeration */
enum tcu_mng_connection_response_type {
  TCU_MNG_CONNECTION_ACCEPT = 0x00,
  TCU_MNG_CONNECTION_REJECT = 0x01
};

/*! TCU management link key usage enumeration */
enum tcu_mng_link_key_usage {
  TCU_MNG_USE_LINK_KEY = 0x00,
  TCU_MNG_NO_LINK_KEY = 0x01
};

/*! Contains TCU_MNG_CONNECTION_ACCEPT_REQ data */
struct tcu_mng_connection_accept_request
{
  enum tcu_mng_connection_response_type response_type; /*!< response type */
  uint8_t bd_addr[TCU_SIZE_BDADDR];                    /*!< Bluetooth device address */
  uint8_t use_of_link_key;                             /*!< use or not link key */
  uint8_t link_key[TCU_SIZE_LINKKEY];                  /*!< to link key */
};

/*! Contains TCU_MNG_READ_RSSI_REQ data */
struct tcu_mng_read_rssi_request
{
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
};

/*! Contains TCU_MNG_READ_RSSI_RESP data */
struct tcu_mng_read_rssi_response
{
  enum tcu_mng_status status;       /*!< status */
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
  uint16_t rssi;                    /*!< rssi value" */
};

/*! Contains TCU_MNG_SNIFF_MODE_CONTROL_REQ data */
struct tcu_mng_sniff_mode_control_request
{
  uint8_t bd_addr[TCU_SIZE_BDADDR]; /*!< Bluetooth device address */
  uint16_t max_interval;            /*!< max. interval */
  uint16_t min_interval;            /*!< min. interval */
  uint16_t attempt;                 /*!< attempt  */
  uint16_t timeout;                 /*!< timeout */
};

/*! Contains TCU_MNG_SET_DI_SDP_RECORD_REQ data */
struct tcu_mng_set_di_sdp_record_request
{
  uint16_t specification_id; /*!< Specification ID */
  uint16_t vendor_id;        /*!< Vendor ID */
  uint16_t product_id;       /*!< Product ID */
  uint16_t version;          /*!< version */
  uint8_t primary_record;    /*!< primary_record */
  uint16_t vendor_id_source; /*!< vendor_id_source */
};

/**
* @brief       Event parser. Parses TCU_ACCEPT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuAccept_Event(uint8_t *data, struct tcu_accept_event *event);

/**
* @brief       Event parser. Parses TCU_NOT_ACCEPT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuNotAccept_Event(uint8_t *data, struct tcu_not_accept_event *event);

/**
* @brief       Event parser. Parses TCU_SYS_INVALID_COMMAND.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuSysInvalidCommand_Event(uint8_t *data, struct tcu_sys_invalid_com_event *event);

/**
* @brief Construct and send TCU_MNG_INIT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngInit_Request(struct tcu_mng_init_request *request);

/**
* @brief       Response parser. Parses TCU_MNG_INIT_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuMngInit_Response(uint8_t *data, struct tcu_mng_init_response *response);

/**
* @brief Construct and send TCU_MNG_SET_SCAN_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngSetScan_Request(struct tcu_mng_set_scan_request *request);

/**
* @brief       Response parser. Parses TCU_MNG_SET_SCAN_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuMngSetScan_Response(uint8_t *data, struct tcu_mng_set_scan_response *response);

/**
* @brief       Event parser. Parses TCU_MNG_CONNECTION_STATUS_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuMngConnectionStatus_Event(uint8_t *data, struct tcu_mng_connection_status_event *event);

/**
* @brief       Event parser. Parses TCU_MNG_CONNECTION_REQUEST_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuMngConnectionRequest_Event(uint8_t *data, struct tcu_mng_connection_request_event *event);

/**
* @brief Construct and send TCU_MNG_CONNECTION_ACCEPT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngConnectionAccept_Request(struct tcu_mng_connection_accept_request *request);

/**
* @brief       Response parser. Parses TCU_MNG_CONNECTION_ACCEPT_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuMngConnectionAccept_Response(uint8_t *data, enum tcu_mng_connection_response_type *response);

/**
* @brief       Event parser. Parses TCU_MNG_PIN_REQUEST_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuMngPinRequest_Event(uint8_t *data, struct tcu_mng_pin_request_event *event);

/**
* @brief Construct and send TCU_MNG_PIN_WRITE_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngPinWrite_Request(struct tcu_mng_pin_write_request *request);

/**
* @brief       Response parser. Parses TCU_MNG_PIN_WRITE_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuMngPinWrite_Response(uint8_t *data, struct tcu_mng_pin_write_response *response);

/**
* @brief Construct and send TCU_MNG_DISCOVER_REMOTE_DEVICE_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngDiscoverRemoteDevice_Request(struct tcu_mng_discover_remote_device_request *request);

/**
* @brief       Event parser. Parses TCU_MNG_DISCOVER_REMOTE_DEVICE_RESULT_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuMngDiscoverRemoteDeviceResult_Event(uint8_t *data, struct tcu_mng_discover_remote_dev_result_event *event);

/**
* @brief       Event parser. Parses TCU_MNG_DISCOVER_REMOTE_DEVICE_COMPLETE_EVENT.
*
* @param[in]     *data pointer to received data
*/
enum tcu_func_status TcuMngDiscoverRemoteDeviceComplete_Event(uint8_t *data);

/**
* @brief Construct and send TCU_MNG_DISCOVER_REMOTE_SERVICE_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngDiscoverRemoteService_Request(struct tcu_mng_discover_remote_service_request *request);

/**
* @brief       Event parser. Parses TCU_MNG_DISCOVER_REMOTE_SERVICE_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuMngDiscoverRemoteService_Event(uint8_t *data, struct tcu_mng_discover_remote_service_event *event);

/**
* @brief       Event parser. Parses TCU_MNG_REMOTE_DEV_NAME_AUTO_NOTIFY_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuMngRemoteDevNameAutoNotify_Event(uint8_t *data, struct tcu_mng_remote_device_name_auto_notify_event *event);

/**
* @brief       Event parser. Parses TCU_MNG_SSP_INFO_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuMngSspInfo_Event(uint8_t *data, struct tcu_mng_ssp_info_event *event);

/**
* @brief Construct and send TCU_MNG_SSP_SET_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngSspSet_Request(struct tcu_mng_ssp_set_request *request);

/**
* @brief       Response parser. Parses TCU_MNG_SSP_SET_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuMngSspSet_Response(uint8_t *data, struct tcu_mng_ssp_set_response *response);

/**
* @brief Construct and send TCU_MNG_READ_RSSI_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngReadRssi_Request(struct tcu_mng_read_rssi_request *request);

/**
* @brief       Response parser. Parses TCU_MNG_READ_RSSI_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuMngReadRssi_Response(uint8_t *data, struct tcu_mng_read_rssi_response *response);

/**
* @brief Construct and send TCU_MNG_SET_DI_SDP_RECORD_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuMngSetDiSdpRecord_Request(struct tcu_mng_set_di_sdp_record_request *request);

/**
* @brief       Response parser. Parses TCU_MNG_SET_DI_SDP_RECORD_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuMngSetDiSdpRecord_Response(uint8_t *data, enum tcu_mng_status *response);


#if 0
void tcu_application_log_event(uint8_t *buf, const char *name, const char *format, ...);

void tcu_application_log_response(uint8_t *buf, const char *name, const char *format, ...);

void tcu_application_log_request(uint8_t *buf, const char *name, const char *format, ...);
#endif
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
