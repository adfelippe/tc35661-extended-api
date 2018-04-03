/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file tcu_api.h
*
* @brief Contains definitions and function declarations that are shared between
* the different parts of the TCU API.
*/
#ifndef __TCU_COMMON_H__
#define __TCU_COMMON_H__

#include <stdint.h>

void tcu_application_log_request(uint8_t *buf, const char *name, const char *format, ...);
void tcu_application_log_response(uint8_t *buf, const char *name, const char *format, ...);
void tcu_application_log_event(uint8_t *buf, const char *name, const char *format, ...);

/*! data buffer used for sending and parsing */
extern uint8_t *tcu_buffer;
/*! length of data buffer */
extern uint32_t tcu_buffer_len;

/*! SMP address type enumeration */
enum tcu_smp_address_type {
  SMP_ADDRESS_TYPE_PUBLIC_ADDRESS = 0x00, /*!< public address */
  SMP_ADDRESS_TYPE_RANDOM_ADDRESS = 0x01  /*!< random address */
};

/*! TCU function status enumeration */
enum tcu_func_status {
  TCU_FUNC_SUCCESS = 0x00,     /*!< Success */
  TCU_FUNC_ERROR = 0x01,       /*!< General error */
  TCU_FUNC_TIMEOUT = 0x02,     /*!< Timeout error */
  TCU_FUNC_OPCODE = 0x03,      /*!< Commmand opcode mismatch */
  TCU_FUNC_SERVICEID = 0x04,   /*!< Ccmmmand service id mismatch */
  TCU_FUNC_PARAMETER = 0x05,   /*<! Parameter error */
  TCU_FUNC_LENGTH = 0x06,      /*!< Wrong Length */
  TCU_FUNC_SEND_FAILED = 0x07, /*!< Command sending failed */
  TCU_FUNC_PARSING = 0x08      /*!< Parsing error */
};

/*! GATT error codes enumeration */
enum tcu_gatt_status {
  LE_GATT_SUCCESS = 0x00,                                       /*!< success*/
  LE_ERR_INV_HANDLE = 0x01,                                     /*!< invalid handle */
  LE_ERR_READ_NOT_PERMITTED = 0x02,                             /*!< read not permitted */
  LE_ERR_WRITE_NOT_PERMITTED = 0x03,                            /*!< write not permitted */
  LE_ERR_INVALID_PACKET_DATA_UNIT = 0x04,                       /*!< invalid packet data */
  LE_ERR_INSUFFICIENT_AUTHENTICATION = 0x05,                    /*!< insufficient authentication */
  LE_ERR_REQUEST_NOT_SUPPORTED = 0x06,                          /*!< request not supported */
  LE_ERR_INVALID_OFFSET = 0x07,                                 /*!< invalid offset */
  LE_ERR_INSUFFICIENT_AUTHORIZATION = 0x08,                     /*!< insufficient authorization */
  LE_ERR_PREPARE_QUEUE_FULL = 0x09,                             /*!< prepare queue full */
  LE_ERR_LOOK_UP_ATTRIBUTE_NOT_FOUND = 0x0A,                    /*!< look up attribute not found */
  LE_ERR_ATTRIBUTE_NOT_LONG = 0x0B,                             /*!< attribute not long */
  LE_ERR_INSUFFICIENT_ENCRYPTION_KEY_SIZE = 0x0C,               /*!< insufficient encryption key size */
  LE_ERR_INVALID_ATTRIBUTE_VALUE_LENGTH = 0x0D,                 /*!< invalid attribute value length */
  LE_ERR_UNLIKELY_ERROR = 0x0E,                                 /*!< unlikely error */
  LE_ERR_INSUFFICIENT_ENCRYPTION = 0x0F,                        /*!< insufficient encryption */
  LE_ERR_UNSUPPORTED_GROUP_TYPE = 0x10,                         /*!< unsupported group type */
  LE_ERR_INSUFFICIENT_RESOURCES = 0x12,                         /*!< insufficient resources */
  LE_ERR_APPLICATION_ERROR = 0x80,                              /*!< application error */
  LE_ERR_CLIENT_CONFIG_DESCRIPTOR_IMPROPERLY_CONFIGURED = 0x81, /*!< ccd improperly configured */
  LE_ERR_GATT_UPDATE_NOT_NEEDED = 0xc0,                         /*!< GATT update not needed */
  LE_ERR_NOT_INITIALIZED = 0xE1,                                /*!< not initialized */
  LE_ERR_REQUEST_IN_PROGRESS = 0xE2,                            /*!< request in progress */
  LE_ERR_INVALID_CONNECTION_HANDLE = 0xE3,                      /*!< invalid connection handle */
  LE_ERR_DISCONNECT_ERROR = 0x84,                               /*!< disconnect error */
  LE_ERR_REMOTE_DATA_LENGTH_ERROR = 0xE5,                       /*!< remote data length error */
  LE_ERR_INVALID_PARAMETER_LENGTH = 0xE6,                       /*!< invalid parameter length */
  LE_ERR_ACCEPT_REQUEST_STATUS_ERROR = 0xE7,                    /*!< accept request status error */
  LE_ERR_INVALID_REQUEST = 0xEE,                                /*!< invalid request */
  LE_ERR_INITIALIZATION_ALREADY_DONE = 0xF1,                    /*!< initialization already done */
  LE_ERR_PROCEDURE_IN_PROGRESS = 0xFE,
  LE_ERR_PROFILE_SPECIFIC = 0xFD,
  LE_ERR_TIMEOUT = 0xFF /*!< timeout */
};

