/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file key_store_ble_api.h
*
* @brief Contains definitions and function declarations for the key storage API
* which is used by the BLE managagement (MNG) module to handle
* everything related to storing security informations.
*/
#ifndef __KEY_STORE_BLE_API_H__
#define __KEY_STORE_BLE_API_H__

#include <stdint.h>
#include "tcu_le_smp_slv.h"

/**
* @brief Store a BLE security storage entry for a certain address.
*
* @param[in] *bd_address Bluetooth address storage entry belongs to
* @param[in] address_type Address type of the Bluetooth device address
* @param[in] *request Pointer to data with security informations
*
* @return 0 Success
* @return -1 Error
* @return -2 Out of storage memory
*/
extern int key_store_ble_store_info(uint8_t *bd_address, enum tcu_smp_address_type address_type, struct tcu_le_smp_slv_key_accept_request *request);

/**
* @brief Delete BLE security information belonging to a certain address.
*
* @param[in] *bd_address Bluetooth address
* @param[in] address_type Address type of the Bluetooth device address
*
* @return 0 Success
* @return -1 Error
* @return -2 No security information for this address found
*/
extern int key_store_ble_delete_info(uint8_t *bd_address, enum tcu_smp_address_type address_type);

/**
* @brief Retrieve BLE security information belonging to a certain address.
*
* @param[in] *bd_address Bluetooth address
* @param[in] address_type Address type of the Bluetooth device address
* @param[in] *request Pointer to data where security information can be put to
*
* @return 0 Success
* @return -1 Error
* @return -2 No security information for this address found
*/
extern int key_store_ble_get_info(uint8_t *bd_address, enum tcu_smp_address_type address_type, struct tcu_le_smp_slv_key_accept_request *request);

/**
* @brief Iterate over all available IRKs to resolve public random address.
*
* The iterator must be 0 on first call and not be changed by the application for subsequent calls.
* The implementation *must* make sure that the iterator is set in such a way (ie. incremented)
* that for subsequent calls always the next IRK is retrieved.
*
* @param[in] *iterator Pointer to internally used iterator.
* @param[in] *irk Pointer to data where IRK can be stored.
*
* @return 0 Success, IRK is returned
* @return -1 No further IRK available
*/
extern int key_store_ble_get_irk_iterator(int *iterator, uint8_t *irk);

/**
* @brief Update a BLE security storage entry specified by iterator.
*
* This function updates the BLE security storage entry specified by the iterator to use the
* new Bluetooth device address and the new IRK.
*
* @param[in] iterator Iterator selecting a BLE security storage entry
* @param[in] *bd_address New Bluetooth device address
* @param[in] *irk Pointer to data with security informations
*
* @return 0 Success
* @return -1 Error
*/
extern int key_store_ble_store_info_based_on_resolved_irk(int iterator, uint8_t *bd_address, uint8_t *irk);

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
