/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file tcu_le_gatt_ser.h
*
* @brief Contains definitions and function declarations for the GATT server part of the TCU API.
*
* All functions may return the following, common return codes.
*
* @return    TCU_FUNC_SUCCESS Completed succesfully
* @return    TCU_FUNC_OPCODE Op-code missmatch
* @return    TCU_FUNC_SERVICEID Sservice ID mismatch
*/
#ifndef __TCU_LE_GATT_SER_H__
#define __TCU_LE_GATT_SER_H__

#include "tcu_api.h"

/*! GATT characteristic properties  */
#define LE_BROADCAST 0x01          /*!< Permits broadcasts of the Characteristic Value using Characteristic ConfigurationDescriptor */
#define LE_READ 0x02               /*!< Permits reads of the Characteristic Value */
#define LE_WRITE_WO_RESP 0x04      /*!< Permits writes of the Characteristic Value without response */
#define LE_WRITE 0x08              /*!< Permits writes of the Characteristic Value with response */
#define LE_NOTIFY 0x10             /*!< Permits notifications of a Characteristic Value without acknowledgement */
#define LE_INDICATE 0x20           /*!< Permits indications of a Characteristic Value with acknowledgement */
#define LE_AUTH_SIGNED_WRITES 0x40 /*!< Permits signed writes to the Characteristic Value */
#define LE_EXT_PROP 0x80           /*!< Additional characteristic properties are defined in the Characteristic Extended Properties Descriptor */

/*! Masks for attribute permissions */
#define LE_GATT_READ 0x0001           /*!< read permission */
#define LE_GATT_WRITE 0x0002          /*!< write permission */
#define LE_GATT_AUTHORIZATION 0x0004  /*!< authorization */
#define LE_GATT_AUTHENTICATION 0x0008 /*!< authentication */

/*! Contains TCU_LE_GATT_SER_INIT_RESP */
struct tcu_le_gatt_ser_init_response
{
  enum tcu_gatt_status gatt_status; /*!< GATT status */
};

/*! Contains TCU_LE_GATT_SER_EXG_MTU_EVENT */
struct tcu_le_gatt_ser_exg_mtu_event
{
  uint16_t connection_handle; /*!< connection handle */
  uint16_t client_mtu_size;   /*!< client mtu size */
};

/*! Contains TCU_LE_GATT_SER_EXG_MTU_ACCEPT_REQ */
struct tcu_le_gatt_ser_exg_mtu_accept_request
{
  uint16_t connection_handle;       /*!< connection handle */
  enum tcu_gatt_status gatt_status; /*!< GATT status */
  uint16_t server_mtu_size;         /*!< server mtu size */
};

/*! Contains TCU_LE_GATT_SER_EXG_MTU_ACCEPT_RESP */
struct tcu_le_gatt_ser_exg_mtu_accept_response
{
  uint16_t connection_handle;       /*!< connection handle */
  enum tcu_gatt_status gatt_status; /*!< GATT status from remote device */
  uint16_t server_mtu_size;         /*!< server mtu size */
};

/*! Contains TCU_LE_GATT_SER_READ_CHAR_DESP_EVENT */
struct tcu_le_gatt_ser_read_char_desp_event
{
  uint16_t connection_handle; /*!< connection handle */
  uint16_t descriptor_handle; /*!< descriptor handle to be read */
};

/*! Contains TCU_LE_GATT_SER_READ_CHAR_DESP_ACCEPT_REQ */
struct tcu_le_gatt_ser_read_char_desp_accept_request
{
  uint16_t connection_handle;       /*!< connection handle */
  enum tcu_gatt_status gatt_status; /*!< GATT status of the read operation */
  uint16_t descriptor_handle;       /*!< descriptor handle that was read */
};

/*! Contains TCU_LE_GATT_SER_READ_CHAR_DESP_ACCEPT_RESP */
struct tcu_le_gatt_ser_read_char_desp_accept_response
{
  uint16_t connection_handle;       /*!< connection handle */
  enum tcu_gatt_status gatt_status; /*!< GATT status from remote device */
};