/*! SDB error code enumeration */
enum sdb_status_code {
  LE_SDB_SUCCESS = 0x00,                  /*!< success */
  LE_SDB_MEMORY_NOT_AVAILABLE = 0x01,     /*!< memory not available */
  LE_SDB_INVALID_HANDLE = 0x02,           /*!< invalid handle */
  LE_SDB_MAX_CHAR_HANDLE_EXCEEDED = 0x03, /*!< maximum characteristic handle exceeded */
  LE_SDB_INVALID_PACKET = 0x04,           /*!< invalid packet */
  LE_SDB_DEVICE_NOT_INITIALIZED = 0x81    /*!< device is not initialized */
};

/*! TCU service id enumeration */
enum tcu_service_ids {
  TCU_LE_SERVICE_MNG = 0xD1,      /*!< low-energy management service id */
  TCU_LE_SERVICE_GATT_CLI = 0xD2, /*!< GATT client service id */
  TCU_LE_SERVICE_GATT_SRV = 0xD3, /*!< GATT server service id */
  TCU_LE_SERVICE_SMP_MAS = 0xD4,  /*!<  SMP master service id */
  TCU_LE_SERVICE_SMP_SLV = 0xD5,  /*!< SMP slave service id */
  TCU_BT_SERVICE_VEN = 0xEF,      /*!< vendor service id */
  TCU_BT_SERVICE_MNG = 0xE1,      /*!< classic management service id */
  TCU_BT_SERVICE_SPP = 0xE5       /*!< SPP profile service id */
};

/*! TCU UUID length enumeration */
enum tcu_uuid_type {
  LE_SHORT_UUID = 0x02, /*!< two-byte form */
  LE_LONG_UUID = 0x10   /*!< sixteen-byte form */
};

/*! Size of Bluetooth device address definition */
#define TCU_SIZE_BDADDR 6
/*! Size of link key definition */
#define TCU_SIZE_LINKKEY 16
/*! Max. size of a device name definition */
#define TCU_SIZE_DEVICE_NAME 24
/*! Size of class of device definition */
#define TCU_SIZE_CLASS_OF_DEVICE 3

/*! Definition of bit-field offset for service id used in command macros */
#define CMD_SERVICE_ID_OFFSET 24
/*! Definition of bit-field offsets for opcode used in command macros */
#define CMD_OPCODE_OFFSET 16

/*! Definition of UART packet offset */
#define UART_PACKET_OFFSET 3

/*! Definition of service id offset in command array (in bytes) */
#define CMD_ARRAY_SER_ID_OFFS (UART_PACKET_OFFSET + 0)
/*! Definition of opcode offset in command array (in bytes) */
#define CMD_ARRAY_OPCODE_OFFS (UART_PACKET_OFFSET + 1)
/*! Definition of length offset in command array (in bytes) */
#define CMD_ARRAY_LEN_OFFS (UART_PACKET_OFFSET + 2)
/*! Definition of parameter offset in command array (in bytes) */
#define CMD_ARRAY_PARAM_OFFS (UART_PACKET_OFFSET + 4)

/*! Definition of service id mask used in command macros */
#define CMD_SER_ID_MASK (0xFFUL << CMD_SERVICE_ID_OFFSET)
/*! Definition of opcode mask used in command macros */
#define CMD_OPCODE_MASK (0xFFUL << CMD_OPCODE_OFFSET)
/*! Definition of length mask used in command macros */
#define CMD_LENGTH_MASK (0xFFFFUL)

