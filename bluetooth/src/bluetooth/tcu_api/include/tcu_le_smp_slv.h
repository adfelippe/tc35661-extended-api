/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file tcu_le_smp_slv.h
*
* @brief Contains definitions and function declarations for the security manager protocl (slave) (LE_SMP_SLV) part of the TCU API.
*
* All functions may return the following, common return codes.
*
* @return    TCU_FUNC_SUCCESS Completed succesfully
* @return    TCU_FUNC_OPCODE Op-code missmatch
* @return    TCU_FUNC_SERVICEID Sservice ID mismatch
*/
#ifndef __TCU_LE_SMP_SLV_H__
#define __TCU_LE_SMP_SLV_H__

#include "tcu_api.h"

/*! Definition of TCU_LE_SMP_SLV_SECURITY_REQ */
#define TCU_LE_SMP_SLV_SECURITY_REQ (CMD_CONCAT(0xD5, 0x02, 0x0003))
/*! Definition of TCU_LE_SMP_SLV_PAIRING_EVENT */
#define TCU_LE_SMP_SLV_PAIRING_EVENT (CMD_CONCAT(0xD5, 0xC1, 0x0008))
/*! Definition of TCU_LE_SMP_SLV_PAIRING_ACCEPT_REQ */
#define TCU_LE_SMP_SLV_PAIRING_ACCEPT_REQ (CMD_CONCAT(0xD5, 0x01, 0x0009))
/*! Definition of TCU_LE_SMP_SLV_PAIRING_REJECT_REQ */
#define TCU_LE_SMP_SLV_PAIRING_REJECT_REQ (CMD_CONCAT(0xD5, 0x01, 0x0003))
/*! Definition of TCU_LE_SMP_SLV_PAIRING_ACCEPT_RESP */
#define TCU_LE_SMP_SLV_PAIRING_ACCEPT_RESP (CMD_CONCAT(0xD5, 0x81, 0x0003))
/*! Definition of TCU_LE_SMP_SLV_STK_GEN_METHOD_EVENT */
#define TCU_LE_SMP_SLV_STK_GEN_METHOD_EVENT (CMD_CONCAT(0xD5, 0xCB, 0x0004))
/*! Definition of TCU_LE_SMP_SLV_KEY_ENTRY_REQ_EVENT */
#define TCU_LE_SMP_SLV_KEY_ENTRY_REQ_EVENT (CMD_CONCAT(0xD5, 0x44, 0x0002))
/*! Definition of TCU_LE_SMP_SLV_KEY_ENTRY_ACCEPT_REQ */
#define TCU_LE_SMP_SLV_KEY_ENTRY_ACCEPT_REQ (CMD_CONCAT(0xD5, 0x05, 0x0007))
/*! Definition of TCU_LE_SMP_SLV_KEY_ENTRY_REJECT_REQ */
#define TCU_LE_SMP_SLV_KEY_ENTRY_REJECT_REQ (CMD_CONCAT(0xD5, 0x05, 0x0003))
/*! Definition of TCU_LE_SMP_SLV_KEY_ENTRY_WRITE_RESP */
#define TCU_LE_SMP_SLV_KEY_ENTRY_WRITE_RESP (CMD_CONCAT(0xD5, 0x85, 0x0003))
/*! Definition of TCU_LE_SMP_SLV_DISPLAY_KEY_EVENT */
#define TCU_LE_SMP_SLV_DISPLAY_KEY_EVENT (CMD_CONCAT(0xD5, 0x46, 0x0002))
/*! Definition of TCU_LE_SMP_SLV_DISPLAY_KEY_WRITE_ACCEPT_REQ */
#define TCU_LE_SMP_SLV_DISPLAY_KEY_WRITE_ACCEPT_REQ (CMD_CONCAT(0xD5, 0x07, 0x0007))
/*! Definition of TCU_LE_SMP_SLV_DISPLAY_KEY_WRITE_REJECT_REQ */
#define TCU_LE_SMP_SLV_DISPLAY_KEY_WRITE_REJECT_REQ (CMD_CONCAT(0xD5, 0x07, 0x0003))
/*! Definition of TCU_LE_SMP_SLV_DISPLAY_KEY_WRITE_RESP */
#define TCU_LE_SMP_SLV_DISPLAY_KEY_WRITE_RESP (CMD_CONCAT(0xD5, 0x87, 0x0003))
/*! Definition of TCU_LE_SMP_SLV_OOB_KEY_ENTRY_REQ_EVENT */
#define TCU_LE_SMP_SLV_OOB_KEY_ENTRY_REQ_EVENT (CMD_CONCAT(0xD5, 0x59, 0x0002))
/*! Definition of TCU_LE_SMP_SLV_OOB_KEY_ENTRY_ACCEPT_REQ */
#define TCU_LE_SMP_SLV_OOB_KEY_ENTRY_ACCEPT_REQ (CMD_CONCAT(0xD5, 0x1A, 0x0013))
/*! Definition of TCU_LE_SMP_SLV_OOB_KEY_ENTRY_REJECT_REQ */
#define TCU_LE_SMP_SLV_OOB_KEY_ENTRY_REJECT_REQ (CMD_CONCAT(0xD5, 0x1A, 0x0003))
/*! Definition of TCU_LE_SMP_SLV_OOB_KEY_ENTRY_WRITE_RESP */
#define TCU_LE_SMP_SLV_OOB_KEY_ENTRY_WRITE_RESP (CMD_CONCAT(0xD5, 0x9A, 0x0003))
/*! Definition of TCU_LE_SMP_SLV_STK_GENERATED_EVENT */
#define TCU_LE_SMP_SLV_STK_GENERATED_EVENT (CMD_CONCAT(0xD5, 0x48, 0x0012))
/*! Definition of TCU_LE_SMP_SLV_STK_ENCRYPT_SESSION_REQ_REPLY_EVENT */
#define TCU_LE_SMP_SLV_STK_ENCRYPT_SESSION_REQ_REPLY_EVENT (CMD_CONCAT(0xD5, 0xCE, 0x0013))
/*! Definition of TCU_LE_SMP_SLV_LTK_ENCRYPT_SESSION_REQ_REPLY_EVENT */
#define TCU_LE_SMP_SLV_LTK_ENCRYPT_SESSION_REQ_REPLY_EVENT (CMD_CONCAT(0xD5, 0xCF, 0x0013))
/*! Definition of TCU_LE_SMP_SLV_ENCRYPTION_CHANGE_EVENT */
#define TCU_LE_SMP_SLV_ENCRYPTION_CHANGE_EVENT (CMD_CONCAT(0xD5, 0xD0, 0x0006))
/*! Definition of TCU_LE_SMP_SLV_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT */
#define TCU_LE_SMP_SLV_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT (CMD_CONCAT(0xD5, 0xD1, 0x0005))
/*! Definition of TCU_LE_SMP_SLV_LTK_SENT_EVENT */
#define TCU_LE_SMP_SLV_LTK_SENT_EVENT (CMD_CONCAT(0xD5, 0xCC, 0x0012))
/*! Definition of TCU_LE_SMP_SLV_EDIV_RAND_SENT_EVENT */
#define TCU_LE_SMP_SLV_EDIV_RAND_SENT_EVENT (CMD_CONCAT(0xD5, 0xCD, 0x000C))
/*! Definition of TCU_LE_SMP_SLV_IRK_SENT_EVENT */
#define TCU_LE_SMP_SLV_IRK_SENT_EVENT (CMD_CONCAT(0xD5, 0xD3, 0x0012))
/*! Definition of TCU_LE_SMP_SLV_IDENTITY_ADDRESS_SENT_EVENT */
#define TCU_LE_SMP_SLV_IDENTITY_ADDRESS_SENT_EVENT (CMD_CONCAT(0xD5, 0xD4, 0x0009))
/*! Definition of TCU_LE_SMP_SLV_CSRK_SENT_EVENT */
#define TCU_LE_SMP_SLV_CSRK_SENT_EVENT (CMD_CONCAT(0xD5, 0xD5, 0x0012))
/*! Definition of TCU_LE_SMP_SLV_LTK_RECEIVED_EVENT */
#define TCU_LE_SMP_SLV_LTK_RECEIVED_EVENT (CMD_CONCAT(0xD5, 0xC9, 0x0012))
/*! Definition of TCU_LE_SMP_SLV_EDIV_RAND_RECEIVED_EVENT */
#define TCU_LE_SMP_SLV_EDIV_RAND_RECEIVED_EVENT (CMD_CONCAT(0xD5, 0xCA, 0x000C))
/*! Definition of TCU_LE_SMP_SLV_IRK_RECEIVED_EVENT */
#define TCU_LE_SMP_SLV_IRK_RECEIVED_EVENT (CMD_CONCAT(0xD5, 0xD6, 0x0012))
/*! Definition of TCU_LE_SMP_SLV_IDENTITY_ADDRESS_RECEIVED_EVENT */
#define TCU_LE_SMP_SLV_IDENTITY_ADDRESS_RECEIVED_EVENT (CMD_CONCAT(0xD5, 0xD7, 0x0009))
/*! Definition of TCU_LE_SMP_SLV_CSRK_RECEIVED_EVENT */
#define TCU_LE_SMP_SLV_CSRK_RECEIVED_EVENT (CMD_CONCAT(0xD5, 0xD8, 0x0012))
/*! Definition of TCU_LE_SMP_SLV_PAIRING_COMPLETED_EVENT */
#define TCU_LE_SMP_SLV_PAIRING_COMPLETED_EVENT (CMD_CONCAT(0xD5, 0xD2, 0x0003))
/*! Definition of TCU_LE_SMP_SLV_PAIRING_FAILED_EVENT */
#define TCU_LE_SMP_SLV_PAIRING_FAILED_EVENT (CMD_CONCAT(0xD5, 0x43, 0x0003))
/*! Definition of TCU_LE_SMP_SLV_KEY_REQ_EVENT */
#define TCU_LE_SMP_SLV_KEY_REQ_EVENT (CMD_CONCAT(0xD5, 0xDA, 0x0009))
/*! Definition of TCU_LE_SMP_SLV_KEY_ACCEPT_REQ */
#define TCU_LE_SMP_SLV_KEY_ACCEPT_REQ (CMD_CONCAT(0xD5, 0x1C, LEN_VAR))
/*! Definition of TCU_LE_SMP_SLV_KEY_REJECT_REQ */
#define TCU_LE_SMP_SLV_KEY_REJECT_REQ (CMD_CONCAT(0xD5, 0x1C, 0x0003))
/*! Definition of TCU_LE_SMP_SLV_KEY_ACCEPT_RESP */
#define TCU_LE_SMP_SLV_KEY_ACCEPT_RESP (CMD_CONCAT(0xD5, 0x9C, 0x0003))
/*! Definition of TCU_LE_SMP_SLV_STORE_KEY_EVENT */
#define TCU_LE_SMP_SLV_STORE_KEY_EVENT (CMD_CONCAT(0xD5, 0xD9, 0x000A))
/*! Definition of TCU_LE_SMP_SLV_BONDING_ENABLED_INFO_REQ */
#define TCU_LE_SMP_SLV_BONDING_ENABLED_INFO_REQ (CMD_CONCAT(0xD5, 0x1B, 0x0002))
/*! Definition of TCU_LE_SMP_SLV_BONDING_ENABLED_INFO_RESP */
#define TCU_LE_SMP_SLV_BONDING_ENABLED_INFO_RESP (CMD_CONCAT(0xD5, 0x9B, 0x0003))

