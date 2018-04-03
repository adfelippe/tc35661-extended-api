/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "tcu_internal.h"
#include "tcu_mng.h"
#include <assert.h>
/*! Contains TCU_MNG_SNIFF_MODE_CONTROL_REQ data */
struct tcu_mng_rcv_sniff_subrating_event
{
  uint16_t max_transmit_latency; /*!< Maximum latency for data being transmitted from the local device to the remote device */
  uint16_t max_receive_latency;  /*!< Maximum latency for data being received by the local device from the remote device */
  uint16_t min_remote_timeout;   /*!< The base sniff subrate timeout in baseband slots that the remote device shall use */
  uint16_t min_local_timeout;    /*!< The base sniff subrate timeout in baseband slots that the local device will use */
};

/* these functions are not part of the public API yet, but may be when they are properly tested */
enum tcu_func_status TcuMngRcvSniffSubrating_Event(uint8_t *data, struct tcu_mng_rcv_sniff_subrating_event *event);
enum tcu_func_status TcuMngSniffModeControl_Request(struct tcu_mng_sniff_mode_control_request *request);
enum tcu_func_status TcuMngSniffModeControl_Response(uint8_t *data, enum tcu_mng_status *response);
enum tcu_func_status TcuMngExitSniffModeControl_Request(uint8_t *bd_addr);
enum tcu_func_status TcuMngExitSniffModeControl_Response(uint8_t *data, enum tcu_mng_status *response);
enum tcu_func_status TcuAccept_Event(uint8_t *buf, struct tcu_accept_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_ACCEPT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_ACCEPT));

  event->status = (enum tcu_mng_status)buf[offset++];
  event->service_id = buf[offset++];
  event->opcode = buf[offset++];

  tcu_application_log_response(buf, "TCU_ACCEPT", "status %d|service_id 0x%04x|opcode 0x%02x", event->status, event->service_id, event->opcode);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuNotAccept_Event(uint8_t *buf, struct tcu_not_accept_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_NOT_ACCEPT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_NOT_ACCEPT));

  event->service_id = buf[offset++];
  event->opcode = buf[offset++];

  tcu_application_log_response(buf, "TCU_NOT_ACCEPT", "service_id %02x|opcode %02x", event->service_id, event->opcode);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSysInvalidCommand_Event(uint8_t *buf, struct tcu_sys_invalid_com_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_SYS_INVALID_COMMAND));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_SYS_INVALID_COMMAND));

  event->service_id = buf[offset++];
  event->opcode = buf[offset++];

  tcu_application_log_response(buf, "TCU_SYS_INVALID_COMMAND", "service_id %02x|opcode %02x", event->service_id, event->opcode);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngInit_Request(struct tcu_mng_init_request *request)
{
  REQUEST_PREAMBLE();

  /* Check if name length ok */
  if (!request->device_name_len || request->device_name_len >= 248) {
    return TCU_FUNC_PARAMETER;
  }

  /* Set command op-code service id and parameter length */
  offset = set_cmd_header_user_length(buf, TCU_MNG_INIT_REQ, 1 + 1 + 1 + request->device_name_len, offset);

  buf[offset++] = 0x04;                     /* supported profiles = spp */
  buf[offset++] = 0x00;                     /* paired information / sniff subrating */
  buf[offset++] = request->device_name_len; /* Length of Device Name */
  /* Copy device name in the array */
  copy_array(request->device_name, buf, 0, offset, (uint16_t)request->device_name_len);

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_MNG_INIT_REQ", "device_name '%s'|device_name_lenf %d", request->device_name, request->device_name_len);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngInit_Response(uint8_t *buf, struct tcu_mng_init_response *response)
{
  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_INIT_RESP));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_INIT_RESP));

  response->status = (enum tcu_mng_status)buf[CMD_ARRAY_PARAM_OFFS];
  copy_array(buf, &response->bd_addr[0], CMD_ARRAY_PARAM_OFFS + 1, 0, TCU_SIZE_BDADDR);

  tcu_application_log_response(buf, "TCU_MNG_INIT_RESP", "status %02x", response->status);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngSetScan_Request(struct tcu_mng_set_scan_request *req)
{
  REQUEST_PREAMBLE();

  /* Set command op-code service id and parameter length */
  offset = set_cmd_header_user_length(buf, TCU_MNG_SET_SCAN_REQ, 1, offset);

  buf[offset++] = req->scan_mode;

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_MNG_SET_SCAN_REQ", "scan_mode %02x", req->scan_mode);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngSetScan_Response(uint8_t *buf, struct tcu_mng_set_scan_response *response)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_SET_SCAN_RESP));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_SET_SCAN_RESP));

  response->status = (enum tcu_mng_status)buf[offset++];

  tcu_application_log_response(buf, "TCU_MNG_SET_SCAN_RESP", "status %02x", response->status);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngConnectionRequest_Event(uint8_t *buf, struct tcu_mng_connection_request_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_CONNECTION_REQUEST_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_CONNECTION_REQUEST_EVENT));

  copy_array(buf, &event->bd_addr[0], offset, 0, TCU_SIZE_BDADDR);
  offset += TCU_SIZE_BDADDR;
  copy_array(buf, &event->class_of_device[0], offset, 0, TCU_SIZE_CLASS_OF_DEVICE);
  offset += TCU_SIZE_CLASS_OF_DEVICE;

  tcu_application_log_response(buf, "TCU_MNG_CONNECTION_REQUEST_EVENT", "bd_addr %02x:%02x:%02x:%02x:%02x:%02x:", event->bd_addr[0], event->bd_addr[1], event->bd_addr[2], event->bd_addr[3], event->bd_addr[4], event->bd_addr[5]);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngConnectionStatus_Event(uint8_t *buf, struct tcu_mng_connection_status_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_CONNECTION_STATUS_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_CONNECTION_STATUS_EVENT));

  event->status = (enum tcu_mng_status)buf[offset++];
  copy_array(buf, &event->bd_addr[0], offset, 0, TCU_SIZE_BDADDR);
  offset += TCU_SIZE_BDADDR;
  event->connection_status = (enum tcu_mng_connection_status)buf[offset++];
  if (event->connection_status > (enum tcu_mng_connection_status)TCU_MNG_CONNECTION_FAILURE) {
    copy_array(buf, &event->link_key[0], offset, 0, TCU_SIZE_LINKKEY);
    offset += TCU_SIZE_LINKKEY;
  }
  if (event->connection_status == (enum tcu_mng_connection_status)TCU_MNG_CONNECTION_LINK_KEY) {
    event->link_key_type = buf[offset++];
  }

  event->sniff_interval = buf[offset];

  tcu_application_log_response(buf, "TCU_MNG_CONNECTION_STATUS_EVENT", "bd_addr %02x:%02x:%02x:%02x:%02x:%02x:", event->bd_addr[0], event->bd_addr[1], event->bd_addr[2], event->bd_addr[3], event->bd_addr[4], event->bd_addr[5]);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngDiscoverRemoteDeviceResult_Event(uint8_t *buf, struct tcu_mng_discover_remote_dev_result_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_DISCOVER_REMOTE_DEVICE_RESULT_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_DISCOVER_REMOTE_DEVICE_RESULT_EVENT));

  memcpy(&event->bd_addr[0], buf + offset, TCU_SIZE_BDADDR);
  offset += TCU_SIZE_BDADDR;
  copy_array(buf, &event->class_of_device[0], offset, 0, 3);
  offset += 3;
  event->length_of_device_name = buf[offset++];
  event->name = &buf[offset];

  tcu_application_log_response(buf, "TCU_MNG_DISCOVER_REMOTE_DEVICE_RESULT_EVENT", "bd_addr %02x:%02x:%02x:%02x:%02x:%02x:", event->bd_addr[0], event->bd_addr[1], event->bd_addr[2], event->bd_addr[3], event->bd_addr[4], event->bd_addr[5]);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngDiscoverRemoteDeviceComplete_Event(uint8_t *buf)
{
  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_DISCOVER_REMOTE_DEVICE_COMPLETE_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_DISCOVER_REMOTE_DEVICE_COMPLETE_EVENT));

  tcu_application_log_event(buf, "TCU_MNG_DISCOVER_REMOTE_DEVICE_COMPLETE_EVENT", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngSspInfo_Event(uint8_t *buf, struct tcu_mng_ssp_info_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_SSP_INFO_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_SSP_INFO_EVENT));

  event->opcode = (enum tcu_mng_ssp_info_event_opcode)buf[offset++];
  switch (event->opcode) {
  case HCI_Encryption_Key_Refresh_Complete_Event:
    tcu_application_log_response(buf, "TCU_MNG_SSP_INFO_EVENT", "HCI_Encryption_Key_Refresh_Complete_Event");
    break;
  case HCI_User_Passkey_Request:
    event->parameter_length = buf[offset++];
    copy_array(buf, &event->details.HCI_User_Passkey_Request_Events.bd_addr[0], offset, 0, TCU_SIZE_BDADDR);
    offset += TCU_SIZE_BDADDR;
    tcu_application_log_response(buf, "TCU_MNG_SSP_INFO_EVENT", "HCI_User_Passkey_Request");
    break;
  case HCI_IO_Capability_Request_Event:
    event->parameter_length = buf[offset++];
    copy_array(buf, &event->details.HCI_IO_Capability_Request_Event.bd_addr[0], offset, 0, TCU_SIZE_BDADDR);
    tcu_application_log_response(buf, "TCU_MNG_SSP_INFO_EVENT", "HCI_IO_Capability_Request_Event");
    break;
  case HCI_IO_Capability_Response_Event:
    event->parameter_length = buf[offset++];
    copy_array(buf, &event->details.HCI_IO_Capability_Response_Event.bd_addr[0], offset, 0, TCU_SIZE_BDADDR);
    offset += TCU_SIZE_BDADDR;
    event->details.HCI_IO_Capability_Response_Event.io_capability = (enum tcu_mng_smp_io_capability)buf[offset++];
    event->details.HCI_IO_Capability_Response_Event.oob_data_present = (enum tcu_mng_smp_oob_data_present)buf[offset++];
    event->details.HCI_IO_Capability_Response_Event.authentication_requirement = (enum tcu_mng_smp_authentication_requirement)buf[offset++];
    tcu_application_log_response(buf, "TCU_MNG_SSP_INFO_EVENT", "HCI_IO_Capability_Response_Event|io_capability %02x|authentication_requirement %02x", event->details.HCI_IO_Capability_Response_Event.io_capability, event->details.HCI_IO_Capability_Response_Event.authentication_requirement);
    break;
  case HCI_IO_User_Confirmation_Request_Event:
    event->parameter_length = buf[offset++];
    copy_array(buf, &event->details.HCI_IO_User_Confirmation_Request_Event.bd_addr[0], offset, 0, TCU_SIZE_BDADDR);
    offset += TCU_SIZE_BDADDR;
    event->details.HCI_IO_User_Confirmation_Request_Event.pass_key = (buf[offset++] << 0);
    event->details.HCI_IO_User_Confirmation_Request_Event.pass_key |= (buf[offset++] << 8);
    event->details.HCI_IO_User_Confirmation_Request_Event.pass_key |= (buf[offset++] << 16);
    event->details.HCI_IO_User_Confirmation_Request_Event.pass_key |= (buf[offset++] << 24);
    tcu_application_log_response(buf, "TCU_MNG_SSP_INFO_EVENT", "HCI_IO_User_Confirmation_Request_Event");
    break;
  case HCI_Simple_Pairing_Complete_Event:
    event->parameter_length = buf[offset++];
    event->details.HCI_Simple_Pairing_Complete_Event.status = buf[offset++];
    copy_array(buf, &event->details.HCI_Simple_Pairing_Complete_Event.bd_addr[0], offset, 0, TCU_SIZE_BDADDR);
    tcu_application_log_response(buf, "TCU_MNG_SSP_INFO_EVENT", "HCI_Simple_Pairing_Complete_Event");
    break;
  case HCI_User_Passkey_Notification:
    event->parameter_length = buf[offset++];
    copy_array(buf, &event->details.HCI_User_Passkey_Notification_Events.bd_addr[0], offset, 0, TCU_SIZE_BDADDR);
    offset += TCU_SIZE_BDADDR;
    event->details.HCI_User_Passkey_Notification_Events.pass_key = buf[offset++] << 0;
    event->details.HCI_User_Passkey_Notification_Events.pass_key |= buf[offset++] << 8;
    event->details.HCI_User_Passkey_Notification_Events.pass_key |= buf[offset++] << 16;
    event->details.HCI_User_Passkey_Notification_Events.pass_key |= buf[offset++] << 24;
    tcu_application_log_response(buf, "TCU_MNG_SSP_INFO_EVENT", "HCI_User_Passkey_Notification");
    break;
  default:
    break;
  }
  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngDiscoverRemoteService_Event(uint8_t *buf, struct tcu_mng_discover_remote_service_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;
  uint8_t profile;
  int i;
  int j;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_DISCOVER_REMOTE_SERVICE_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_DISCOVER_REMOTE_SERVICE_EVENT));

  event->status = (enum tcu_mng_status)buf[offset++];
  copy_array(buf, &event->bd_addr[0], offset, 0, TCU_SIZE_BDADDR);
  offset += TCU_SIZE_BDADDR;
  event->service_count = buf[offset++];

  for (i = 0; i < event->service_count; i++) {
    event->service_type[i] = (enum tcu_mng_service_type)buf[offset++];
  }

  event->information_device_id.valid = 0;
  event->information_spp.valid = 0;

  for (i = 0; i < event->service_count; i++) {
    profile = buf[offset++];
    offset++; /* skip length */
    if (profile == 0x02) {
      event->information_spp.valid = 1;
      event->information_spp.number_of_server_channels = buf[offset++];
      for (j = 0; j < event->information_spp.number_of_server_channels; j++) {
        event->information_spp.server_channel[j] = buf[offset++];
      }
    }
    if (profile == 0x03) {
      event->information_device_id.valid = 1;
      event->information_device_id.specification_id = GET_SHORT(buf[offset], buf[offset + 1]);
      offset += 2;
      event->information_device_id.vendor_id = GET_SHORT(buf[offset], buf[offset + 1]);
      offset += 2;
      event->information_device_id.product_id = GET_SHORT(buf[offset], buf[offset + 1]);
      offset += 2;
      event->information_device_id.version = GET_SHORT(buf[offset], buf[offset + 1]);
      offset += 2;
      event->information_device_id.primary_record = buf[offset++];
      event->information_device_id.vendor_id_source = GET_SHORT(buf[offset], buf[offset + 1]);
      offset += 2;
    }
  }

  tcu_application_log_response(buf, "TCU_MNG_DISCOVER_REMOTE_SERVICE_EVENT", "bd_addr %02x:%02x:%02x:%02x:%02x:%02x|information_device_id.valid %d|information_spp.valid %d", event->bd_addr[0], event->bd_addr[1], event->bd_addr[2], event->bd_addr[3], event->bd_addr[4], event->bd_addr[5], event->information_device_id.valid, event->information_spp.valid);

  return TCU_FUNC_SUCCESS;
}
enum tcu_func_status TcuMngPinRequest_Event(uint8_t *buf, struct tcu_mng_pin_request_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_PIN_REQUEST_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_PIN_REQUEST_EVENT));

  copy_array(buf, &event->bd_addr[0], offset, 0, TCU_SIZE_BDADDR);
  offset += TCU_SIZE_BDADDR;
  event->length_of_device_name = buf[offset++];
  event->device_name = &buf[offset];

  tcu_application_log_response(buf, "TCU_MNG_PIN_REQUEST_EVENT", "bd_addr %02x:%02x:%02x:%02x:%02x:%02x:", event->bd_addr[0], event->bd_addr[1], event->bd_addr[2], event->bd_addr[3], event->bd_addr[4], event->bd_addr[5]);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngPinWrite_Request(struct tcu_mng_pin_write_request *pin)
{
  REQUEST_PREAMBLE();
  offset = CMD_ARRAY_PARAM_OFFS;

  copy_array(pin->bd_addr, buf, 0, offset, sizeof(pin->bd_addr));
  offset += TCU_SIZE_BDADDR;
  write_array_8(buf, pin->length_of_pin_code, &offset);
  copy_array(&pin->pin_code[0], buf, 0, offset, pin->length_of_pin_code);
  offset += pin->length_of_pin_code;

  offset = set_cmd_header_user_length(buf, TCU_MNG_PIN_WRITE_REQ, offset - 7, UART_PACKET_OFFSET);

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_SPP_CONNECT_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngPinWrite_Response(uint8_t *buf, struct tcu_mng_pin_write_response *response)
{
  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_PIN_WRITE_RESP));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_PIN_WRITE_RESP));

  response->status = (enum tcu_mng_status)buf[CMD_ARRAY_PARAM_OFFS];
  copy_array(buf, &response->bd_addr[0], CMD_ARRAY_PARAM_OFFS + 1, 0, TCU_SIZE_BDADDR);

  tcu_application_log_response(buf, "TCU_MNG_PIN_WRITE_RESP", "status %02x", response->status);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngRemoteDevNameAutoNotify_Event(uint8_t *buf, struct tcu_mng_remote_device_name_auto_notify_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_REMOTE_DEVICE_NAME_AUTO_NOTIFY_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_REMOTE_DEVICE_NAME_AUTO_NOTIFY_EVENT));

  copy_array(buf, &event->bd_addr[0], offset, 0, TCU_SIZE_BDADDR);
  offset += TCU_SIZE_BDADDR;
  event->length_of_device_name = buf[offset++];
  event->device_name = &buf[offset];

  tcu_application_log_response(buf, "TCU_MNG_REMOTE_DEVICE_NAME_AUTO_NOTIFY_EVENT", "bd_addr %02x:%02x:%02x:%02x:%02x:%02x:", event->bd_addr[0], event->bd_addr[1], event->bd_addr[2], event->bd_addr[3], event->bd_addr[4], event->bd_addr[5]);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngSspSet_Request(struct tcu_mng_ssp_set_request *request)
{
  REQUEST_PREAMBLE();
  offset = CMD_ARRAY_PARAM_OFFS;

  switch (request->opcode) {
  case HCI_IO_User_Passkey_Request_Reply_Opcode:
    write_array_16(buf, HCI_IO_User_Passkey_Request_Reply_Opcode, &offset);
    buf[offset++] = 0x0a; /* parameter length */
    copy_array(&request->bd_addr[0], buf, 0, offset, TCU_SIZE_BDADDR);
    offset += TCU_SIZE_BDADDR;
    buf[offset++] = (request->parameter.HCI_IO_User_Passkey_Request_Reply.pass_key >> 0) & 0xff;
    buf[offset++] = (request->parameter.HCI_IO_User_Passkey_Request_Reply.pass_key >> 8) & 0xff;
    buf[offset++] = (request->parameter.HCI_IO_User_Passkey_Request_Reply.pass_key >> 16) & 0xff;
    buf[offset++] = (request->parameter.HCI_IO_User_Passkey_Request_Reply.pass_key >> 24) & 0xff;
    break;
  case HCI_IO_User_Passkey_Negativ_Request_Reply_Opcode:
    write_array_16(buf, HCI_IO_User_Passkey_Negativ_Request_Reply_Opcode, &offset);
    buf[offset++] = 0x06; /*parameter length */
    copy_array(&request->bd_addr[0], buf, 0, offset, TCU_SIZE_BDADDR);
    offset += TCU_SIZE_BDADDR;
    break;
  case HCI_IO_Capability_Request_Reply_Opcode:
    write_array_16(buf, HCI_IO_Capability_Request_Reply_Opcode, &offset);
    buf[offset++] = 0x09; /*parameter length */
    copy_array(&request->bd_addr[0], buf, 0, offset, TCU_SIZE_BDADDR);
    offset += TCU_SIZE_BDADDR;
    buf[offset++] = request->parameter.HCI_IO_Capability_Request_Reply_Parameter.io_capability;
    buf[offset++] = request->parameter.HCI_IO_Capability_Request_Reply_Parameter.oob_data_present;
    buf[offset++] = request->parameter.HCI_IO_Capability_Request_Reply_Parameter.authentication_requirement;
    break;
  case HCI_IO_Capability_Request_Negative_Reply_Opcode:
    write_array_16(buf, HCI_IO_Capability_Request_Negative_Reply_Opcode, &offset);
    buf[offset++] = 0x07; /*parameter length */
    copy_array(&request->bd_addr[0], buf, 0, offset, TCU_SIZE_BDADDR);
    offset += TCU_SIZE_BDADDR;
    buf[offset++] = request->parameter.HCI_IO_Capability_Request_Negative_Reply.reason;
    break;
  case HCI_User_Confirmation_Request_Reply_Opcode:
    write_array_16(buf, HCI_User_Confirmation_Request_Reply_Opcode, &offset);
    buf[offset++] = 0x06; /*parameter length */
    copy_array(&request->bd_addr[0], buf, 0, offset, TCU_SIZE_BDADDR);
    offset += TCU_SIZE_BDADDR;
    buf[offset++] = 0;
    break;
  case HCI_User_Confirmation_Negative_Reply_Opcode:
    write_array_16(buf, HCI_User_Confirmation_Negative_Reply_Opcode, &offset);
    buf[offset++] = 0x06; /*parameter length */
    copy_array(&request->bd_addr[0], buf, 0, offset, TCU_SIZE_BDADDR);
    offset += TCU_SIZE_BDADDR;
    break;
  case HCI_Write_Simple_Pairing_Debug_Mode_Command_Opcode:
    write_array_16(buf, HCI_Write_Simple_Pairing_Debug_Mode_Command_Opcode, &offset);
    buf[offset++] = 0x01; /*parameter length */
    buf[offset++] = request->parameter.HCI_Write_Simple_Pairing_Debug_Mode_Command.debug_mode;

    ;
    break;
  default:
    break;
  }

  set_cmd_header_user_length(buf, TCU_MNG_SSP_SET_REQ, offset - 7, UART_PACKET_OFFSET);
  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_MNG_SSP_SET_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngSspSet_Response(uint8_t *buf, struct tcu_mng_ssp_set_response *response)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_SSP_SET_RESP));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_SSP_SET_RESP));

  response->status = (enum tcu_mng_status)buf[offset++];
  response->parameter_length = buf[offset++];
  response->opcode_command_complete = buf[offset++];
  response->event_length = buf[offset++];
  response->Num_HCI_Command_Packet = buf[offset++];
  response->opcode = GET_SHORT(buf[offset], buf[offset + 1]);
  offset += 2;
  response->command_status = buf[offset++];

  switch (response->opcode) {
  case HCI_IO_Capability_Request_Reply_Opcode:
  case HCI_IO_Capability_Request_Negative_Reply_Opcode:
  case HCI_User_Confirmation_Request_Reply_Opcode:
  case HCI_User_Confirmation_Negative_Reply_Opcode:
    copy_array(buf, &response->bd_addr[0], offset, 0, TCU_SIZE_BDADDR);
    break;
  default:
    break;
  }

  tcu_application_log_response(buf, "TCU_MNG_SSP_SET_RESP", "status %02x", response->status);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngConnectionAccept_Request(struct tcu_mng_connection_accept_request *conn)
{
  REQUEST_PREAMBLE();
  offset = CMD_ARRAY_PARAM_OFFS;

  /* fixme: this is wrong: rejection does not work most likely, because link
  key must not be sent in this case! */

  write_array_8(buf, conn->response_type, &offset);
  copy_array(conn->bd_addr, buf, 0, offset, TCU_SIZE_BDADDR);
  offset += TCU_SIZE_BDADDR;
  write_array_8(buf, conn->use_of_link_key, &offset);
  if (conn->use_of_link_key) {
    copy_array(&conn->link_key[0], buf, 0, offset, TCU_SIZE_LINKKEY);
    offset += TCU_SIZE_LINKKEY;
  }

  offset = set_cmd_header_user_length(buf, TCU_MNG_CONNECTION_ACCEPT_REQ, offset - 7, UART_PACKET_OFFSET);

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_MNG_CONNECTION_ACCEPT_REQ", "");

  return TCU_FUNC_SUCCESS;
}