/*! Contains TCU_LE_GATT_SER_WRITE_CHAR_DESP_EVENT */
struct tcu_le_gatt_ser_write_char_desp_event
{
  uint16_t connection_handle; /*!< connection handle */
  uint16_t descriptor_handle; /*!< descriptor handle to be written */
  uint16_t length;            /*!< length of the data */
  uint8_t *value;             /*!< data */
};

/*! Contains TCU_LE_GATT_SER_WRITE_CHAR_DESP_ACCEPT_REQ */
struct tcu_le_gatt_ser_write_char_desp_accept_request
{
  uint16_t connection_handle;       /*!< connection handle */
  enum tcu_gatt_status gatt_status; /*!< GATT status of write operation*/
  uint16_t descriptor_handle;       /*!< descriptor handle that was written */
};

/*! Contains TCU_LE_GATT_SER_WRITE_CHAR_DESP_ACCEPT_RESP */
struct tcu_le_gatt_ser_write_char_desp_accept_response
{
  uint16_t connection_handle;       /*!< connection handle */
  enum tcu_gatt_status gatt_status; /*!< GATT status from remote device */
};

/*! Contains TCU_LE_GATT_SER_READ_CHAR_VAL_EVENT */
struct tcu_le_gatt_ser_read_char_val_event
{
  uint16_t connection_handle; /*!< connection handle */
  uint16_t value_handle;      /*!< value handle to be read */
};

/*! Contains TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_REQ */
struct tcu_le_gatt_ser_read_char_val_accept_request
{
  uint16_t connection_handle;       /*!< connection handle */
  enum tcu_gatt_status gatt_status; /*!< GATT status of read operation*/
  uint16_t value_handle;            /*!< value handle that was read */
};

/*! Contains TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_RESP */
struct tcu_le_gatt_ser_read_char_val_accept_response
{
  uint16_t connection_handle;       /*!< connection handle */
  enum tcu_gatt_status gatt_status; /*!< GATT status from remote device */
};

/*! Contains TCU_LE_GATT_SER_WRITE_CHAR_VAL_EVENT */
struct tcu_le_gatt_ser_write_char_val_event
{
  uint16_t connection_handle; /*!< connection handle */
  uint16_t value_handle;      /*!< value handle to be written */
  uint16_t length;            /*!< length of the data */
  uint8_t *value;             /*!< data */
};

/*! Contains TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_REQ */
struct tcu_le_gatt_ser_write_char_val_accept_request
{
  uint16_t connection_handle;       /*!< connection handle */
  enum tcu_gatt_status gatt_status; /*!< GATT status of write operation */
  uint16_t value_handle;            /*!< value handle that was written */
};

/*! Contains TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_RESP */
struct tcu_le_gatt_ser_write_char_val_accept_response
{
  uint16_t connection_handle;       /*!< connection handle */
  enum tcu_gatt_status gatt_status; /*!< GATT status from remote device */
};

/*! Contains TCU_LE_GATT_SER_READ_MULTIPLE_EVENT */
struct tcu_le_gatt_ser_read_multiple_event
{
  uint16_t connection_handle; /*!< connection handle */
  uint16_t handle_count;      /*!< number of handles */
  uint8_t *handle_list;       /*!< list of handles to be read */
};

/*! Contains TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_RESP */
struct tcu_le_gatt_ser_read_multiple_accept_request
{
  uint16_t connection_handle;       /*!< connection handle */
  enum tcu_gatt_status gatt_status; /*!< GATT status of read operation */
  uint16_t handle;                  /*!< handle that cause the error (if any) */
};

/*! Contains TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_RESP */
struct tcu_le_gatt_ser_read_multiple_accept_response
{
  uint16_t connection_handle;       /*!< connection handle */
  enum tcu_gatt_status gatt_status; /*!< GATT status from remote device */
};

/*! Contains TCU_LE_GATT_SER_CHAR_VAL_INDICATION_EVENT */
struct tcu_le_gatt_ser_char_val_indication_event
{
  uint16_t connection_handle;       /*!< connection handle */
  enum tcu_gatt_status gatt_status; /*!< GATT status from remote device */
};

/*! Contains TCU_LE_GATT_SER_CHAR_VAL_NOTIFICATION_EVENT */
struct tcu_le_gatt_ser_char_val_notification_event
{
  uint16_t connection_handle; /*!< connection handle */
};

