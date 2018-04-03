/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file tcu_spp.h
*
* @brief Contains definitions and function declarations for the classic SPP-profile part of the TCU API.
*
* All functions may return the following, common return codes.
*
* @return    TCU_FUNC_SUCCESS Completed succesfully
* @return    TCU_FUNC_OPCODE Op-code missmatch
* @return    TCU_FUNC_SERVICEID Sservice ID mismatch
*/
#ifndef __TCU_SPP_H__
#define __TCU_SPP_H__

#include "tcu_api.h"
#include "tcu_mng.h"

/*! Definition of TCU_SPP_SETUP_REQ */
#define TCU_SPP_SETUP_REQ (CMD_CONCAT(0xe5, 0x01, 0x0000))
/*! Definition of TCU_SPP_SETUP_RESP */
#define TCU_SPP_SETUP_RESP (CMD_CONCAT(0xe5, 0x81, 0x0001))
/*! Definition of TCU_SPP_SHUTDOWN_REQ */
#define TCU_SPP_SHUTDOWN_REQ (CMD_CONCAT(0xe5, 0x02, 0x0000))
/*! Definition of TCU_SPP_SHUTDOWN_RESP */
#define TCU_SPP_SHUTDOWN_RESP (CMD_CONCAT(0xe5, 0x82, 0x0001))
/*! Definition of TCU_SPP_CONNECT_REQ */
#define TCU_SPP_CONNECT_REQ (CMD_CONCAT(0xe5, 0x03, LEN_VAR))
/*! Definition of TCU_SPP_CONNECT_EVENT */
#define TCU_SPP_CONNECT_EVENT (CMD_CONCAT(0xe5, 0x43, LEN_VAR))
/*! Definition of TCU_SPP_DISCONNECT_REQ */
#define TCU_SPP_DISCONNECT_REQ (CMD_CONCAT(0xe5, 0x04, 0x0000))
/*! Definition of TCU_SPP_DISCONNECT_EVENT */
#define TCU_SPP_DISCONNECT_EVENT (CMD_CONCAT(0xe5, 0x44, 0x0008))
/*! Definition of TCU_SPP_LINE_NOTIFY_EVENT */
#define TCU_SPP_LINE_NOTIFY_EVENT (CMD_CONCAT(0xe5, 0x47, 0x0001))
/*! Definition of TCU_SPP_DATA_TRANSFER_REQ */
#define TCU_SPP_DATA_TRANSFER_REQ (CMD_CONCAT(0xe5, 0x08, LEN_VAR))
/*! Definition of TCU_SPP_DATA_RECEIVE_EVENT */
#define TCU_SPP_DATA_RECEIVE_EVENT (CMD_CONCAT(0xe5, 0x48, LEN_VAR))
/*! Definition of TCU_SPP_DATA_SEND_EVENT */
#define TCU_SPP_DATA_SEND_EVENT (CMD_CONCAT(0xe5, 0xF1, 0x0000))
/*! Definition of TCU_SPP_UUID_ASSIGN_REQ */
#define TCU_SPP_UUID_ASSIGN_REQ (CMD_CONCAT(0xe5, 0x20, LEN_VAR))
/*! Definition of TCU_SPP_UUID_ASSIGN_RESP */
#define TCU_SPP_UUID_ASSIGN_RESP (CMD_CONCAT(0xe5, 0xA0, 0x0001))

/*! TCU SPP service UUID data type enumeration */
enum tcu_spp_uuid_data_type {
  TCU_SSP_UUID16 = 0x19, /*!< 16bit data */
  TCU_SSP_UUID32 = 0x1A, /*!< 32bit data */
  TCU_SSP_UUID128 = 0x1C /*!< 128bit data */
};

#ifdef __GNUC__
/* gcc will emit "warning: type of bit-field ‘xxx’ is a GCC extension" for the structures below */
#pragma GCC diagnostic ignored "-pedantic"
#endif

/*! Contains data format portion of TCU_SPP_CONNECT_REQ */
union tcu_spp_connection_data_format
{
  struct
  {
    uint8_t unused1 : 1;
    uint8_t data_bit : 2;
    uint8_t stop_bit : 1;
    uint8_t parity : 1;
    uint8_t parity_type : 2;
    uint8_t unused2 : 1;
  } format;     /*!< format */
  uint8_t byte; /*!< byte representation of format */
};

