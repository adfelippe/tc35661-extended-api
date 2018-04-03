/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file bt_api_mng.h
*
* @brief Contains definitions and function declarations for the management (MNG) part of the BT API.
*/
#ifndef __BT_MNG_API_H__
#define __BT_MNG_API_H__

#include "bt_api.h"
#include "tcu_mng.h"

/*! Enum to represent the visibility scan parameter */
enum bt_api_mng_scan_visibility {
  mng_scan_non_visible = 0x00, /*!< device cannot be connected to */
  mng_scan_visible = 0x01      /*!< device can be connected to by other devices */
};

/*! Enum to represent the disoverability scan parameter */
enum bt_api_mng_scan_discoverability {
  mng_scan_non_discoverable = 0x00, /*!< device cannot be discovered by other devices */
  mng_scan_discoverable = 0x01      /*!< device can be discovered by other devices */
};

/**
* @brief Initialises the Bluetooth management functionality.
*
* Initialising the Bluetooth management functionality is necessary before any
* other Bluetooth Classic actions can be done.
*
* The Bluetooth management must be initialized as well before any vendor-specific
* commands can be send, for example for accessing the EEPROM.
*
* @param[in] device_name Pointer to Bluetooth device name
* @param[in] device_name_len Length (in bytes) of the Bluetooth device name
*/
enum bt_api_result bt_api_mng_init(uint8_t const *device_name, uint8_t device_name_len);

/**
* @brief Configures the Bluetooth scanning which controls if the device is
* discoverable by other devices and accepts incoming connections.
*
* @param[in] visible Device can be connected to.
* @param[in] discoverable Devices can be discovered.
*/
enum bt_api_result bt_api_mng_set_scan(enum bt_api_mng_scan_visibility visible, enum bt_api_mng_scan_discoverability discoverable);

/**
* @brief Starts a search for discoverable remote devices.
*
* No other functionality may be started until bt_application_mng_discover_remote_devices_done_callback()
* is received. This may take up to max. 10.24s.
*/
enum bt_api_result bt_api_mng_discover_remote_devices(void);

/**
* @brief Callback function to signal that a single device has been found.
*
* @param[in] *event Pointer to TCU remote device discovery event.
*/
extern void bt_application_mng_discover_remote_devices_callback(struct tcu_mng_discover_remote_dev_result_event *event);

/**
* @brief Callback function to signal that searching for discoverable remote
* devices has been finished.
*/
extern void bt_application_mng_discover_remote_devices_done_callback(void);

/**
* @brief Callback function to signal that (new) device name informations
* have been received for a certain device.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
* @param[in] device_name Name of the corresponding device
* @param[in] length_of_device_name Length (in bytes) of the device name
*/
extern void bt_application_mng_device_name_notification_callback(uint8_t *bd_addr, uint8_t *device_name, uint8_t length_of_device_name);

/**
* @brief Starts a search to discover services supported by the remote device.
*
* No other functionality may be started until
* bt_application_mng_discover_remote_service_done_callback() is received.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
*/
enum bt_api_result bt_api_mng_discover_remote_service(uint8_t *bd_addr);

/**
* @brief Callback function to signal when discovery of remote services has finished.
*
* The SPP server channel numbers must be used when using bt_api_spp_connect().
*
* @param[in] bd_addr[] Bluetooth device address of remote device
* @param[in] number_of_spp_channels Number of SPP channels the remote device supports
* @param[in] *spp_server_channel_numbers Array of SPP channels
*/
extern void bt_application_mng_discover_remote_service_done_callback(uint8_t *bd_addr, uint8_t number_of_spp_channels, uint8_t *spp_server_channel_numbers);

/**
* @brief Callback function to signal when a remote device wants to establish
* a connection.
*
* This function is called when the remote device just wants to pair as well.
* It is *not* an indication that a SPP connection will be established.
*
* When a negative value is returned, that indicates immediately that a connection
* to the remote device should not be allowed.
*
* The connection request may be presented to the user via the GUI. In that case,
* either bt_api_connection_request_confirm() or bt_api_connection_request_reject()
* must be called within 5 seconds.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
*/
extern int bt_application_mng_connection_request_callback(uint8_t *bd_addr);

/**
* @brief Confirms that a connection request to the remote device should be allowed.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
*/
enum bt_api_result bt_api_connection_request_confirm(uint8_t *bd_addr);

/**
* @brief Confirms that a connection request to the remote device should be rejected.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
*/
enum bt_api_result bt_api_connection_request_reject(uint8_t *bd_addr);

/**
* @brief Reads the rssi data from a remote device.
*
* This only works when a connection to the device is established.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
* @param[out] rssi Raw rssi value.
*/
enum bt_api_result bt_api_mng_read_rssi(uint8_t *bd_addr, uint16_t *rssi);