/*! Contains TCU_LE_GATT_SER_WRITE_WITHOUT_RESPONSE_CMD_EVENT */
struct tcu_le_gatt_ser_write_without_response_cmd_event
{
  uint16_t connection_handle; /*!< connection handle */
  uint16_t value_handle;      /*!< value handle */
  uint16_t length;            /*!< length of the data */
  uint8_t *value;             /*!< data */
};

/*! Contains TCU_LE_GATT_SER_CHAR_VAL_NOTIFICATION_REQ */
struct tcu_le_gatt_ser_char_val_notification_request
{
  uint16_t connection_handle; /*!< connection handle */
  uint16_t value_handle;      /*!< value handle */
  uint16_t length;            /*!< length of the data */
  uint8_t *value;             /*!< data */
};

/*! Contains TCU_LE_GATT_SER_CHAR_VAL_INDICATION_REQ */
struct tcu_le_gatt_ser_char_val_indication_request
{
  uint16_t connection_handle; /*!< connection handle */
  uint16_t value_handle;      /*!< value handle */
  uint16_t length;            /*!< length of the data */
  uint8_t *value;             /*!< data */
};

/*! Definition of TCU_LE_GATT_SER_INIT_REQ */
#define TCU_LE_GATT_SER_INIT_REQ (CMD_CONCAT(0xD3, 0x00, NO_PARAM))
/*! Definition of TCU_LE_GATT_SER_INIT_RESP */
#define TCU_LE_GATT_SER_INIT_RESP (CMD_CONCAT(0xD3, 0x80, 0x0001))
/*! Definition of TCU_LE_GATT_SER_WRITE_CHAR_DESP_EVENT */
#define TCU_LE_GATT_SER_WRITE_CHAR_DESP_EVENT (CMD_CONCAT(0xD3, 0xC4, LEN_VAR))
/*! Definition of TCU_LE_GATT_SER_WRITE_CHAR_DESP_ACCEPT_REQ */
#define TCU_LE_GATT_SER_WRITE_CHAR_DESP_ACCEPT_REQ (CMD_CONCAT(0xD3, 0x04, 0x0005))
/*! Definition of TCU_LE_GATT_SER_WRITE_CHAR_DESP_ACCEPT_RESP */
#define TCU_LE_GATT_SER_WRITE_CHAR_DESP_ACCEPT_RESP (CMD_CONCAT(0xD3, 0x84, 0x0003))
/*! Definition of TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_REQ */
#define TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_REQ (CMD_CONCAT(0xD3, 0x02, 0x0005))
/*! Definition of TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_RESP */
#define TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_RESP (CMD_CONCAT(0xD3, 0x82, 0x0003))
/*! Definition of TCU_LE_GATT_SER_EXG_MTU_EVENT */
#define TCU_LE_GATT_SER_EXG_MTU_EVENT (CMD_CONCAT(0xD3, 0xC1, 0x0004))
/*! Definition of TCU_LE_GATT_SER_EXG_MTU_ACCEPT_REQ */
#define TCU_LE_GATT_SER_EXG_MTU_ACCEPT_REQ (CMD_CONCAT(0xD3, 0x01, 0x0005))
/*! Definition of TCU_LE_GATT_SER_EXG_MTU_ACCEPT_RESP */
#define TCU_LE_GATT_SER_EXG_MTU_ACCEPT_RESP (CMD_CONCAT(0xD3, 0x81, LEN_VAR))
/*! Definition of TCU_LE_GATT_SER_READ_CHAR_VAL_EVENT */
#define TCU_LE_GATT_SER_READ_CHAR_VAL_EVENT (CMD_CONCAT(0xD3, 0xC2, 0x0004))
/*! Definition of TCU_LE_GATT_SER_WRITE_CHAR_VAL_EVENT */
#define TCU_LE_GATT_SER_WRITE_CHAR_VAL_EVENT (CMD_CONCAT(0xD3, 0xC3, LEN_VAR))
/*! Definition of TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_REQ */
#define TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_REQ (CMD_CONCAT(0xD3, 0x03, 0x0005))
/*! Definition of TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_RESP */
#define TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_RESP (CMD_CONCAT(0xD3, 0x83, 0x0003))
/*! Definition of TCU_LE_GATT_SER_READ_CHAR_DESP_EVENT */
#define TCU_LE_GATT_SER_READ_CHAR_DESP_EVENT (CMD_CONCAT(0xD3, 0xC8, 0x0004))
/*! Definition of TCU_LE_GATT_SER_READ_CHAR_DESP_ACCEPT_REQ */
#define TCU_LE_GATT_SER_READ_CHAR_DESP_ACCEPT_REQ (CMD_CONCAT(0xD3, 0x08, 0x0005))
/*! Definition of TCU_LE_GATT_SER_READ_CHAR_DESP_ACCEPT_RESP */
#define TCU_LE_GATT_SER_READ_CHAR_DESP_ACCEPT_RESP (CMD_CONCAT(0xD3, 0x88, 0x0003))
/*! Definition of TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_REQ */
#define TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_REQ (CMD_CONCAT(0xD3, 0x03, 0x0005))
/*! Definition of TCU_LE_GATT_SER_READ_MULTIPLE_EVENT */
#define TCU_LE_GATT_SER_READ_MULTIPLE_EVENT (CMD_CONCAT(0xD3, 0xCA, LEN_VAR))
/*! Definition of TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_REQ */
#define TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_REQ (CMD_CONCAT(0xD3, 0x0A, 0x0005))
/*! Definition of TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_RESP */
#define TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_RESP (CMD_CONCAT(0xD3, 0x8A, 0x0003))
/*! Definition of TCU_LE_GATT_SER_CHAR_VAL_NOTIFICATION_REQ */
#define TCU_LE_GATT_SER_CHAR_VAL_NOTIFICATION_REQ (CMD_CONCAT(0xD3, 0x05, LEN_VAR))
/*! Definition of TCU_LE_GATT_SER_CHAR_VAL_NOTIFICATION_EVENT */
#define TCU_LE_GATT_SER_CHAR_VAL_NOTIFICATION_EVENT (CMD_CONCAT(0xD3, 0x45, 0x0002))
/*! Definition of TCU_LE_GATT_SER_CHAR_VAL_INDICATION_REQ */
#define TCU_LE_GATT_SER_CHAR_VAL_INDICATION_REQ (CMD_CONCAT(0xD3, 0x06, LEN_VAR))
/*! Definition of TCU_LE_GATT_SER_CHAR_VAL_INDICATION_EVENT */
#define TCU_LE_GATT_SER_CHAR_VAL_INDICATION_EVENT (CMD_CONCAT(0xD3, 0x46, 0x0003))
/*! Definition of TCU_LE_GATT_SER_WRITE_WITHOUT_RESPONSE_CMD_EVENT */
#define TCU_LE_GATT_SER_WRITE_WITHOUT_RESPONSE_CMD_EVENT (CMD_CONCAT(0xD3, 0xC9, LEN_VAR))

