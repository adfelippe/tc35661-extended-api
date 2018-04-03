/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/
#include <stdio.h>
#include "bt_api_ven.h"
#include "bt_api_internal.h"
#include "tcu_api.h"
#include "tcu_mng.h"
#include "tcu_ven.h"
//#include "debug_api.h"

/* Defining this macro disables all debug messages. */
#define printf(fmt, args...)	/* NOPRINTF */

#define TCU_MNG_VENDOR_CMD_TIMEOUT 200

/* accessing the eeprom in bigger chunks than this has to be split up so that
the maximum message size is not exceeded. 24 is the max. "TCU prefix" that is
added when eeprom read or write command is constructed. */
#define MAX_LEN_EEPROM_ACCESS (tcu_buffer_len - 24U)

enum bt_api_result bt_ven_event_handler(uint8_t *buf)
{
  return BT_RESULT_SUCCESS;
}

enum tcu_func_status extract_ven_data(uint8_t *buf, void *return_data)
{
  enum tcu_func_status status;
  uint8_t opcode = PARSE_CMD_OPCODE(buf);
	printf("opcode =%x\n",opcode);
  if (PARSE_CMD_SERVICE_ID(buf) != TCU_BT_SERVICE_VEN) {
    return TCU_FUNC_SERVICEID;
  }

  switch (opcode) {
  case GET_CMD_OPCODE(TCU_VEN_GET_MODULE_MAINTENANCE_EVENT):
    status = TcuVenGetModuleMaintenenace_Event(buf, (struct tcu_ven_get_module_maintenance_event *)return_data);
    printf("GET_MODULE_EVENT =%x\n",status);
    break;
  case GET_CMD_OPCODE(TCU_VEN_SET_MODULE_MAINTENANCE_EVENT):
    status = TcuVenSetModuleMaintenenace_Event(buf, (struct tcu_ven_set_module_maintenance_event *)return_data);
    break;
  case GET_CMD_OPCODE(TCU_VEN_SET_HOST_WAKEUP_RESP):
    status = TcuVenSetHostWakeupNotification_Response(buf, (struct tcu_ven_set_host_wakeup_notification_response *)return_data);
    break;
  default:
    status = TCU_FUNC_PARSING;
    break;
  }

  return status;
}

static enum tcu_func_status eeprom_read(uint8_t device_address, uint8_t device_address_bit_length, uint16_t address, uint8_t data_length)
{
  uint8_t buf[6];
  uint8_t *ptr = &buf[0];
  struct tcu_ven_get_module_maintenance_request request;

  /* general read eeprom request */
  *ptr++ = device_address;
  *ptr++ = device_address_bit_length;
  *ptr++ = 0x01; /* random read */
  *ptr++ = data_length;
  *ptr++ = (address & 0xff);
  *ptr++ = (address >> 8);

  request.command = COMMAND_READ_EEPROM;
  request.data_type = DATA_TYPE_ARRAY;
  request.request_buf = &buf[0];
//  request.request_len = sizeof(buf);
  request.request_len = data_length;

  return TcuVenGetModuleMaintenance_Request(&request);
}

static enum tcu_func_status eeprom_write(uint8_t device_address, uint8_t device_address_bit_length, uint16_t address, uint8_t data_length, uint8_t *data)
{
  uint8_t buf[6];
  uint8_t *ptr = &buf[0];
  struct tcu_ven_set_module_maintenance_request request;

  /* general write eeprom request */
  *ptr++ = device_address;
  *ptr++ = device_address_bit_length;
  *ptr++ = 0x01;
  *ptr++ = data_length;
  *ptr++ = (address & 0xff);
  *ptr++ = (address >> 8);

  request.command = COMMAND_WRITE_EEPROM;
  request.data_type = DATA_TYPE_ARRAY;
  request.request_buf = &buf[0];
  request.request_len = sizeof(buf);
  request.data_buf = data;
  request.data_len = data_length;

  return TcuVenSetModuleMaintenance_Request(&request);
}

enum bt_api_result bt_api_ven_eeprom_read(uint8_t device_address,
										uint8_t device_address_bit_length,
										uint16_t address,
										uint8_t *data,
										uint16_t length)
{
  struct tcu_accept_event accept;
  struct tcu_ven_get_module_maintenance_event event;
  uint16_t todo = length;
  uint16_t done = 0;
  enum tcu_func_status status;
  enum bt_api_result result;

  while (todo) {
    printf("address %d, todo %u, done %u\n", address, todo, done);

    event.data = data + done;
    if (todo > MAX_LEN_EEPROM_ACCESS) {
      event.length = (uint8_t)MAX_LEN_EEPROM_ACCESS;
    } else {
      event.length = (uint8_t)todo;
    }
	printf("event.length =%d\n",event.length);
    	status = eeprom_read(device_address, device_address_bit_length, address + done, event.length);
    if (status) {
      printf("eeprom_read() failed\n");
      return BT_RESULT_ERROR;
    }

    result = bt_wait_for_specific_event(TCU_ACCEPT, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&accept));
    if (result) {
      printf("bt_wait_for_specific_event() failed @ TCU_ACCEPT\n");
      return result;
    }

    result = bt_wait_for_specific_event(TCU_VEN_GET_MODULE_MAINTENANCE_EVENT, TCU_MNG_VENDOR_CMD_TIMEOUT, (void *)(&event));
    if (result) {
      printf("bt_wait_for_specific_event() failed @ TCU_ACCEPT\n");
      return result;
    }

    todo -= event.length;
    done += event.length;
  }

  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_ven_eeprom_write(uint8_t device_address, uint8_t device_address_bit_length, uint16_t address, uint8_t *data, uint16_t length)
{
  struct tcu_accept_event accept;
  struct tcu_ven_set_module_maintenance_event event;
  uint16_t todo = length;
  uint16_t done = 0;
  enum tcu_func_status status;
  enum bt_api_result result;

  while (todo) {
    uint8_t len;
    if (todo > MAX_LEN_EEPROM_ACCESS) {
      len = (uint8_t)MAX_LEN_EEPROM_ACCESS;
    } else {
      len = (uint8_t)todo;
    }

    printf("address %d, todo %ul, done %ul, len %ul\n", address, todo, done, len);

    status = eeprom_write(device_address, device_address_bit_length, address + done, len, data + done);
    if (status) {
      printf("eeprom_write() failed\n");
      return BT_RESULT_ERROR;
    }

    result = bt_wait_for_specific_event(TCU_ACCEPT, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&accept));
    if (result) {
      printf("bt_wait_for_specific_event() failed @ TCU_ACCEPT\n");
      return result;
    }

    result = bt_wait_for_specific_event(TCU_VEN_SET_MODULE_MAINTENANCE_EVENT, TCU_MNG_VENDOR_CMD_TIMEOUT, (void *)(&event));
    if (result) {
      printf("bt_wait_for_specific_event() failed @ TCU_VEN_SET_MODULE_MAINTENANCE_EVENT\n");
      return result;
    }

    todo -= len;
    done += len;
  }

  return BT_RESULT_SUCCESS;
}

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
