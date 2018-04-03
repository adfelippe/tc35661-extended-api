/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file tcu_le_gatt_sdb.h
*
* @brief Contains definitions and function declarations for the GATT server database part of the TCU API.
*
* All functions may return the following, common return codes.
*
* @return    TCU_FUNC_SUCCESS Completed succesfully
* @return    TCU_FUNC_OPCODE Op-code missmatch
* @return    TCU_FUNC_SERVICEID Sservice ID mismatch
*/
#ifndef __TCU_LE_GATT_SDB_H__
#define __TCU_LE_GATT_SDB_H__

#include "tcu_api.h"

/*! Contains TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ data */
struct tcu_le_gatt_sdb_add_prim_svc_request
{
  enum tcu_uuid_type uuid_type; /*!< uuid type */
  const uint8_t *uuid_val;      /*!< uuid value */
};

/*! Contains TCU_LE_GATT_SDB_ADD_PRIM|SEC_SVC_RESP data */
struct tcu_le_gatt_sdb_add_svc_resp
{
  enum sdb_status_code status; /*!< status code */
  uint16_t handle;             /*!< primary/secondary service handle */
};

/*! Contains TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ data */
struct tcu_le_gatt_sdb_add_char_decl_request
{
  uint16_t handle;              /*!< handle of the primary service or secondary service under which the characteristic needs to be declared */
  uint8_t properties;           /*!< property of the characteristic added to the server database. Please use gatt_char_prop_mask_e enumerations */
  enum tcu_uuid_type uuid_type; /*!< length of the UUID value of the characteristic declaration to be added to the server database */
  const uint8_t *uuid;          /*!< UUID value of the characteristic declaration to be added. This is a UUID which can either be 2 bytes or 16 bytes */
};

/*! Contains TCU_LE_GATT_SDB_ADD_CHAR_DECL|ELE_RESP data */
struct tcu_le_gatt_sdb_add_char_response
{
  enum tcu_gatt_status status; /*!< status */
  uint16_t handle;             /*!< handle */
};

/*! Contains TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ data */
struct tcu_le_gatt_sdb_add_char_ele_request
{
  uint16_t handle;              /*!< handle */
  enum tcu_uuid_type uuid_type; /*!< uuid_type */
  uint8_t uuid[LE_LONG_UUID];   /*!< uuid */
  uint16_t permissions;         /*!< please use att_permissions_e masks */
  uint16_t length;              /*!< length */
  uint8_t *value;               /*!< value */
};

/*! Contains TCU_LE_GATT_SDB_UPD_CHAR_ELE_REQ data */
struct tcu_le_gatt_sdb_upd_char_ele_request
{
  uint16_t handle; /*!<  handle of the characteristic element to be updated */
  uint16_t length; /*!< length of the characteristic element to be updated */
  uint8_t *value;  /*!< attribute value to be updated in the server database */
};

/*! Contains TCU_LE_GATT_SDB_UPD_CHAR_ELE_RESP data */
struct tcu_le_gatt_sdb_upd_char_ele_response
{
  enum tcu_gatt_status gatt_status; /*!< GATT status of element update response */
};

