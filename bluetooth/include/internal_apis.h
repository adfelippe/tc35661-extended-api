/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file internal_apis.h
*
* @brief Contains definitions and function declarations for internal APIs that
* are used across the application.
*/
#ifndef __INTERNAL_APIS_H__
#define __INTERNAL_APIS_H__

// rkdev: #include "rtc_api.h"
#include "nvm_api.h"
#include "tcu_api.h"
#include "tcu_le_smp_slv.h"

/*! Contains a single index element for BT classic */
struct key_store_bt_index_element
{
  uint8_t crc8;                        /*!< crc */
  uint8_t in_use;                      /*!< is this entry in use? */
  uint8_t bd_address[TCU_SIZE_BDADDR]; /*!< Bluetooth device address this entry belongs to */
};

/*! Contains a single storage element */
struct key_store_bt_element
{
  uint8_t crc8;                       /*!< crc */
  uint8_t link_key[TCU_SIZE_LINKKEY]; /*!< link key */
};

/**
* @brief Initialises the Bluetooth Classic key storage subsystem.
*
* @return 0 Subsystem initialized successfully
* @return -1 Subsystem not initialized successfully
*/
int key_store_bt_init(void);

/**
* @brief Deletes all keys in the Bluetooth Classic key storage subsystem.
*
* @return 0 All keys deleted
* @return -1 Some error occured
*/
int key_store_bt_reset(void);

/*! Contains a single index element for BLE */
struct key_index_element
{
  uint8_t crc8;                              /*!< crc */
  uint8_t in_use;                            /*!< is this entry in use? */
  enum tcu_smp_address_type bd_address_type; /*!< Bluetooth address type */
  uint8_t bd_address[TCU_SIZE_BDADDR];       /*!< Bluetooth device address this entry belongs to */
};

/*! Contains a single storage element */
struct key_store_element
{
  uint8_t crc8;                                  /*!< crc */
  struct tcu_le_smp_slv_key_accept_request data; /*!< security information */
};

/**
* @brief Initialises the BLE key storage subsystem.
*
* @return 0 Subsystem initialized successfully
* @return -1 Subsystem not initialized successfully
*/
int key_store_init(void);

/**
* @brief Reset all keys in the the BLE key storage subsystem.
*
* @return 0 Success
* @return -1 Failure
*/
int key_store_reset(void);

/**
* @brief Initializes the timer subsystem.
*
*/
int timer_init(void);

/**
* @brief Initializes the RTC subsystem.
*
*/
int rtc_init(void);

/*! Contains informations about one partition */
struct nvm_partition
{
  enum nvm_partition_type type; /*!< type of partition */
  uint32_t len;                 /*!< length of partition */
  uint32_t offset;              /*!< offset */
};

/**
* @brief Set the current time.
*
* @param[in] time Pointer to time to be set
*
* @return 0 Success
* @return -1 Error
*/
// rkdev: extern int rtc_set(struct rtc_time *time);

/*! Global partition array. Must be defined by the application.
*
* This is an array of pointers to struct nvm_partitions. The very last entry
* must be a NULL pointer, because the subsystem must be able to recognize
* the last entry reliably.
*
* In each struct nvm_partition the very last entry must be an empty entry
* of type 'nvm_partition_end', because the subsystem must be able to recognize
* the last entry reliably.
*
* Usually each partition type should only be available once.
*
* The very last entry used (usually of type 'nvm_application' may have a
* length of 0. In this case, the remaining space of the NVM will be set as the
* length automatically.
*/
extern struct nvm_partition *const nvm_partitions[];

/**
* @brief Initializes the NVM subsystem.
*
* @param[in] nvm_size Total size of NVM storage
*/
int nvm_init(int nvm_size);

/**
* @brief Read a measurement from an attached thermometer.
*
* @return Temperature value as expected from the health thermometer API.
*/
uint32_t health_thermometer_measurement_read(void);

/**
* @brief Callback function that is called when a hardware-specific timer
* has been expired.
*
* @param[in] timeout timer-specific timeout value of the timer that expired
*/
void timer_callback(uint32_t timeout);

int handle_timer(void);

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
