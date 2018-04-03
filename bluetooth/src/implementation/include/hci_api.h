/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file hci_api.h
*
* @brief Contains definitions and function declarations for Bluetooth HCI API.
*/
#ifndef __HCI_API_H__
#define __HCI_API_H__

#include "types.h"

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
* @brief Change from HCI mode to complete mode.
* HW RESET is needed to change from complete mode to HCI mode.
*
* @return 0 Success
* @return -1 Error
*/
int hci_api_set_complete_mode(void);

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
