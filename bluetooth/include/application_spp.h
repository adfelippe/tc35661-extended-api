/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file application_spp.h
*
* @brief Contains definitions and function declarations for handling
* the SPP-specific parts of the application.
*/
#ifndef __APPLICATION_SPP_H__
#define __APPLICATION_SPP_H__

#include <stdio.h>
#include "application.h"
//#include "event.h"

//Bluetooth SID//
#define BT_SID_GET_CURRENT_TARE                                                 3   //0x03
#define BT_SID_POWER_SUPPLY_INFORMATION                                         4   //0x04
#define BT_SID_GET_ERROR_INFORMATION                                            7   //0x07
#define BT_SID_CATTLE_COUNTER                                                   11  //0x0B
#define BT_SID_FIELD_BOX_REBOOT_SHUTDOWN                                        13  //0x0D
#define BT_SID_SYSTEM_CONFIGURATION                                             14  //0x0E
#define BT_SID_GET_LOG_INFORMATION                                              15  //0x0F
#define BT_SID_CONFIGURE_PLATFORM_SLOPE                                         16  //0x10
#define BT_SID_PLATFORM_ALIGNMENT_TOOL                                          19  //0x13
#define BT_SID_GET_LAST_WEIGHT_AND_TAG                                          20  //0x14
#define BT_SID_CONFIGURE_PEN_NUMBER                                             21  //0x15
#define BT_SID_XBEE_NETWORK_SETTINGS                                            22  //0x16
#define BREAK                                                                    0  //0x00

//Bluetooth LID//
//BT_SID_GET_CURRENT_TARE
#define BT_LID_GET_CURRENT_TARE                                                 1   //0X01
//BT_SID_POWER_SUPPLY_INFORMATION
#define BT_LID_READ_PANEL_STATUS                                                1   //0X01
#define BT_LID_READ_BATTERY_CHARGING_STATUS                                     2   //0X02
//BT_SID_GET_ERROR_INFORMATION
#define BT_LID_GET_ERROR_INFORMATION                                            0   //0X00
//BT_SID_CATTLE_COUNTER
#define BT_LID_GET_CATTLE_COUNTER                                               1   //0X01
#define BT_LID_RESET_ADC_CATTLE_COUNTER                                         2   //0X02
#define BT_LID_RESET_WEIGHT_CATTLE_COUNTER                                      3   //0X03
//BT_SID_FIELD_BOX_REBOOT_SHUTDOWN
#define BT_LID_REBOOT_SYSTEM                                                    0   //0X00
#define BT_LID_RESET_TARE_FILE                                                  1   //0X01
#define BT_LID_RESET_CONFIGURATION_FILE                                         2   //0X02
#define BT_LID_RESET_DATABASE_FILE                                              3   //0X03
#define BT_LID_FACTORY_RESET                                                    4   //0X04
#define BT_LID_RUN_FILESYSTEM_CORRUPTION_CHECK                                  5   //0X05
#define BT_LID_SHUTDOWN_SYSTEM                                                  6   //0X06
#define BT_LID_FORMAT_SD_CARD                                                   7   //0X07
//BT_SID_SYSTEM_CONFIGURATION
#define BT_LID_GET_FIELD_BOX_CONFIGURATION                                      0   //0X00
#define BT_LID_SET_FIELD_BOX_CONFIGURATION                                      1   //0X01
#define BT_LID_GET_RFID_READER_CONFIGURATION                                    2   //0X02
#define BT_LID_SET_RFID_READER_CONFIGURATION                                    3   //0X03
//BT_SID_GET_LOG_INFORMATION
#define BT_LID_GET_LOG_INFORMATION                                              0   //0X00
//BT_SID_CONFIGURE_PLATFORM_SLOPE
#define BT_LID_ENTER_CALIBRATION_MODE                                           0   //0X00
#define BT_LID_GET_SLOPE                                                        1   //0X01
#define BT_LID_SET_SLOPE                                                        2   //0X02
//BT_SID_PLATFORM_ALIGNMENT_TOOL
#define BT_LID_PLATFORM_ALIGNMENT_TOOL                                          1   //0X01
//BT_SID_GET_LAST_WEIGHT_AND_TAG
#define BT_LID_GET_LAST_WEIGHT_AND_TAG                                          1   //0X01
//BT_SID_CONFIGURE_PEN_NUMBER
#define BT_LID_SET_PEN_NUMBER                                                   0   //0X00
#define BT_LID_READ_PEN_NUMBER                                                  1   //0X01
//BT_SID_XBEE_NETWORK_SETTINGS
#define BT_LID_READ_MAC_ADDRESS                                                 0   //0X00
#define BT_LID_START_NETWORK_DEPLOYMENT                                         1   //0X01
#define BT_LID_GET_STATUS                                                       2   //0X02
//BREAK

/**
* @brief Initialise the SPP application.
*
* @return 0 if success, -1 if error
*/
int application_spp_setup(void);

/**
* @brief Event handler for application-specific SPP events.
*
* @param[in] arg Raw application-specific argument
*
* @return 0 if success, -1 if error
*/
int application_spp_event_handler(uint32_t arg);

/**
* @brief Event handler for SPP TCU events.
*
* @param[in] data Pointer to raw TCU event
*
* @return 0 if success, -1 if error
*/
int application_spp_tcu_event_handler(uint8_t *data);

/**
* @brief Set bluetooth device name from XBEE Mac Address
*/
void set_bt_device_name(void);
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