/*! Definition for SMP security key size */
#define SMP_SECURITY_KEY_SIZE TCU_SIZE_LINKKEY

/*! Definition for SMP random number key size */
#define SMP_RAND_NUM_KEY_SIZE 8

/*! Definition for SMP EDIV key size */
#define SMP_EDIV_NUM_KEY_SIZE 2

/*! SMP status code enumeration */
enum smp_status {
  STATUS_SUCCESS = 0x00,                             /*!< success */
  STATUS_PASSKEY_ENTRY_FAILED = 0x01,                /*!< passkey entry failed */
  STATUS_OOB_NOT_AVAILABLE = 0x02,                   /*!< */
  STATUS_AUTHENTICATION_REQUIREMENTS_NOT_MET = 0x03, /*!< */
  STATUS_CONFIRM_VALUE_FAILED = 0x04,                /*!< */
  STATUS_PAIRING_NOT_SUPPORTED = 0x05,               /*!< */
  STATUS_ENCRYPTION_KEY_SIZE_INSUFFICIENT = 0x06,    /*!< */
  STATUS_COMMAND_NOT_SUPPORTED = 0x07,               /*!< */
  STATUS_UNSPECIFIED_REASON = 0x08,                  /*!< */
  STATUS_REPEATED_ATTEMPTS = 0x09,                   /*!< */
  STATUS_INVALID_PARAMETERS = 0x0A,                  /*!< */
  STATUS_SMP_TIME_OUT = 0x0B,                        /*!< */
  STATUS_SMP_TIMEOUT_NO_FURTHER_SMP = 0x0C,          /*!< */
  STATUS_DEVICE_NOT_INITIALIZED = 0x81,              /*!< */
  STATUS_REQUEST_IN_PROGRESS = 0x82,                 /*!< */
  STATUS_DISCONNECT_ERROR = 0x84,                    /*!< */
  STATUS_INVALID_PARAMETER_LENGTH_ERROR = 0x86       /*!< */
};