/**
* @brief Construct and send TCU_LE_GATT_SER_INIT_REQ.
*/
enum tcu_func_status TcuLeGattSerInit_Request(void);

/**
* @brief       Response parser. Parses TCU_LE_GATT_SER_INIT_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeGattSerInit_Response(uint8_t *data, struct tcu_le_gatt_ser_init_response *response);

/**
* @brief       Event parser. Parses TCU_LE_GATT_SER_READ_CHAR_DESP_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeGattSerReadCharDesp_Event(uint8_t *data, struct tcu_le_gatt_ser_read_char_desp_event *event);

/**
* @brief Construct and send TCU_LE_GATT_SER_READ_CHAR_DESP_ACCEPT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeGattSerReadCharDespAccept_Request(struct tcu_le_gatt_ser_read_char_desp_accept_request *request);

/**
* @brief       Response parser. Parses TCU_LE_GATT_SER_READ_CHAR_DESP_ACCEPT_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeGattSerReadCharDespAccept_Response(uint8_t *data, struct tcu_le_gatt_ser_read_char_desp_accept_response *response);

/**
* @brief       Event parser. Parses TCU_LE_GATT_SER_WRITE_CHAR_DESP_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeGattSerWriteCharDesp_Event(uint8_t *data, struct tcu_le_gatt_ser_write_char_desp_event *event);

/**
* @brief Construct and send TCU_LE_GATT_SER_WRITE_CHAR_DESP_ACCEPT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeGattSerWriteCharDespAccept_Request(struct tcu_le_gatt_ser_write_char_desp_accept_request *request);

/**
* @brief       Response parser. Parses TCU_LE_GATT_SER_WRITE_CHAR_DESP_ACCEPT_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeGattSerWriteCharDespAccept_Response(uint8_t *data, struct tcu_le_gatt_ser_write_char_desp_accept_response *response);

/**
* @brief       Event parser. Parses TCU_LE_GATT_SER_READ_CHAR_VAL_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeGattSerReadCharVal_Event(uint8_t *data, struct tcu_le_gatt_ser_read_char_val_event *event);

/**
* @brief Construct and send TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeGattSerReadCharValAccept_Request(struct tcu_le_gatt_ser_read_char_val_accept_request *request);

/**
* @brief       Response parser. Parses TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeGattSerReadCharValAccept_Response(uint8_t *data, struct tcu_le_gatt_ser_read_char_val_accept_response *response);

/**
* @brief       Event parser. Parses TCU_LE_GATT_SER_WRITE_CHAR_VAL_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeGattSerWriteCharVal_Event(uint8_t *data, struct tcu_le_gatt_ser_write_char_val_event *event);

/**
* @brief Construct and send TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeGattSerWriteCharValAccept_Request(struct tcu_le_gatt_ser_write_char_val_accept_request *request);

/**
* @brief       Response parser. Parses TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeGattSerWriteCharValAccept_Response(uint8_t *data, struct tcu_le_gatt_ser_write_char_val_accept_response *response);

/**
* @brief       Event parser. Parses TCU_LE_GATT_SER_EXG_MTU_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeGattSerExgMtu_Event(uint8_t *data, struct tcu_le_gatt_ser_exg_mtu_event *event);

/**
* @brief Construct and send TCU_LE_GATT_SER_EXG_MTU_ACCEPT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeGattSerExgMtuAccept_Request(struct tcu_le_gatt_ser_exg_mtu_accept_request *request);

/**
* @brief       Response parser. Parses TCU_LE_GATT_SER_EXG_MTU_ACCEPT_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeGattSerExgMtuAccept_Response(uint8_t *data, struct tcu_le_gatt_ser_exg_mtu_accept_response *response);

/**
* @brief       Event parser. Parses TCU_LE_GATT_SER_READ_MULTIPLE_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeGattSerReadMultiple_Event(uint8_t *data, struct tcu_le_gatt_ser_read_multiple_event *event);

/**
* @brief Construct and send TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeGattSerReadMultipleAccept_Request(struct tcu_le_gatt_ser_read_multiple_accept_request *request);

/**
* @brief       Response parser. Parses TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeGattSerReadMultipleAccept_Response(uint8_t *data, struct tcu_le_gatt_ser_read_multiple_accept_response *response);

/**
* @brief       Event parser. Parses TCU_LE_GATT_SER_WRITE_WITHOUT_RESPONSE_CMD_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeGattSerWriteWithoutResponseCmd_Event(uint8_t *data, struct tcu_le_gatt_ser_write_without_response_cmd_event *event);

/**
* @brief       Event parser. Parses TCU_LE_GATT_SER_CHAR_VAL_NOTIFICATION_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeGattSerCharValNotification_Event(uint8_t *data, struct tcu_le_gatt_ser_char_val_notification_event *event);

/**
* @brief Construct and send TCU_LE_GATT_SER_CHAR_VAL_NOTIFICATION_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeGattSerCharValNotification_Request(struct tcu_le_gatt_ser_char_val_notification_request *request);

/**
* @brief Construct and send TCU_LE_GATT_SER_CHAR_VAL_INDICATION_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeGattSerCharValIndication_Request(struct tcu_le_gatt_ser_char_val_indication_request *request);

/**
* @brief       Event parser. Parses TCU_LE_GATT_SER_CHAR_VAL_INDICATION_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuLeGattSerCharValIndication_Event(uint8_t *data, struct tcu_le_gatt_ser_char_val_indication_event *event);

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