/*! Contains flow control portion of TCU_SPP_CONNECT_REQ */
union tcu_spp_connection_flow_control
{
  struct
  {
    uint8_t no_flow_control : 1;
    uint8_t x_flow_input : 1;
    uint8_t x_flow_output : 1;
    uint8_t rtr_input : 1;
    uint8_t rtr_output : 1;
    uint8_t rtc_input : 1;
    uint8_t rtc_output : 1;
    uint8_t unused : 1;
  } flow_control; /*!< flow control */
  uint8_t byte;   /*!< byte representation of flow control */
};

/*! Contains connection parameter portion of TCU_SPP_CONNECT_REQ */
union tcu_spp_connection_parameter
{
  struct
  {
    uint8_t RFCOMM_RPN_MASK_BAUD : 1;
    uint8_t RFCOMM_RPN_MASK_DATA : 1;
    uint8_t RFCOMM_RPN_MASK_STOP : 1;
    uint8_t RFCOMM_RPN_MASK_PARITY : 1;
    uint8_t RFCOMM_RPN_MASK_PARITY_TYPE : 1;
    uint8_t RFCOMM_RPN_MASK_XON_CHAR : 1;
    uint8_t RFCOMM_RPN_MASK_XOFF_CHAR : 1;
    uint8_t Unused1 : 1;
    uint8_t RFCOMM_RPN_MASK_FLOW_X_IN : 1;
    uint8_t RFCOMM_RPN_MASK_FLOW_X_OUT : 1;
    uint8_t RFCOMM_RPN_MASK_FLOW_RTR_IN : 1;
    uint8_t RFCOMM_RPN_MASK_FLOW_RTR_OUT : 1;
    uint8_t RFCOMM_RPN_MASK_FLOW_RTC_IN : 1;
    uint8_t RFCOMM_RPN_MASK_FLOW_RTC_OUT : 1;
    uint8_t Unused2 : 2;
  } parameter;   /*!< parameter */
  uint16_t byte; /*!< byte representation of parameter */
};

/*! TCU SPP connection baud rate enumeration */
enum tcu_spp_baud_rate {
  TCU_SPP_BAUD_2400bps = 0x00,   /*!< 2400bps */
  TCU_SPP_BAUD_4800bps = 0x01,   /*!< 4800bps */
  TCU_SPP_BAUD_7200bps = 0x02,   /*!< 7200bps */
  TCU_SPP_BAUD_9600bps = 0x03,   /*!< 9600bps */
  TCU_SPP_BAUD_19200bps = 0x04,  /*!< 19200bps */
  TCU_SPP_BAUD_38400bps = 0x05,  /*!< 38400bps */
  TCU_SPP_BAUD_57600bps = 0x06,  /*!< 57600bps */
  TCU_SPP_BAUD_115200bps = 0x07, /*!< 115200bps */
  TCU_SPP_BAUD_230400bps = 0x08  /*!< 230400bps */
};

/*! Contains TCU_SPP_SETUP_RESPONSE data */
struct tcu_spp_setup_response
{
  enum tcu_mng_status status; /*!< status */
};

/*! Contains TCU_SPP_SHUTDOWN_RESPONSE data */
struct tcu_spp_shutdown_response
{
  enum tcu_mng_status status; /*!< status */
};

/*! Contains TCU_SPP_UUID_ASSIGN_REQ data */
struct tcu_uuid_assign_request
{
  enum tcu_spp_uuid_data_type initiator_uuid_data_type; /*!< initiator data type */
  uint8_t initiator_uuid_value[TCU_SSP_UUID128];        /*!< initiator data value */
  enum tcu_spp_uuid_data_type acceptor_uuid_data_type;  /*!< acceptor data type */
  uint8_t acceptor_uuid_value[TCU_SSP_UUID128];         /*!< acceptor data value */
};

/*! Contains TCU_SPP_UUID_ASSIGN_RESP data */
struct tcu_spp_uuid_assign_response
{
  enum tcu_mng_status status; /*!< status */
};

/*! Contains TCU_SPP_CONNECT_REQ data */
struct tcu_spp_connect_request
{
  uint8_t bd_addr[TCU_SIZE_BDADDR];                   /*!< Bluetooth device address */
  enum tcu_spp_baud_rate baud_rate;                   /*!< baud rate */
  union tcu_spp_connection_data_format data_format;   /*!< data format */
  union tcu_spp_connection_flow_control flow_control; /*!< flow control */
  uint8_t x_on_char;                                  /*!< x on character */
  uint8_t x_off_char;                                 /*!< x off character */
  union tcu_spp_connection_parameter parameter;       /*!< connection parameter */
  uint8_t server_chanel_validity;                     /*!< is server channel valid? */
  uint8_t server_channel;                             /*!< server channel to use */
  uint8_t use_of_link_key;                            /*!< use link key? */
  uint8_t link_key[TCU_SIZE_LINKKEY];                 /*!< link key to use */
};

