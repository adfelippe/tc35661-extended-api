/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file tcu_ven.h
*
* @brief Contains definitions and function declarations for the classic vendor-specific (VEN) part of the TCU API.
*
* All functions may return the following, common return codes.
*
* @return    TCU_FUNC_SUCCESS Completed succesfully
* @return    TCU_FUNC_OPCODE Op-code missmatch
* @return    TCU_FUNC_SERVICEID Sservice ID mismatch
*/
#ifndef __TCU_VEN_H__
#define __TCU_VEN_H__

#include "tcu_api.h"
#include "tcu_mng.h"

/*! Definition of TCU_VEN_GET_MODULE_MAINTENANCE_REQ */
#define TCU_VEN_GET_MODULE_MAINTENANCE_REQ (CMD_CONCAT(0xEF, 0x02, LEN_VAR))
/*! Definition of TCU_VEN_GET_MODULE_MAINTENANCE_EVENT */
#define TCU_VEN_GET_MODULE_MAINTENANCE_EVENT (CMD_CONCAT(0xEF, 0x42, LEN_VAR))
/*! Definition of TCU_VEN_SET_MODULE_MAINTENANCE_REQ */
#define TCU_VEN_SET_MODULE_MAINTENANCE_REQ (CMD_CONCAT(0xEF, 0x01, LEN_VAR))
/*! Definition of TCU_VEN_SET_MODULE_MAINTENANCE_EVENT */
#define TCU_VEN_SET_MODULE_MAINTENANCE_EVENT (CMD_CONCAT(0xEF, 0x41, LEN_VAR))
/*! Definition of TCU_VEN_SET_HOST_WAKEUP_NOTIFICATION_REQ */
#define TCU_VEN_SET_HOST_WAKEUP_NOTIFICATION_REQ (CMD_CONCAT(0xEF, 0x0F, 0x0001))
/*! Definition of TCU_VEN_SET_HOST_WAKEUP_RESP */
#define TCU_VEN_SET_HOST_WAKEUP_RESP (CMD_CONCAT(0xEF, 0x8F, 0x0001))

void tcu_application_log_event(uint8_t *buf, const char *name, const char *format, ...);
void tcu_application_log_request(uint8_t *buf, const char *name, const char *format, ...);
void tcu_application_log_response(uint8_t *buf, const char *name, const char *format, ...);

/*! Contains TCU_VEN_GET_MODULE_MAINTENANCE_EVENT */
struct tcu_ven_get_module_maintenance_event
{
  uint8_t acceptor;         /*!< acceptor */
  uint8_t information_id;   /*!< information id */
  uint8_t result;           /*!< result */
  uint8_t information_type; /*!< information type */
  uint8_t *data;            /*!< data */
  uint8_t length;           /*!< length */
};

/*! Contains TCU_VEN_SET_MODULE_MAINTENANCE_EVENT */
struct tcu_ven_set_module_maintenance_event
{
  uint8_t acceptor;         /*!< acceptor */
  uint8_t information_id;   /*!< information id */
  uint8_t result;           /*!< result  */
  uint8_t information_type; /*!< information type */
};

/*! Contains TCU_VEN_SET_HOST_WAKEUP_RESP */
struct tcu_ven_set_host_wakeup_notification_response
{
  enum tcu_mng_status status; /*!< status */
};

/*! Module maintenance data type enumeration */
enum tcu_ven_xet_module_maintenance_data_type {
  DATA_TYPE_ARRAY = 0x10 /*!< data type is of array type */
};

/*! Module maintenance command enumeration */
enum tcu_ven_xet_module_maintenance_command {
  COMMAND_READ_EEPROM = 0x88, /*!< read eeprom */
  COMMAND_WRITE_EEPROM = 0x89 /*!< write eeprom */
};

/*! Contains TCU_VEN_GET_MODULE_MAINTENANCE_REQ */
struct tcu_ven_get_module_maintenance_request
{
  enum tcu_ven_xet_module_maintenance_command command;     /*!< command */
  enum tcu_ven_xet_module_maintenance_data_type data_type; /*!< data type */
  uint8_t *request_buf;                                    /*!< request */
  uint8_t request_len;                                     /*!< length of request */
};

/**
* @brief Construct and send TCU_VEN_GET_MODULE_MAINTENANCE_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuVenGetModuleMaintenance_Request(struct tcu_ven_get_module_maintenance_request *request);

/*! Contains TCU_VEN_SET_MODULE_MAINTENANCE_REQ */
struct tcu_ven_set_module_maintenance_request
{
  enum tcu_ven_xet_module_maintenance_command command;     /*!< command */
  enum tcu_ven_xet_module_maintenance_data_type data_type; /*!< data type */
  uint8_t *request_buf;                                    /*!< request */
  uint8_t request_len;                                     /*!< length of request */
  uint8_t *data_buf;                                       /*!< data to be set */
  uint8_t data_len;                                        /*!< length of data to be set */
};

/**
* @brief Construct and send TCU_VEN_SET_MODULE_MAINTENANCE_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuVenSetModuleMaintenance_Request(struct tcu_ven_set_module_maintenance_request *request);

/**
* @brief       Event parser. Parses TCU_VEN_GET_MODULE_MAINTENANCE_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuVenGetModuleMaintenenace_Event(uint8_t *data, struct tcu_ven_get_module_maintenance_event *event);

/**
* @brief       Event parser. Parses TCU_VEN_SET_MODULE_MAINTENANCE_EVENT.
*
* @param[in]     *data pointer to received data
* @param[out]    *event pointer to event data structure
*/
enum tcu_func_status TcuVenSetModuleMaintenenace_Event(uint8_t *data, struct tcu_ven_set_module_maintenance_event *event);

/*! Host wakeup notification control mode enumeration */
enum tcu_ven_host_wakeup_notification_control_mode {
  HOST_WAKEUP_MODE_DISABLE = 0x00, /*!< disable */
  HOST_WAKEUP_MODE_ENABLE = 0x01   /*!< enable*/
};

/*! Contains TCU_VEN_SET_HOST_WAKEUP_NOTIFICATION_REQ */
struct tcu_ven_set_host_wakeup_notification_request
{
  enum tcu_ven_host_wakeup_notification_control_mode mode; /*!< mode */
};

/**
* @brief Construct and send TCU_VEN_SET_HOST_WAKEUP_NOTIFICATION_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuVenSetHostWakeupNotification_Request(struct tcu_ven_set_host_wakeup_notification_request *request);

/**
* @brief       Response parser. Parses TCU_VEN_SET_HOST_WAKEUP_NOTIFICATION_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuVenSetHostWakeupNotification_Response(uint8_t *data, struct tcu_ven_set_host_wakeup_notification_response *response);

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