/*! Definition of variable length value used in command declarations */
#define LEN_VAR 0xFFFF
/*! Defintion of length zero used in command declarations */
#define NO_PARAM 0x0000

/*! Defintion of TCU message header size (in bytes) */
#define BLE_MSG_HDR_SIZE 4

/*! Definition of command macro concatenation */
#define CMD_CONCAT(service_id, opcode, len) (((((uint32_t)((service_id)&0xFF)) << CMD_SERVICE_ID_OFFSET) | (((uint32_t)((opcode)&0xFF)) << CMD_OPCODE_OFFSET)) | (((uint32_t)(len) & 0xFFFF)))

/*! Definition of macro that returns uint16_t from two bytes */
#define GET_SHORT(lo_byte, hi_byte) ((lo_byte & 0xFF) | (((uint16_t)(hi_byte) << 8) & 0xFF00))
/*! Definition of macro that returns uint8_t LSB from two bytes */
#define GET_BYTE_LSB(short_data) (short_data & 0xFF)
/*! Definition of macro that returns uint8_t MSB from two bytes */
#define GET_BYTE_MSB(short_data) ((short_data >> 8) & 0xFF)

/*! Defintion of extract service id from command macro */
#define GET_CMD_SERVICE_ID(command) ((uint8_t)((((uint32_t)(command)) & CMD_SER_ID_MASK) >> CMD_SERVICE_ID_OFFSET))
/*! Definition of extracts opcode from command macro */
#define GET_CMD_OPCODE(command) ((uint8_t)(((uint32_t)(command) & CMD_OPCODE_MASK) >> CMD_OPCODE_OFFSET))
/*! Defintion of extracts command length from command macro */
#define GET_PARAM_LENGTH(command) ((uint16_t)((uint32_t)(command) & CMD_LENGTH_MASK))
/*! Defintition of extracts service id and opcode from command macro. returns a 16-bit word  */
#define GET_CMD_MESSAGE(command) (GET_SHORT(GET_CMD_OPCODE(command), GET_CMD_SERVICE_ID(command)))
/*! Definition of total message size macro */
#define GET_TOTAL_LE_MESSAGE_SIZE(data) (PARSE_CMD_PARAM_LENGTH(data) + BLE_MSG_HDR_SIZE)
/*! Definition of extracts service id from input stream macro */
#define PARSE_CMD_SERVICE_ID(data) ((uint8_t)((data[CMD_ARRAY_SER_ID_OFFS])))
/*! Definition of extract opcode from input stream macro */
#define PARSE_CMD_OPCODE(data) ((uint8_t)((data[CMD_ARRAY_OPCODE_OFFS])))
/*! Definition of extract service id and opcode from input stream macro. returns a 16-bit word (to be used togehter with GET_CMD_MESSAGE()) */
#define PARSE_CMD_MESSAGE(data) (GET_SHORT(PARSE_CMD_OPCODE(data), PARSE_CMD_SERVICE_ID(data)))
/*! Definition of extracts length from command macro */
#define PARSE_CMD_PARAM_LENGTH(data) ((data[CMD_ARRAY_LEN_OFFS] & 0xFF) | (((data[CMD_ARRAY_LEN_OFFS + 1]) & 0xFF) << 8))
/*! Definition for setting a short UUID macro */
#define SET_LE_SHORT_UUID(uuid)                                                                              \
  {                                                                                                          \
    GET_BYTE_LSB((uint16_t)(uuid)), GET_BYTE_MSB((uint16_t)(uuid)), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \
  }

/**
* @brief Sends a TCU command to TC356xx by whatever means necessary.
*
* This function shall not return before the complete message has been send.
*
* @param[in] *data Pointer to the TCU message
* @param[in] length Number of bytes to be send
*
* @return 0 Message was sent successfully
*/
extern int tcu_application_send(uint8_t *data, uint16_t length);

/**
* @brief Retrieve a specific TCU command from TC356xx by whatever means necessary.
*
* A single buffer to store events is not enough. The TCU subsystem may call this
* function multiple times after another and expects xxx
*
* @param[in] expected_service_id Service ID of command that we expect to receive
* @param[in] expected_opcode Opcode of command that we expect to receive
* @param[in] timeout_ms Timeout in ms to wait
* @param[in] *buf Pointer to storage where received command can be put
* @param[in] buf_length Length of storage
*
* @return 0 Message was sent successfully
*/
extern int tcu_application_wait_for_specific_event(uint8_t expected_service_id, uint8_t expected_opcode, uint32_t timeout_ms, uint8_t *buf, int buf_length);

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
