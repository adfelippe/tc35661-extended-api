/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/
/*  HCI API extended - Features to control radio settings
*   These additions are useful for anyone wishing to control PAN1026/TC35661
*   radio settings in order to go under any certitication test
*   Author: Anderson Felippe <adfelippe@gmail.com>
****************************************************************************/
/* "THE BEER-WARE LICENSE" (Revision 42):
 * <adfelippe@gmail.com> incremented this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return.
 * 							     Anderson Felippe
****************************************************************************/

/**
* @file hci_api.h
*
* @brief Contains definitions and function declarations for Bluetooth HCI API.
*/
#ifndef __HCI_API_H__
#define __HCI_API_H__

#include <stdint.h>//types.h"


/*! HCI message buffer */
extern uint8_t *hci_buffer;

/*! Length of HCI message buffer */
extern uint32_t hci_buffer_len;

/*! HCI event packet type */
#define HCI_PACKET_TYPE_EVENT_PACKET 0x04

/*! Helper macro to calculate the length of a HCI response */
#define HCI_API_RETRIEVE_HCI_RESPONSE_LENGTH(ptr) (ptr[2] + 3)

/*! Helper macro to ccheck the correct type of a HCI response */
#define HCI_API_CHECK_HCI_RESPONSE(ptr) (ptr[0] != HCI_PACKET_TYPE_EVENT_PACKET)

/*! HCI deep sleep CLKREQ signal enumeration */
enum hci_api_deep_sleep_clkreq_signal {
  hci_clkreq_work_deep_sleep = 0x00, /*!< work deep sleep */
  hci_clkreq_always_low = 0x01,      /*!< always low */
  hci_clkreq_always_high = 0x02      /*!< always high */
};

/*! HCI deep sleep CLKREQ signal enumeration */
enum hci_api_deep_sleep_notify_interface {
  hci_notify_interface_gpio = 0x00, /*!< GPIO */
  hci_notify_interface_uart = 0x01, /*!< UART */
  hci_notify_interface_usb = 0x02   /*!< USB */
};

/*! HCI UART baudrate enumeration */
enum hci_api_baudrate {
  HCI_API_BAUDRATE_115200, /*!< 115200 */
  HCI_API_BAUDRATE_230400, /*!< 230400 */
  HCI_API_BAUDRATE_460800, /*!< 460800 */
  HCI_API_BAUDRATE_921600  /*!< 921600 */
};

/*! Structure definition for HCI deep sleep configuration data */
struct hci_api_deep_sleep_data
{
  enum hci_api_deep_sleep_clkreq_signal clkreq_signal;       /*!< clkreq signal */
  enum hci_api_deep_sleep_notify_interface notify_interface; /*!< notify interface */
  uint16_t osciallator_drift;                                /*!< osciallator drift */
  uint16_t jitter_drift;                                     /*!< jitter drift */
  uint16_t sniff_attempt_wakeup_margin;                      /*!< sniff attempt wakeup margin*/
};

/**
* @brief Reset the Bluetooth HCI controller.
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_reset(void);

/**
* @brief Select the specific frequency in AFH.
*
* @param[in] channel_mask: 10-element 8-bit array to set each channel mask.
* 	     Each channel is expressed by 1 bit in each byte (channel mask).
*	     Used channel is 1, not used is 0.
* 	     Number of channels set to 1 must be 20.
*	     More or less channels set to 1 will fail to run this mode.
*	     Byte 0: Setting between channel from 0 to 7
*	     Byte 1: Setting between channel from 8 to 15
*	     Byte 2: Setting between channel from 16 to 23
*	     Byte 3: Setting between channel from 24 to 31
*	     Byte 4: Setting between channel from 32 to 39
*	     Byte 5: Setting between channel from 40 to 47
*	     Byte 6: Setting between channel from 48 to 55
*	     Byte 7: Setting between channel from 56 to 63
*	     Byte 8: Setting between channel from 64 to 71
*	     Byte 9: Setting between channel from 72 to 78
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_set_afh_host_channel_classification(uint8_t* channel_mask);

/**
* @brief Set local mode in order to permit changing RF parameters
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_loc_operation_mode_set(void);

/**
* @brief Set enable/disable Hopping sequence.
* 	 When hopping is disabled, frequency channel setting is needed at the same time.
*
* @param[in] mode: Hopping mode ENABLED (0x00) or DISABLED (0x01). Other values are set to 0x01
* @param[in] frequency: Frequency setting in case of Hopping disable. 2400MHz is set to 00.
* 	     If 1MHz is increased, then add "1" value.
* 	     Example, in case of 2440MHz, this value should be set 40 (0x28)
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_set_hopping_mode(uint8_t mode, uint8_t frequency);

/**
* @brief Set enable/disable whitening. Default value is enabled.
*
* @param[in] mode: Whitening mode ENABLED (0x00) or DISABLED (0x01). Other values are set to 0x01
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_loc_set_whitening_mode(uint8_t mode);

/**
* @brief Generate non modulated carrier. Hopping is always disabled.
*
* @param[in] frequency: Frequency setting in case of Hopping disable. 2400MHz is set to 00.
* 	     If 1MHz is increased, then add "1" value.
* 	     Example, in case of 2440MHz, this value should be set 40 (0x28)
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_non_modulated_carrier(uint8_t frequency);

/**
* @brief Set TX burst generation. Payload length should be set in advance with
* HCI_LOC_TX_SLOT_LENGTH command.
* This command operates the TX slot only. RX slot is not operated in this state
* LOCAL_OPERATION_MODE and HCI_SENDER_RECEIVE command are needed in advance.
*
* @param[in] payload: Payload content:
* 0x13: 11111111
* 0x14: 00000000
* 0x15: 0101010101
* 0x17: PRBS9
* 0x33: 00001111
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_loc_tx_mode_set(uint8_t payload);

/**
* @brief This command should be set before HCI_LOC_TX_MODE_SET command.
* This command is valid during local mode.
* HCI_LOC_OPERATION_MODE_SET command can set local mode.
* Moreover, this command is effective only when LOCAL_OPERATION mode is enabled.
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_loc_sender_receiver(void);

/**
* @brief Set Set payload length for TX burst generation.
* This command is valid during TX burst generation and local mode.
* HCI_LOC_OPERATION_MODE_SET command can set local mode.
*
* @param[in] payload_length: Payload length options:
* 0x05: DH5
* 0x03: DH3
* 0x01: DH1
* 0x25: 2-DH5
* 0x23: 2-DH3
* 0x21: 2-DH1
* 0x35: 3-DH5
* 0x33: 3-DH3
* 0x31: 3-DH1
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_loc_tx_slot_length(uint8_t payload_length);

/**
* @brief Write the Page Scan Activity configuration parameter.
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_write_page_scan_activity(void);

/**
* @brief Write the value for the Scan Enable configuration parameter.
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_write_scan_enable(void);

/**
* @brief Specify event filter for Bluetooth certification test.
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_set_event_filter_for_certif_test(void);

/**
* @brief allows the local Bluetooth module to enter test mode via LMP test commands.
*
* @return 0 Success
* @return -1 Error
*/
int hci_enable_device_under_test_mode(void);