/*! Definition of TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ */
#define TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ (CMD_CONCAT(0xD3, 0x20, 0x0011))
/*! Definition of TCU_LE_GATT_SDB_ADD_PRIM_SVC_RESP */
#define TCU_LE_GATT_SDB_ADD_PRIM_SVC_RESP (CMD_CONCAT(0xD3, 0xA0, 0x0003))
/*! Definition of TCU_LE_GATT_SDB_ADD_SEC_SVC_REQ */
#define TCU_LE_GATT_SDB_ADD_SEC_SVC_REQ (CMD_CONCAT(0xD3, 0x21, LEN_VAR))
/*! Definition of TCU_LE_GATT_SDB_ADD_SEC_SVC_RESP */
#define TCU_LE_GATT_SDB_ADD_SEC_SVC_RESP (CMD_CONCAT(0xD3, 0xA1, LEN_VAR))
/*! Definition of TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ */
#define TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ (CMD_CONCAT(0xD3, 0x22, 0x0014))
/*! Definition of TCU_LE_GATT_SDB_ADD_CHAR_DECL_RESP */
#define TCU_LE_GATT_SDB_ADD_CHAR_DECL_RESP (CMD_CONCAT(0xD3, 0xA2, 0x0003))
/*! Definition of TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ */
#define TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ (CMD_CONCAT(0xD3, 0x23, 0xFFFE))
/*! Definition of TCU_LE_GATT_SDB_ADD_CHAR_ELE_RESP */
#define TCU_LE_GATT_SDB_ADD_CHAR_ELE_RESP (CMD_CONCAT(0xD3, 0xA3, 0x0003))
/*! Definition of TCU_LE_GATT_SDB_UPD_CHAR_ELE_REQ */
#define TCU_LE_GATT_SDB_UPD_CHAR_ELE_REQ (CMD_CONCAT(0xD3, 0x25, LEN_VAR))
/*! Definition of TCU_LE_GATT_SDB_UPD_CHAR_ELE_RESP */
#define TCU_LE_GATT_SDB_UPD_CHAR_ELE_RESP (CMD_CONCAT(0xD3, 0xA5, 0x0001))
/*! Definition of TCU_LE_GATT_SDB_ADD_INC_SVC_REQ */
#define TCU_LE_GATT_SDB_ADD_INC_SVC_REQ (CMD_CONCAT(0xD3, 0x24, LEN_VAR))
/*! Definition of TCU_LE_GATT_SDB_ADD_INC_SVC_RESP */
#define TCU_LE_GATT_SDB_ADD_INC_SVC_RESP (CMD_CONCAT(0xD3, 0xA4, LEN_VAR))
/*! Definition of TCU_LE_GATT_SDB_RET_END_GRP_HLE_REQ */
#define TCU_LE_GATT_SDB_RET_END_GRP_HLE_REQ (CMD_CONCAT(0xD3, 0x26, 0x0002))
/*! Definition of TCU_LE_GATT_SDB_RET_END_GRP_HLE_RESP */
#define TCU_LE_GATT_SDB_RET_END_GRP_HLE_RESP (CMD_CONCAT(0xD3, 0xA6, 0x0003))

/**
* @brief Construct and send TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeGattSdbAddPrimSvc_Request(struct tcu_le_gatt_sdb_add_prim_svc_request *request);

/**
* @brief       Response parser. Parses TCU_LE_GATT_SDB_ADD_PRIM_SVC_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeGattSdbAddPrimSvc_Response(uint8_t *data, struct tcu_le_gatt_sdb_add_svc_resp *response);

/**
* @brief Construct and send TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeGattSdbAddCharDecl_Request(struct tcu_le_gatt_sdb_add_char_decl_request *request);

/**
* @brief       Response parser. Parses TCU_LE_GATT_SDB_ADD_CHAR_DECL_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeGattSdbAddCharDecl_Response(uint8_t *data, struct tcu_le_gatt_sdb_add_char_response *response);

/**
* @brief Construct and send TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeGattSdbAddCharEle_Request(struct tcu_le_gatt_sdb_add_char_ele_request *request);

/**
* @brief       Response parser. Parses TCU_LE_GATT_SDB_ADD_CHAR_ELE_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeGattSdbAddCharEle_Response(uint8_t *data, struct tcu_le_gatt_sdb_add_char_response *response);

/**
* @brief Construct and send TCU_LE_GATT_SDB_UPD_CHAR_ELE_REQ.
*
* @param[in]     *request pointer to request data
*/
enum tcu_func_status TcuLeGattSdbUpdCharEle_Request(struct tcu_le_gatt_sdb_upd_char_ele_request *request);

/**
* @brief       Response parser. Parses TCU_LE_GATT_SDB_UPD_CHAR_ELE_RESP.
*
* @param[in]     *data pointer to received data
* @param[out]    *response pointer to response data structure
*/
enum tcu_func_status TcuLeGattSdbUpdCharEle_Response(uint8_t *data, struct tcu_le_gatt_sdb_upd_char_ele_response *response);

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