/*! Contains TCU_SPP_CONNECT_EVENT data */
struct tcu_spp_connect_event
{
  enum tcu_mng_status status;                /*!< status */
  uint8_t bd_addr[TCU_SIZE_BDADDR];          /*!< Bluetooth device address */
  uint8_t negotiated_frame_size_lower;       /*!< lower negotiated frame size */
  uint8_t negotiated_frame_size_upper;       /*!< upper negotiated frame size */
  uint8_t length_of_device_name;             /*!< length of device name */
  uint8_t device_name[TCU_SIZE_DEVICE_NAME]; /*!< device name */
};

/*! TCU SPP disconnection reason enumeration */
enum tcu_spp_disconnect_reason {
  TCU_SPP_ReleasingRequiredFromLocalHost = 0x01,
  TCU_SPP_ReleasingRequiredFromRemoteDevice = 0x02,
  TCU_SPP_DisconnectionError = 0x03,
  TCU_SPP_LinkLoss = 0x04
};

/*! Contains TCU_SPP_DISCONNECT_EVENT data */
struct tcu_spp_disconnect_event
{
  enum tcu_mng_status status;            /*!< status */
  uint8_t bd_addr[TCU_SIZE_BDADDR];      /*!< Bluetooth device address */
  enum tcu_spp_disconnect_reason reason; /*!< disconnect reason */
};

/*! Contains TCU_SPP_LINE_NOTIFY_EVENT data */
struct tcu_spp_line_notify_event
{
  enum tcu_mng_status status; /*!< status */
  uint8_t line_status;        /*!< line status */
};

/*! Contains TCU_SPP_DATA_RECEIVE_EVENT data */
struct tcu_spp_data_receive_event
{
  uint16_t length_of_data; /*!< length of data */
  uint8_t *data;           /*!< data */
};

/*! Contains TCU_SPP_DATA_TRANSFER_REQUEST data */
struct tcu_spp_data_transfer_request
{
  uint16_t length_of_data; /*!< length of data */
  uint8_t *data;           /*!< data */
};

/**
* @brief Construct and send TCU_SPP_SETUP_REQ.
*/
enum tcu_func_status TcuSppSetup_Request(void);

/**
* @brief       Response parser. Parses TCU_SPP_SETUP_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuSppSetup_Response(uint8_t *data, struct tcu_spp_setup_response *response);

/**
* @brief Construct and send TCU_SPP_SHUTDOWN_REQ.
*/
enum tcu_func_status TcuSppShutdown_Request(void);

/**
* @brief       Response parser. Parses TCU_SPP_SHUTDOWN_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuSppShutdown_Response(uint8_t *data, struct tcu_spp_shutdown_response *response);

/**
* @brief Construct and send TCU_SPP_CONNECT_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuSppConnect_Request(struct tcu_spp_connect_request *request);

/**
* @brief       Event parser. Parses TCU_SPP_CONNECT_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuSppConnect_Event(uint8_t *data, struct tcu_spp_connect_event *event);

/**
* @brief Construct and send TCU_SPP_DISCONNECT_REQ.
*/
enum tcu_func_status TcuSppDisconnect_Request(void);

/**
* @brief       Event parser. Parses TCU_SPP_DISCONNECT_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuSppDisconnect_Event(uint8_t *data, struct tcu_spp_disconnect_event *event);

/**
* @brief Construct and send TCU_SPP_UUID_ASSIGN_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuSppUuidAssign_Request(struct tcu_uuid_assign_request *request);

/**
* @brief       Response parser. Parses TCU_SPP_UUID_ASSIGN_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuSppUuidAssign_Response(uint8_t *data, struct tcu_spp_uuid_assign_response *response);

/**
* @brief Construct and send TCU_SPP_DATA_TRANSFER_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuSppDataTransfer_Request(struct tcu_spp_data_transfer_request *request);

/**
* @brief       Event parser. Parses TCU_SPP_DATA_SENT_EVENT.
*
* @param[in]     *data pointer to received data
*/
enum tcu_func_status TcuSppDataSent_Event(uint8_t *data);

/**
* @brief       Event parser. Parses TCU_SPP_DATA_RECEIVE_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuSppDataReceive_Event(uint8_t *data, struct tcu_spp_data_receive_event *event);

/**
* @brief       Event parser. Parses TCU_SPP_LINE_NOTIFY_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to response data structure
*/
enum tcu_func_status TcuSppLineNotify_Event(uint8_t *data, struct tcu_spp_line_notify_event *event);

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