/*! SMP I/O capabilities enumeration */
enum tcu_smp_io_capabilities {
  SMP_IO_CAP_DISPLAY_ONLY = 0x00, /*!< display only */
  SMP_IO_CAP_DISPLAY_YES_NO,      /*!< display + yes/no keyboard */
  SMP_IO_CAP_KEYBOARD_ONLY,       /*!< keyboard only */
  SMP_IO_CAP_NO_IP_NO_OP,         /*!< no input, no output */
  SMP_IO_CAP_KEYBOARD_DISPLAY     /*!< full keyboard and display */
};

/*! SMP authentication requirements enumeration */
enum tcu_smp_authentication_requirements {
  SMP_AUTH_NO_MITM_NO_BONDING = 0x00,      /*!< no mitm protection, no bonding */
  SMP_AUTH_BONDING_ENABLED_NO_MITM = 0x01, /*!< mitm protection, no bonding */
  SMP_AUTH_NO_BONDING_MITM_ENABLED = 0x04, /*!< no mitm protection, general bonding */
  SMP_AUTH_BONDING_AND_MITM_ENABLED = 0x05 /*!< mitm protection, general bonding */
};

/*! Contains TCU_LE_SMP_SLV_PAIRING_ACCEPT_RESP/TCU_LE_SMP_SLV_KEY_ENTRY_WRITE_RESP/TCU_LE_SMP_SLV_DISPLAY_KEY_WRITE_RESP
/TCU_LE_SMP_SLV_OOB_KEY_ENTRY_WRITE_RESP/TCU_LE_SMP_SLV_KEY_ACCEPT_RESP */
struct tcu_le_smp_slv_response
{
  uint16_t connection_handle; /*!< connection handle */
  enum smp_status smp_status; /*!< SMP status for request response */
};

