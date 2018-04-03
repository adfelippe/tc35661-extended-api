/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file bt_api_ven.h
*
* @brief Contains definitions and function declarations for the SPP profile-specific parts of the Bluetooth Classic API.
*/
#ifndef __BT_API_VEN_H__
#define __BT_API_VEN_H__

#include "bt_api.h"
#include "tcu_ven.h"

/**
* @brief Read data from EEPROM attached to TC356xx.
*
* @param[in] device_address Device address
* @param[in] device_address_bit_length Bit-length of device address
* @param[in] address Address inside EEPROM
* @param[out] *data Pointer to memory where data is read to
* @param[in] length Length of data to be read
*
* @return BT_RESULT_SUCCESS Success
* @return BT_RESULT_ERROR Unspecified error
*/
enum bt_api_result bt_api_ven_eeprom_read(uint8_t device_address, uint8_t device_address_bit_length, uint16_t address, uint8_t *data, uint16_t length);

/**
* @brief Write data to EEPROM attached to TC356xx.
*
* @param[in] device_address Device address
* @param[in] device_address_bit_length Bit-length of device address
* @param[in] address Address inside EEPROM
* @param[out] *data Pointer to memory with data to be written
* @param[in] length Length of data to be written
*
* @return BT_RESULT_SUCCESS Success
* @return BT_RESULT_ERROR Unspecified error
*/
enum bt_api_result bt_api_ven_eeprom_write(uint8_t device_address, uint8_t device_address_bit_length, uint16_t address, uint8_t *data, uint16_t length);

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
