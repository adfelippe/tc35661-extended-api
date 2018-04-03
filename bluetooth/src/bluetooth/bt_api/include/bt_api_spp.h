/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file bt_api_spp.h
*
* @brief Contains definitions and function declarations for the SPP profile part of the BT API.
*/
#ifndef __BT_SPP_API_H__
#define __BT_SPP_API_H__

#include "bt_api.h"
#include "tcu_spp.h"

/*! Structure definition for a SPP UUID value */
struct bt_api_spp_uuid
{
  uint8_t uuid_length; /*!< 2, 4 or 16 bytes */
  uint8_t uuid[16];    /*!< the UUID value */
};

/**
* @brief Initialises the Bluetooth SPP profile functionality
*/
enum bt_api_result bt_api_spp_init(void);

/**
* @brief Changes the initiator and acceptor SPP UUID values to custom settings.
*
* @param[in] *initiator pointer to structure containing UUID initiator settings
* @param[in] *acceptor pointer to structure containing UUID acceptor settings
*/
enum bt_api_result bt_api_spp_uuid_assign(struct bt_api_spp_uuid *initiator, struct bt_api_spp_uuid *acceptor);

/**
* @brief Connects to a SPP server on a specific channel.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
* @param[in] server_channel Bluetooth server channel of remote device
*/
enum bt_api_result bt_api_spp_connect(uint8_t *bd_addr, uint8_t server_channel);

/**
* @brief Disconnects from the currently connected SPP server.
*/
enum bt_api_result bt_api_spp_disconnect(void);

/**
* @brief Sends data to the currently connected SPP server.
*
* After this function has been called, bt_application_spp_data_sent_callback() must be
* awaited.
*
* @param[in] *buf Pointer to buffer that contains data to be sent
* @param[in] length Length of data to be sent
*/
enum bt_api_result bt_api_spp_data_transfer(uint8_t *buf, uint16_t length);

/**
* @brief Callback function to signal that data has sent successfully.
*
* After this callback has been received, bt_api_spp_data_transfer() may be
* called again.
*/
extern void bt_application_spp_data_sent_callback(void);

/**
* @brief Callback function to signal that data has been received.
*
* @param[in] *buf Pointer to buffer that contains data to be sent
* @param[in] length Length of data to be sent
*/
extern void bt_application_spp_data_received_callback(uint8_t *buf, int length);

/**
* @brief Callback function to signal that SPP connection has been established.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
*/
extern void bt_application_spp_connect_callback(uint8_t *bd_addr);

/**
* @brief Callback function to signal that SPP connection has been terminated.
*/
extern void bt_application_spp_disconnect_callback(void);

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