/*! SMP STK generation method enumeration*/
enum tcu_le_smp_slv_stk_generation_method {
  STK_JUST_WORK_METHOD,                     /*!< just works */
  STK_PASSKEY_INIT_DISP_RESP_INPUT_METHOD,  /*!< initiator displays, responder inputs authenticated */
  STK_PASSKEY_INIT_INPUT_RESP_DISP_METHOD,  /*!< initiator inputs, responder displays authenticated */
  STK_PASSKEY_INIT_INPUT_RESP_INPUT_METHOD, /*!< initiator and responder input authenticated */
  STK_OOB_METHOD                            /*!< oob */
};

/*! Contains TCU_LE_SMP_SLV_STK_GEN_METHOD_EVENT */
struct tcu_le_smp_slv_stk_gen_method_event
{
  uint16_t connection_handle;                                      /*!< connection handle */
  enum smp_status smp_status;                                      /*!< smp status */
  enum tcu_le_smp_slv_stk_generation_method stk_generation_method; /*!< stk generation method  */
};

/*! Contains TCU_LE_SMP_SLV_KEY_ENTRY_REQ_EVENT / TCU_LE_SMP_SLV_OOB_KEY_ENTRY_REQ_EVENT / TCU_LE_SMP_SLV_DISPLAY_KEY_REQ_EVENT */
struct tag_le_smp_key_req_event
{
  uint16_t connection_handle; /*!< connection handle */
};