/**
* @brief Enable RTS/CTS handling.
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_set_rtscts(void);

/**
* @brief Set Bluetooth device address in HCI controller.
*
* @param[in] bd_addr Pointer to Bluetooth device address.
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_write_bd_addr(uint8_t *bd_addr);

/**
* @brief Switch the Bluetooth controller from HCI to Toshiba TCU mode.
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_switch_to_tcu_mode(const uint8_t *version, int version_len);

/**
* @brief Enable or disable RTS/CTS control in TC3566x
*
* @param[in] enable Set to enable, otherwise disable
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_uart_rts_cts_control(int enable);

/**
* @brief Enable I2C subsystem on HCI controller.
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_set_i2_enable(void);

/**
* @brief Enables writing in I2C subsystem on HCI controller.
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_eeprom_write_enable(void);

/**
* @brief Reads data from the EEPROM attached to the HCI controller.
*
* @param[in] eeprom_device_address Device address of eeprom
* @param[in] addressing_16bit Indicates if 16bit addressing must be used
* @param[in] offset Read offset into the EEPROM
* @param[in] *buf Pointer to buffer to receive read data
* @param[in] read_length Number of bytes to read
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_read_eeprom(uint8_t eeprom_device_address, uint8_t addressing_16bit, uint16_t offset, uint8_t *buf, uint8_t read_length);

/**
* @brief Writes data to the EEPROM attached to the HCI controller.
*
* The maximum number of bytes that can be written at once is 32 bytes.
*
* @param[in] eeprom_device_address Device address of eeprom
* @param[in] addressing_16bit Indicates if 16bit addressing must be used
* @param[in] offset Write offset into the EEPROM
* @param[in] *buf Pointer to buffer to data to be written
* @param[in] write_length Number of bytes to write
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_write_eeprom(uint8_t eeprom_device_address, uint8_t addressing_16bit, uint16_t offset, uint8_t *buf, uint8_t write_length);

/**
* @brief Read out the firmware version string (zero terminated) from
* the HCI controller.
*
* The pointer is valid until the next call to the HCI API.
*
* @param[out] **version Pointer to a pointer that will point to the firmware version string
* @param[out] *version_len Pointer to an integer that will receive the length of the version string
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_read_firmware_version(const uint8_t **version, int *version_len);

/**
* @brief Send HCI deep sleep configuration command to the HCI controller.
*
* @param[in] *data Pointer to deep sleep configuration data
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_set_deep_sleep(const struct hci_api_deep_sleep_data *data);

/**
* @brief Clean up after a hardware reset of the HCI controller.
*
* When the HCI controller is reset by a hardware signal, it will
* output a "reset done" message. This function will check this
* message and process it correctly.
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_hardware_reset_handler(void);

/**
* @brief Send a HCI request.
*
* @param[in] *buf Pointer to buffer containing the request
* @param[in] length Length (in bytes) of the request
*/
extern int hci_application_send_request(const uint8_t *buf, int length);

/**
* @brief Receive a HCI response.
*
* @param[in] *buf Pointer to buffer to receive the response
* @param[in] length Length (in bytes) of the buffer
*/
extern int hci_application_receive_response(uint8_t *buf, int length);

/**
* @brief Receive an extended HCI response.
*
* @param[in] *buf Pointer to buffer to receive the response
* @param[in] length Length (in bytes) of the buffer
*/
extern int hci_application_receive_ext_response(uint8_t *buf, int len);

/**
* @brief Configure a different baudrate for TC35661
*
* @param[in] baudrate New baudrate to be used
*/
int hci_api_set_baudrate_tc35661(enum hci_api_baudrate baudrate);

/**
* @brief Configure a different baudrate for TC35667
*
* @param[in] baudrate New baudrate to be used
*/
int hci_api_set_baudrate_tc35667(enum hci_api_baudrate baudrate);

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