/* fixme: struct */
enum tcu_func_status TcuMngConnectionAccept_Response(uint8_t *buf, enum tcu_mng_connection_response_type *response)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_CONNECTION_ACCEPT_RESP));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_CONNECTION_ACCEPT_RESP));

  *response = (enum tcu_mng_connection_response_type)buf[offset];

  tcu_application_log_response(buf, "TCU_MNG_CONNECTION_ACCEPT_RESP", "status %02x", *response);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngDiscoverRemoteDevice_Request(struct tcu_mng_discover_remote_device_request *request)
{
  REQUEST_PREAMBLE();

  offset = set_cmd_header_user_length(buf, TCU_MNG_DISCOVER_REMOTE_DEVICE_REQ, 1, offset);
  buf[offset++] = request->max_number_of_reports;

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_MNG_DISCOVER_REMOTE_DEVICE_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngDiscoverRemoteService_Request(struct tcu_mng_discover_remote_service_request *serv)
{
  REQUEST_PREAMBLE();
  offset = CMD_ARRAY_PARAM_OFFS;

  write_array_8(buf, serv->security_Mode, &offset);
  copy_array(serv->bd_addr, buf, 0, offset, TCU_SIZE_BDADDR);
  offset += TCU_SIZE_BDADDR;
  write_array_8(buf, serv->use_of_link_key, &offset);
  if (serv->use_of_link_key) {
    copy_array(&serv->link_key[0], buf, 0, offset, TCU_SIZE_LINKKEY);
    offset += TCU_SIZE_LINKKEY;
  }

  offset = set_cmd_header_user_length(buf, TCU_MNG_DISCOVER_REMOTE_SERVICE_REQ, offset - 7, UART_PACKET_OFFSET);

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_MNG_DISCOVER_REMOTE_SERVICE_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngReadRssi_Request(struct tcu_mng_read_rssi_request *request)
{
  REQUEST_PREAMBLE();

  offset = set_cmd_header_user_length(buf, TCU_MNG_READ_RSSI_REQ, 6, offset);
  copy_array(request->bd_addr, buf, 0, offset, TCU_SIZE_BDADDR);
  offset += TCU_SIZE_BDADDR;

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_MNG_READ_RSSI_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngReadRssi_Response(uint8_t *buf, struct tcu_mng_read_rssi_response *return_data)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_READ_RSSI_RESP));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_READ_RSSI_RESP));

  return_data->status = (enum tcu_mng_status)buf[offset++];
  copy_array(buf, return_data->bd_addr, offset, 0, TCU_SIZE_BDADDR);
  offset += TCU_SIZE_BDADDR;
  copy_array(buf, &return_data->rssi, offset, 0, sizeof(uint16_t));

  tcu_application_log_response(buf, "TCU_MNG_READ_RSSI_RESP", "status %02x", return_data->status);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngRcvSniffSubrating_Event(uint8_t *buf, struct tcu_mng_rcv_sniff_subrating_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_RCV_SNIFF_SUBRATING_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_RCV_SNIFF_SUBRATING_EVENT));

  copy_array(buf, &event->max_transmit_latency, offset, 0, sizeof(uint16_t));
  offset += sizeof(uint16_t);
  copy_array(buf, &event->max_receive_latency, offset, 0, sizeof(uint16_t));
  offset += sizeof(uint16_t);
  copy_array(buf, &event->min_remote_timeout, offset, 0, sizeof(uint16_t));
  offset += sizeof(uint16_t);
  copy_array(buf, &event->min_local_timeout, offset, 0, sizeof(uint16_t));
  offset += sizeof(uint16_t);

  /* fixme */
  tcu_application_log_response(buf, "TCU_MNG_RCV_SNIFF_SUBRATING_EVENT", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngSniffModeControl_Request(struct tcu_mng_sniff_mode_control_request *request)
{
  REQUEST_PREAMBLE();

  offset = set_cmd_header_user_length(buf, TCU_MNG_SNIFF_MODE_CONTROL_REQ, 0x0E, offset);
  copy_array(request->bd_addr, buf, 0, offset, TCU_SIZE_BDADDR);
  offset += TCU_SIZE_BDADDR;
  copy_array(&request->max_interval, buf, 0, offset, sizeof(uint16_t));
  offset += sizeof(uint16_t);
  copy_array(&request->min_interval, buf, 0, offset, sizeof(uint16_t));
  offset += sizeof(uint16_t);
  copy_array(&request->attempt, buf, 0, offset, sizeof(uint16_t));
  offset += sizeof(uint16_t);
  copy_array(&request->timeout, buf, 0, offset, sizeof(uint16_t));
  offset += sizeof(uint16_t);

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_MNG_SET_DI_SDP_RECORD_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngSniffModeControl_Response(uint8_t *buf, enum tcu_mng_status *response)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_SNIFF_MODE_CONTROL_RESP));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_SNIFF_MODE_CONTROL_RESP));

  *response = (enum tcu_mng_status)buf[offset];

  /* fixme */
  tcu_application_log_response(buf, "TCU_MNG_SNIFF_MODE_CONTROL_RESP", "status %02x", *response);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngExitSniffModeControl_Request(uint8_t *bd_addr)
{
  REQUEST_PREAMBLE();

  offset = set_cmd_header_user_length(buf, TCU_MNG_EXIT_SNIFF_MODE_CONTROL_REQ, 6, offset);
  copy_array(bd_addr, buf, 0, offset, TCU_SIZE_BDADDR);
  offset += TCU_SIZE_BDADDR;

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_MNG_EXIT_SNIFF_MODE_CONTROL_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngExitSniffModeControl_Response(uint8_t *buf, enum tcu_mng_status *response)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_EXIT_SNIFF_MODE_CONTROL_RESP));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_EXIT_SNIFF_MODE_CONTROL_RESP));

  *response = (enum tcu_mng_status)buf[offset];

  /* fixme */
  tcu_application_log_response(buf, "TCU_MNG_EXIT_SNIFF_MODE_CONTROL_RESP", "status %02x", *response);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngSetDiSdpRecord_Request(struct tcu_mng_set_di_sdp_record_request *request)
{
  REQUEST_PREAMBLE();

  offset = set_cmd_header_user_length(buf, TCU_MNG_SET_DI_SDP_RECORD_REQ, 0x0B, offset);
  copy_array((uint8_t *)&request->specification_id, buf, 0, offset, sizeof(uint16_t));
  offset += sizeof(uint16_t);
  copy_array((uint8_t *)&request->vendor_id, buf, 0, offset, sizeof(uint16_t));
  offset += sizeof(uint16_t);
  copy_array((uint8_t *)&request->product_id, buf, 0, offset, sizeof(uint16_t));
  offset += sizeof(uint16_t);
  copy_array((uint8_t *)&request->version, buf, 0, offset, sizeof(uint16_t));
  offset += sizeof(uint16_t);
  write_array_8(buf, request->primary_record, &offset);
  copy_array((uint8_t *)&request->vendor_id_source, buf, 0, offset, sizeof(uint16_t));
  offset += sizeof(uint16_t);

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_MNG_SET_DI_SDP_RECORD_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuMngSetDiSdpRecord_Response(uint8_t *buf, enum tcu_mng_status *response)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_MNG_SET_DI_SDP_RECORD_RESP));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_MNG_SET_DI_SDP_RECORD_RESP));

  *response = (enum tcu_mng_status)buf[offset];

  /* fixme */
  tcu_application_log_response(buf, "TCU_MNG_SET_DI_SDP_RECORD_RESP", "status %02x", *response);

  return TCU_FUNC_SUCCESS;
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