/*! Contains TCU_LE_SMP_SLV_STK_ENCRYPT_SESSION_REQ_REPLY_EVENT */
struct tcu_le_smp_slv_stk_encrypt_session_req_reply_event
{
  uint16_t connection_handle;         /*!< connection handle */
  enum smp_status smp_status;         /*!< smp status */
  uint8_t stk[SMP_SECURITY_KEY_SIZE]; /*!< security key */
};

/*! Contains TCU_LE_SMP_SLV_LTK_ENCRYPT_SESSION_REQ_REPLY_EVENT */
struct tcu_le_smp_slv_ltk_encrypt_session_req_reply_event
{
  uint16_t connection_handle;         /*!< connection handle */
  enum smp_status smp_status;         /*!< smp status */
  uint8_t ltk[SMP_SECURITY_KEY_SIZE]; /*!< security key */
};

/*! SMP key type enumeration */
enum tcu_smp_slv_encryption_key_type {
  SMP_KEY_TYPE_STK = 0x01, /*!< stk */
  SMP_KEY_TYPE_LTK = 0x02  /*!< ltk */
};

/*! SMP encryption flag enumeration */
enum tcu_smp_slv_encryption_flag {
  SMP_ENCRYPTION_ENABLED = 0x00, /*!< enabled */
  SMP_ENCRYPTION_DISABLED = 0x01 /*!< disabled*/
};

/*! Contains TCU_LE_SMP_SLV_ENCRYPTION_CHANGE_EVENT */
struct tcu_le_smp_slv_encryption_change_event
{
  uint16_t connection_handle;                       /*!< connection handle */
  enum smp_status smp_status;                       /*!< smp status */
  enum tcu_smp_slv_encryption_key_type key_type;    /*!< encryption key type */
  enum tcu_smp_slv_encryption_flag encryption_flag; /*!< encryption key flag */
  uint8_t encryption_key_size;                      /*!< encryption key size */
};

/*! Contains TCU_LE_SMP_SLV_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT */
struct tcu_le_smp_slv_encryption_key_refresh_complete_event
{
  uint16_t connection_handle;                    /*!< connection handle */
  enum smp_status smp_status;                    /*!< smp status */
  enum tcu_smp_slv_encryption_key_type key_type; /*!< encryption key type */
  uint8_t encryption_key_size;                   /*!< encryption key size */
};

/*! Contains STK Gen Event and the Security Keys (LTK, IRK and CSRK) received/sent event */
struct tcu_le_smp_slv_security_key
{
  uint16_t connection_handle;                  /*!< connection handle */
  uint8_t security_key[SMP_SECURITY_KEY_SIZE]; /*!< security key */
};

/*! Contains TCU_LE_SMP_SLV_EDIV_RAND_RECEIVED_EVENT/TCU_LE_SMP_SLV_EDIV_RAND_SENT_EVENT */
struct tcu_le_smp_slv_ediv_rand_event
{
  uint16_t connection_handle;          /*!< connection handle */
  uint16_t ediv;                       /*!< ediv */
  uint8_t rand[SMP_RAND_NUM_KEY_SIZE]; /*!< rand */
};

/*! Contains TCU_LE_SMP_SLV_IDENTITY_ADDRESS_RECEIVED_EVENT/TCU_LE_SMP_SLV_IDENTITY_ADDRESS_SENT_EVENT*/
struct tcu_le_smp_slv_identity_adress_event
{
  uint16_t connection_handle;                /*!< connection handle */
  enum tcu_smp_address_type address_type;    /*!< address type */
  uint8_t identity_address[TCU_SIZE_BDADDR]; /*!< identity address */
};

/*! Contains TCU_LE_SMP_SLV_PAIRING_COMPLETED_EVENT */
struct tcu_le_smp_slv_pairing_complete_event
{
  uint16_t connection_handle; /*!< connection handle */
  enum smp_status smp_status; /*!< smp status */
};