/**
* @brief Configure the service database for the device information record.
*
* @param[in] *config Configuration parameters
*/
enum bt_api_result bt_api_mng_set_sdp_for_di(struct tcu_mng_set_di_sdp_record_request *config);

/**
* @brief Configure the class of device setting.
*
* @param[in] *class_of_device Class of device to be used.
*/
enum bt_api_result bt_api_mng_set_class_of_device(uint8_t *class_of_device);

/**
* @brief Callback function to signal that a pre-Bluetooth v2.1 PIN-based
* pairing should be done.
*
* Either bt_api_mng_pin_write_confirm() or bt_api_mng_pin_write_reject()
* must be called within 30 seconds.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
*/
extern int bt_application_mng_pin_write_request_callback(uint8_t *bd_addr);

/**
* @brief Confirms a PIN-based pairing with the given PIN code.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
* @param[in] *pin_code PIN code
* @param[in] pin_code_length Length of PIN code
*/
enum bt_api_result bt_api_mng_pin_write_confirm(uint8_t *bd_addr, uint8_t *pin_code, uint8_t pin_code_length);

/**
* @brief Rejects a PIN-based pairing.
*
*/
enum bt_api_result bt_api_mng_pin_write_reject(uint8_t *bd_addr);

/**
* @brief Callback function to signal that secure simple pairing (SSP)
* procedure for pairing is in progress.
*
* The application should request a pass key from the user and provide it
* back using bt_api_mng_smp_passkey_input_request_confirm() or
* reject it with bt_api_mng_smp_passkey_input_request_reject().
*
* If -1 is returned, the pairing request is rejected immediately.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
*/
extern int bt_application_mng_smp_passkey_input_request_callback(uint8_t *bd_addr);

/**
* @brief Confirms SSP-based pairing with the given pass key.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
* @param[in] pass_key Pass key as input by the used
*/
enum bt_api_result bt_api_mng_smp_passkey_input_request_confirm(uint8_t *bd_addr, uint32_t pass_key);

/**
* @brief Rejects SSP-based pairing.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
*/
enum bt_api_result bt_api_mng_smp_passkey_input_request_reject(uint8_t *bd_addr);

/**
* @brief Callback function to signal that secure simple pairing (SSP)
* procedure for pairing is in progress.
*
* The application should display the supplied pass key to the user and
* display it until bt_application_mng_smp_hide_pass_key_callback() is called.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
* @param[in] pass_key Pass key to be displayed
*/
extern void bt_application_mng_smp_show_pass_key_callback(uint8_t *bd_addr, uint32_t pass_key);

/**
* @brief Callback function to signal that secure simple pairing (SSP)
* procedure for pairing is in progress.
*
* The application should hide the pass key display dialog.
*/
extern void bt_application_mng_smp_hide_pass_key_callback(void);

/**
* @brief Callback function to signal that secure simple pairing (SSP)
* procedure for pairing is in progress.
*
* The application should display the supplied pass_key and a choice to select
* yes or no.
* If yes is chosen by the user bt_api_mng_smp_user_confirmation_accept() should be
* called, otherwise bt_api_mng_smp_user_confirmation_reject() should be
* called.
*
* If -1 is returned, the pairing request is rejected immediately.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
* @param[in] pass_key Pass key to be displayed
*/
extern int bt_application_mng_smp_confirmation_request_callback(uint8_t *bd_addr, uint32_t pass_key);

/**
* @brief Signals that user has chosen YES for SSP-based pairing.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
*/
enum bt_api_result bt_api_mng_smp_user_confirmation_accept(uint8_t *bd_addr);

/**
* @brief Signals that user has chosen NO for SSP-based pairing.
*
* @param[in] bd_addr[] Bluetooth device address of remote device
*/
enum bt_api_result bt_api_mng_smp_user_confirmation_reject(uint8_t *bd_addr);

/**
* @brief Callback function to signal that secure simple pairing (SSP)
* procedure has succeeded.
*/
extern void bt_application_mng_smp_pairing_success_callback(void);

/**
* @brief Callback function to signal that secure simple pairing (SSP)
* procedure has failed.
*/
extern void bt_application_mng_smp_pairing_failed_callback(void);

/**
* @brief Callback function to retrieve the IO capabilities and
* authentication requirements of the device for doing a
* secure simple pairing (SSP) procedure.
*
* @param[out] *io_capability IO capability of the device
* @param[out] *authentication_requirement Authentication_requirement of the device
*/
extern void bt_application_mng_capability_callback(enum tcu_mng_smp_io_capability *io_capability,
                                                   enum tcu_mng_smp_authentication_requirement *authentication_requirement);

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