/*! Contains TCU_LE_SMP_SLV_PAIRING_FAILED_EVENT */
struct tcu_le_smp_slv_pairing_failed_event
{
  uint16_t connection_handle; /*!< connection handle */
  enum smp_status smp_status; /*!< smp status */
};

/*! Contains TCU_LE_SMP_SLV_KEY_REQ_EVENT */
struct tcu_le_smp_slv_key_req_event
{
  uint16_t connection_handle;             /*!< connection handle */
  enum tcu_smp_address_type address_type; /*!< address type */
  uint8_t bd_address[TCU_SIZE_BDADDR];    /*!< address */
};

/*! Contains SMP key store action */
enum tcu_le_smp_slv_store_key_status {
  STORE_DEVICE_INFO = 0x01, /*!<  */
  DELETE_DEVICE_INFO = 0x02 /*!<  */
};

/*! Contains TCU_LE_SMP_SLV_STORE_KEY_EVENT */
struct tcu_le_smp_slv_store_key_event
{
  uint16_t connection_handle;                  /*!< connection handle */
  enum tcu_smp_address_type address_type;      /*!< address type */
  uint8_t bd_address[TCU_SIZE_BDADDR];         /*!< address */
  enum tcu_le_smp_slv_store_key_status status; /*!< store key status */
};

/*! Contains TCU_LE_SMP_SLV_BONDING_ENABLED_INFO_REQ */
struct tcu_le_smp_slv_bonding_enabled_info_request
{
  uint16_t connection_handle; /*!< connection handle */
};

/*! Contains TCU_LE_SMP_SLV_BONDING_ENABLED_INFO_RESP */
struct tcu_le_smp_slv_bonding_enabled_info_response
{
  uint16_t connection_handle; /*!< connection handle */
  enum smp_status smp_status; /*!< smp status */
};

/*! SMP OOB authentication data available enumeration */
enum tcu_le_smp_slv_oob_authentication_data_available {
  SMP_OOB_DATA_NOT_PRESENT = 0x00, /*!< SMP OOB data not present */
  SMP_OOB_DATA_PRESENT = 0x01      /*!< SMP OOB data not present */
};

/*! Contains TCU_LE_SMP_SLV_PAIRING_ACCEPT_REQ / TCU_LE_SMP_SLV_PAIRING_REJECT_REQ */
struct tcu_le_smp_slv_pairing_request
{
  uint16_t connection_handle;                                          /*!< connection handle */
  enum smp_status smp_status;                                          /*!< smp status */
  enum tcu_smp_io_capabilities io_capabilities;                        /*!< io capabilities */
  enum tcu_le_smp_slv_oob_authentication_data_available oob_data_flag; /*!< oob data available */
  enum tcu_smp_authentication_requirements authentication_requirement; /*!< authentication requirement */
  uint8_t encryption_key_size;                                         /*!< encryption key size */
  uint8_t initiator_key_distribution;                                  /*!< initiator key distribution */
  uint8_t responder_key_distribution;                                  /*!< responder key distribution */
};

/*! SMP key set flag LTK enabled flag definition */
#define SMP_LTK_ENABLED 0x01
/*! SMP key set flag IRK enabled flag definition */
#define SMP_IRK_ENABLED 0x02
/*! SMP key set flag CSRK enabled flag definition */
#define SMP_CSRK_ENABLED 0x04

/*! Contains intermediate structure used in TCU_LE_SMP_SLV_KEY_ACCEPT_REQ */
struct tcu_smp_slv_key_element
{
  uint16_t ediv;                                  /*!< ediv*/
  uint8_t identity_address_type;                  /*!< identity address type */
  uint8_t identity_address_info[TCU_SIZE_BDADDR]; /*!< identity address info */
  uint8_t rand[SMP_RAND_NUM_KEY_SIZE];            /*!< rand */
  uint8_t ltk[SMP_SECURITY_KEY_SIZE];             /*!< ltk */
  uint8_t irk[SMP_SECURITY_KEY_SIZE];             /*!< irk */
  uint8_t csrk[SMP_SECURITY_KEY_SIZE];            /*!< csrk */
  uint8_t keys_set_flag;                          /*!< key set flag */
};

/*! Contains TCU_LE_SMP_SLV_KEY_ACCEPT_REQ */
struct tcu_le_smp_slv_key_accept_request
{
  struct tcu_smp_slv_key_element local;  /*!< local key elements */
  struct tcu_smp_slv_key_element remote; /*!< remote key elements */
  uint8_t encryption_key_size;           /*!< encryption key size */
};

/*! Contains TCU_LE_SMP_SLV_DISPLAY_KEY_WRITE_ACCEPT_REQ / TCU_LE_SMP_SLV_KEY_ENTRY_ACCEPT_REQ */
struct tcu_le_smp_slv_key_write_request
{
  uint16_t connection_handle; /*!< connection handle */
  enum smp_status smp_status; /*!< smp status */
  uint32_t key;               /*!< key to be used */
};

/*! Contains TCU_LE_SMP_SLV_PAIRING_EVENT */
struct tcu_le_smp_slv_pairing_event
{
  uint16_t connection_handle;                                           /*!< connection handle */
  enum tcu_smp_io_capabilities io_capabilities;                         /*!< io capabilities */
  enum tcu_le_smp_slv_oob_authentication_data_available oob_data_flag;  /*!< oob data available */
  enum tcu_smp_authentication_requirements authentication_requirements; /*!< authentication requirements */
  uint8_t encryption_key_size;                                          /*!< encryption key size */
  uint8_t initiator_key_distribution;                                   /*!< initiator key distribution */
  uint8_t responder_key_distribution;                                   /*!< responder key distribution */
};

/*! Contains TCU_LE_SMP_SLV_SECURITY_REQ */
struct tcu_le_smp_slv_security_request
{
  uint16_t connection_handle;                                          /*!< connection handle */
  enum tcu_smp_authentication_requirements authentication_requirement; /*!< authentication requirements */
};

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_PAIRING_COMPLETED_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvPairingComplete_Event(uint8_t *data, struct tcu_le_smp_slv_pairing_complete_event *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_PAIRING_FAILED_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvPairingFailed_Event(uint8_t *data, struct tcu_le_smp_slv_pairing_failed_event *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_KEY_REQ_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvKeyReq_Event(uint8_t *data, struct tcu_le_smp_slv_key_req_event *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_STORE_KEY_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvStoreKey_Event(uint8_t *data, struct tcu_le_smp_slv_store_key_event *event);

/**
* @brief Construct and send TCU_LE_SMP_SLV_BONDING_ENABLED_INFO_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeSmpSlvBondingEnabledInfo_Request(struct tcu_le_smp_slv_bonding_enabled_info_request *request);

/**
* @brief       Response parser. Parses TCU_LE_SMP_SLV_BONDING_ENABLED_INFO_RESPONSE.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeSmpSlvBondingEnabledInfo_Response(uint8_t *data, struct tcu_le_smp_slv_bonding_enabled_info_response *response);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_STK_GENERATED_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvStkGenerated_Event(uint8_t *data, struct tcu_le_smp_slv_security_key *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_STK_ENCRYPT_SESSION_REQ_REPLY_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvStkEncryptSessionReqReply_Event(uint8_t *data, struct tcu_le_smp_slv_stk_encrypt_session_req_reply_event *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_LTK_ENCRYPT_SESSION_REQ_REPLY_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvLtkEncryptSessionReqReply_Event(uint8_t *data, struct tcu_le_smp_slv_ltk_encrypt_session_req_reply_event *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_ENCRYPTION_CHANGE_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvStkEnryptionChange_Event(uint8_t *data, struct tcu_le_smp_slv_encryption_change_event *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvStkEnryptionKeyRefreshComplete_Event(uint8_t *data, struct tcu_le_smp_slv_encryption_key_refresh_complete_event *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_LTK_RECEIVED_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvLtkReceived_Event(uint8_t *data, struct tcu_le_smp_slv_security_key *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_LTK_SENT_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvLtkSent_Event(uint8_t *data, struct tcu_le_smp_slv_security_key *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_EDIV_RAND_RECEIVED_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvEdivRandReceived_Event(uint8_t *data, struct tcu_le_smp_slv_ediv_rand_event *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_EDIV_RAND_SENT_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvEdivRandSent_Event(uint8_t *data, struct tcu_le_smp_slv_ediv_rand_event *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_IDENTITY_ADDRESS_RECEIVED_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvIdentityAddressReceived_Event(uint8_t *data, struct tcu_le_smp_slv_identity_adress_event *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_IDENTITY_ADDRESS_SENT_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvIdentityAddressSent_Event(uint8_t *data, struct tcu_le_smp_slv_identity_adress_event *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_PAIRING_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvPairing_Event(uint8_t *data, struct tcu_le_smp_slv_pairing_event *event);

/**
* @brief       Response parser. Parses TCU_LE_SMP_SLV_PAIRING_ACCEPT_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeSmpSlvPairingAccept_Response(uint8_t *data, struct tcu_le_smp_slv_response *response);

/**
* @brief       Response parser. Parses TCU_LE_SMP_SLV_KEY_ENTRY_WRITE_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeSmpSlvKeyEntryWrite_Response(uint8_t *data, struct tcu_le_smp_slv_response *response);

/**
* @brief       Response parser. Parses TCU_LE_SMP_SLV_DISPLAY_KEY_WRITE_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeSmpSlvDisplayKeyWrite_Response(uint8_t *data, struct tcu_le_smp_slv_response *response);

/**
* @brief       Response parser. Parses TCU_LE_SMP_SLV_OOB_KEY_ENTRY_WRITE_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeSmpSlvOobKeyEntryWrite_Response(uint8_t *data, struct tcu_le_smp_slv_response *response);

/**
* @brief       Response parser. Parses TCU_LE_SMP_SLV_KEY_ACCEPT_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeSmpSlvKeyAccept_Response(uint8_t *data, struct tcu_le_smp_slv_response *response);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_STK_GEN_METHOD_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvGenMethod_Event(uint8_t *data, struct tcu_le_smp_slv_stk_gen_method_event *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_KEY_ENTRY_REQ_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvKeyEntryReq_Event(uint8_t *data, struct tag_le_smp_key_req_event *event);

/**
* @brief Construct and send TCU_LE_SMP_SLV_KEY_ENTRY_ACCEPT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeSmpSlvKeyEntry_Request(struct tcu_le_smp_slv_key_write_request *request);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_DISPLAY_KEY_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvDisplayKey_Event(uint8_t *data, struct tag_le_smp_key_req_event *event);

/**
* @brief Construct and send TCU_LE_SMP_SLV_DISPLAY_KEY_WRITE_ACCEPT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeSmpSlvDisplayKeyWrite_Request(struct tcu_le_smp_slv_key_write_request *request);

/**
* @brief Construct and send TCU_LE_SMP_SLV_PAIRING_ACCEPT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeSmpSlvPairing_Request(struct tcu_le_smp_slv_pairing_request *request);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_IRK_RECEIVED_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvIrkReceived_Event(uint8_t *data, struct tcu_le_smp_slv_security_key *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_IRK_SENT_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvIrkSent_Event(uint8_t *data, struct tcu_le_smp_slv_security_key *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_CSRK_RECEIVED_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvCsrkReceived_Event(uint8_t *data, struct tcu_le_smp_slv_security_key *event);

/**
* @brief       Event parser. Parses TCU_LE_SMP_SLV_CSRK_SENT_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeSmpSlvCsrkSent_Event(uint8_t *data, struct tcu_le_smp_slv_security_key *event);

/**
* @brief Construct and send TCU_LE_SMP_SLV_KEY_ACCEPT_REQ.
*
* @param[in]     connection_handle Connection handle
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeSmpSlvKeyAccept_Request(uint16_t connection_handle, struct tcu_le_smp_slv_key_accept_request *request);

/**
* @brief Construct and send TCU_LE_SMP_SLV_KEY_REJECT_REQ.
*
* @param[in]     connection_handle Connection handle
*/
enum tcu_func_status TcuLeSmpSlvKeyReject_Request(uint16_t connection_handle);

/**
* @brief Construct and send TCU_LE_SMP_SLV_SECURITY_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeSmpSlvSecurity_Request(struct tcu_le_smp_slv_security_request *request);

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
