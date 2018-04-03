/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "bt_api_mng.h"
#include "bt_api_internal.h"
#include "bt_api_spp.h"
#include "key_store_bt_api.h"
#include "tcu_api.h"
#include "tcu_mng.h"
//#include "debug_api.h"

/* Defining this macro disables all debug messages. */
#define printf(fmt, args...)	/* NOPRINTF */

enum tcu_func_status extract_mng_data(uint8_t *buf, void *return_data)
{
  uint8_t opcode = PARSE_CMD_OPCODE(buf);

  if (PARSE_CMD_SERVICE_ID(buf) != TCU_BT_SERVICE_MNG) {
    return TCU_FUNC_SERVICEID;
  }
	printf("extartct_mng_data service_id =%x ,%x\n",PARSE_CMD_SERVICE_ID(buf),TCU_BT_SERVICE_MNG);

	printf("extartct_mng_data opcode =%x ,%x\n",PARSE_CMD_OPCODE(buf),GET_CMD_OPCODE(TCU_MNG_INIT_RESP));
  switch (opcode) {
  case GET_CMD_OPCODE(TCU_MNG_INIT_RESP):
    return TcuMngInit_Response(buf, (struct tcu_mng_init_response *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_SET_SCAN_RESP):
    return TcuMngSetScan_Response(buf, (struct tcu_mng_set_scan_response *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_PIN_WRITE_RESP):
    return TcuMngPinWrite_Response(buf, (struct tcu_mng_pin_write_response *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_SSP_SET_RESP):
    return TcuMngSspSet_Response(buf, (struct tcu_mng_ssp_set_response *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_CONNECTION_REQUEST_EVENT):
    return TcuMngConnectionRequest_Event(buf, (struct tcu_mng_connection_request_event *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_CONNECTION_ACCEPT_RESP):
    return TcuMngConnectionAccept_Response(buf, (enum tcu_mng_connection_response_type *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_REMOTE_DEVICE_NAME_AUTO_NOTIFY_EVENT):
    return TcuMngRemoteDevNameAutoNotify_Event(buf, (struct tcu_mng_remote_device_name_auto_notify_event *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_CONNECTION_STATUS_EVENT):
    return TcuMngConnectionStatus_Event(buf, (struct tcu_mng_connection_status_event *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_DISCOVER_REMOTE_DEVICE_RESULT_EVENT):
    return TcuMngDiscoverRemoteDeviceResult_Event(buf, (struct tcu_mng_discover_remote_dev_result_event *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_DISCOVER_REMOTE_DEVICE_COMPLETE_EVENT):
    return TcuMngDiscoverRemoteDeviceComplete_Event(buf);
  case GET_CMD_OPCODE(TCU_MNG_DISCOVER_REMOTE_SERVICE_EVENT):
    return TcuMngDiscoverRemoteService_Event(buf, (struct tcu_mng_discover_remote_service_event *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_SSP_INFO_EVENT):
    return TcuMngSspInfo_Event(buf, (struct tcu_mng_ssp_info_event *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_PIN_REQUEST_EVENT):
    return TcuMngPinRequest_Event(buf, (struct tcu_mng_pin_request_event *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_READ_RSSI_RESP):
    return TcuMngReadRssi_Response(buf, (struct tcu_mng_read_rssi_response *)return_data);
  case GET_CMD_OPCODE(TCU_ACCEPT):
    return TcuAccept_Event(buf, (struct tcu_accept_event *)return_data);
  case GET_CMD_OPCODE(TCU_NOT_ACCEPT):
    return TcuNotAccept_Event(buf, (struct tcu_not_accept_event *)return_data);
  case GET_CMD_OPCODE(TCU_SYS_INVALID_COMMAND):
    return TcuSysInvalidCommand_Event(buf, (struct tcu_sys_invalid_com_event *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_SET_DI_SDP_RECORD_RESP):
    return TcuMngSetDiSdpRecord_Response(buf, (enum tcu_mng_status *)return_data);
#if 0
  case GET_CMD_OPCODE(TCU_MNG_RCV_SNIFF_SUBRATING_EVENT):
    return TcuMngRcvSniffSubrating_Event(buf, (struct tcu_mng_rcv_sniff_subrating_event *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_SNIFF_MODE_CONTROL_RESP):
    return TcuMngSniffModeControl_Response(buf, (enum tcu_mng_status *)return_data);
  case GET_CMD_OPCODE(TCU_MNG_EXIT_SNIFF_MODE_CONTROL_RESP):
    return TcuMngExitSniffModeControl_Response(buf, (enum tcu_mng_status *)return_data);
#endif
  default:
    printf("parsing error, unknown opcode %04x\n", opcode);
    break;
  }

  return TCU_FUNC_PARSING;
}

#define PRINT_BD_ADDR(x) \
  x[0], x[1], x[2], x[3], x[4], x[5]

static enum bt_api_result TcuMngConnectionRequestHandler(uint8_t *buf)
{
  struct tcu_mng_connection_request_event event;
  enum tcu_func_status status;
  int ret;

  status = extract_mng_data(buf, (void *)&event);
  if (status) {
    printf("extract_mng_data() failed, status %d\n", status);
    return BT_RESULT_ERROR;
  }

  printf("bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", PRINT_BD_ADDR(event.bd_addr));

  ret = bt_application_mng_connection_request_callback(event.bd_addr);
  if (ret) {
    return bt_api_connection_request_reject(&event.bd_addr[0]);
  }

  bt_api_connection_request_confirm(&event.bd_addr[0]);

  return BT_RESULT_SUCCESS;
}
enum bt_api_result bt_api_connection_request_confirm(uint8_t *bd_addr)
{
  struct tcu_mng_connection_accept_request req;
  enum tcu_mng_status response;
  enum tcu_func_status status;
  enum bt_api_result result;
  int ret =0;

  req.response_type = TCU_MNG_CONNECTION_ACCEPT;
  memcpy(&req.bd_addr[0], &bd_addr[0], TCU_SIZE_BDADDR);
#if 0
  ret = key_store_bt_retrieve_link_key(bd_addr, &req.link_key[0]);
#endif
  if (!ret) {
    req.use_of_link_key = 1;
  } else {
    req.use_of_link_key = 0;
  }

  status = TcuMngConnectionAccept_Request(&req);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngConnectionAccept_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_MNG_CONNECTION_ACCEPT_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_CONNECTION_ACCEPT_RESP failed\n");
    return result;
  }
  if (response != TCU_MNG_SUCCESSFULL) {
    printf("TCU_MNG_CONNECTION_ACCEPT_RESP failed, response %02x\n", response);
    return BT_RESULT_ERROR;
  }

  printf("bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", PRINT_BD_ADDR(bd_addr));
  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_connection_request_reject(uint8_t *bd_addr)
{
  struct tcu_mng_connection_accept_request req;
  enum tcu_mng_status response;
  enum tcu_func_status status;
  enum bt_api_result result;

  req.response_type = TCU_MNG_CONNECTION_REJECT;
  memcpy(&req.bd_addr[0], &bd_addr[0], TCU_SIZE_BDADDR);

  status = TcuMngConnectionAccept_Request(&req);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngConnectionAccept_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_MNG_CONNECTION_ACCEPT_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_CONNECTION_ACCEPT_RESP failed\n");
    return result;
  }
  if (response != TCU_MNG_SUCCESSFULL) {
    printf("TCU_MNG_CONNECTION_ACCEPT_RESP failed, status %02x\n", status);
    return BT_RESULT_ERROR;
  }

  printf("bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", PRINT_BD_ADDR(bd_addr));
  return BT_RESULT_SUCCESS;
}

static enum bt_api_result TcuMngConnectionStatusHandler(uint8_t *buf)
{
  struct tcu_mng_connection_status_event event;
  enum tcu_func_status status;

  status = extract_mng_data(buf, (void *)&event);
  if (status) {
    printf("extract_mng_data() failed, status %d\n", status);
    return BT_RESULT_ERROR;
  }

  printf("status %02x, connection_status %02x, bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", event.status, event.connection_status, event.bd_addr[0], event.bd_addr[1], event.bd_addr[2], event.bd_addr[3], event.bd_addr[4], event.bd_addr[5]);

  if (event.connection_status == TCU_MNG_CONNECTION_LINK_KEY) {
    int ret = key_store_bt_store_link_key(event.bd_addr, &event.link_key[0]);
    if (ret) {
      printf("key_store_bt_store_link_key() failed, ret %d\n", ret);
    }
  }

  /* link key failure */
  if (event.status == TCU_MNG_LINK_KEY_FAILURE) {
    int ret = key_store_bt_delete_link_key(event.bd_addr);
    if (ret) {
      printf("key_store_bt_delete_link_key() failed, ret %d\n", ret);
    }
    bt_application_spp_disconnect_callback();
  }

  return BT_RESULT_SUCCESS;
}

static enum bt_api_result TcuMngDiscoverRemoteDeviceResultHandler(uint8_t *buf)
{
  struct tcu_mng_discover_remote_dev_result_event event;
  enum tcu_func_status status;

  status = extract_mng_data(buf, (void *)&event);
  if (status) {
    printf("extract_mng_data() failed, status %d\n", status);
    return BT_RESULT_ERROR;
  }

  printf("bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", event.bd_addr[0], event.bd_addr[1], event.bd_addr[2], event.bd_addr[3], event.bd_addr[4], event.bd_addr[5]);

  /* todo: forward new remote device information to application layer for further use */
  //bt_application_mng_discover_remote_devices_callback(&event);

  return BT_RESULT_SUCCESS;
}

static enum bt_api_result TcuMngDiscoverRemoteDeviceCompleteHandler(uint8_t *buf)
{
  //bt_application_mng_discover_remote_devices_done_callback();

  return BT_RESULT_SUCCESS;
}

static enum bt_api_result TcuMngDiscoverRemoteServiceHandler(uint8_t *buf)
{
  struct tcu_mng_discover_remote_service_event event;
  enum tcu_func_status status;

  status = extract_mng_data(buf, (void *)&event);
  if (status) {
    printf("extract_mng_data() failed, status %d\n", status);
    return BT_RESULT_ERROR;
  }

  printf("status %02x, bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", event.status, event.bd_addr[0], event.bd_addr[1], event.bd_addr[2], event.bd_addr[3], event.bd_addr[4], event.bd_addr[5]);

  if (event.information_spp.valid) {
    //bt_application_mng_discover_remote_service_done_callback(&event.bd_addr[0], event.information_spp.number_of_server_channels, event.information_spp.server_channel);
  } else {
    //bt_application_mng_discover_remote_service_done_callback(&event.bd_addr[0], 0, NULL);
  }

  return BT_RESULT_SUCCESS;
}

static enum bt_api_result HCI_Encryption_Key_Refresh_Complete_Event_handler(struct tcu_mng_ssp_info_event *event)
{
  printf("\n");
  return BT_RESULT_SUCCESS;
}

static enum tcu_mng_smp_io_capability local_io_capabilty = TCU_MNG_INVALID_SETTING;
static enum tcu_mng_smp_io_capability remote_io_capabilty = TCU_MNG_INVALID_SETTING;

static enum bt_api_result HCI_IO_Capability_Response_Event_handler(struct HCI_IO_Capability_Response_Event *event)
{
  printf("io_capability %02x, authentication_requirement %02x\n", event->io_capability, event->authentication_requirement);
  remote_io_capabilty = event->io_capability;
  return BT_RESULT_SUCCESS;
}

static enum bt_api_result HCI_IO_Capability_Request_Event_handler(struct HCI_IO_Capability_Request_Event *event)
{
  struct tcu_mng_ssp_set_request request;
  struct tcu_mng_ssp_set_response response;
  enum tcu_func_status status;
  enum bt_api_result result;

  request.opcode = HCI_IO_Capability_Request_Reply_Opcode;
  request.parameter_length = 0x09;
  memcpy(&request.bd_addr[0], &event->bd_addr[0], TCU_SIZE_BDADDR);

  request.parameter.HCI_IO_Capability_Request_Reply_Parameter.oob_data_present = TCU_MNG_SMP_OOB_DATA_NOT_PRESENT;
#if 1
  bt_application_mng_capability_callback(&request.parameter.HCI_IO_Capability_Request_Reply_Parameter.io_capability,
                                         &request.parameter.HCI_IO_Capability_Request_Reply_Parameter.authentication_requirement);

  local_io_capabilty = request.parameter.HCI_IO_Capability_Request_Reply_Parameter.io_capability;
#endif
  status = TcuMngSspSet_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngSspSet_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_MNG_SSP_SET_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_SSP_SET_RESP failed\n");
    return result;
  }
  if (response.status != TCU_MNG_SUCCESSFULL) {
    printf("TCU_MNG_SSP_SET_RESP failed, status %02x\n", status);
    return BT_RESULT_ERROR;
  }

  printf("\n");
  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_mng_smp_passkey_input_request_confirm(uint8_t *bd_addr, uint32_t pass_key)
{
  struct tcu_mng_ssp_set_request request;
  struct tcu_mng_ssp_set_response response;
  enum tcu_func_status status;
  enum bt_api_result result;

  request.opcode = HCI_IO_User_Passkey_Request_Reply_Opcode;
  request.parameter_length = 0x0a;
  memcpy(&request.bd_addr[0], bd_addr, TCU_SIZE_BDADDR);
  request.parameter.HCI_IO_User_Passkey_Request_Reply.pass_key = pass_key;

  status = TcuMngSspSet_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngSspSet_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_MNG_SSP_SET_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_SSP_SET_RESP failed\n");
    return result;
  }
  if (response.status != TCU_MNG_SUCCESSFULL) {
    printf("TCU_MNG_SSP_SET_RESP failed, status %02x\n", status);
    return BT_RESULT_ERROR;
  }

  printf("\n");
  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_mng_smp_passkey_input_request_reject(uint8_t *bd_addr)
{
  struct tcu_mng_ssp_set_request request;
  struct tcu_mng_ssp_set_response response;
  enum tcu_func_status status;
  enum bt_api_result result;

  request.opcode = HCI_IO_User_Passkey_Negativ_Request_Reply_Opcode;
  request.parameter_length = 0x06;
  memcpy(&request.bd_addr[0], bd_addr, TCU_SIZE_BDADDR);

  status = TcuMngSspSet_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngSspSet_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_MNG_SSP_SET_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_SSP_SET_RESP failed\n");
    return result;
  }
  if (response.status != TCU_MNG_SUCCESSFULL) {
    printf("TCU_MNG_SSP_SET_RESP failed, status %02x\n", response.status);
    return BT_RESULT_ERROR;
  }

  printf("\n");
  return BT_RESULT_SUCCESS;
}

static enum bt_api_result HCI_User_Passkey_Request_handler(struct tcu_mng_ssp_info_event *event)
{
  int ret = 0;

//  ret = bt_application_mng_smp_passkey_input_request_callback(event->details.HCI_User_Passkey_Request_Events.bd_addr);
  if (ret) {
    return bt_api_mng_smp_passkey_input_request_reject(event->details.HCI_User_Passkey_Request_Events.bd_addr);
  }

  printf("\n");
  return BT_RESULT_SUCCESS;
}

static enum bt_api_result HCI_User_Passkey_Notification_handler(struct tcu_mng_ssp_info_event *event)
{
  //bt_application_mng_smp_show_pass_key_callback(event->details.HCI_User_Passkey_Notification_Events.bd_addr, event->details.HCI_User_Passkey_Notification_Events.pass_key);

  printf("\n");
  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_mng_smp_user_confirmation_accept(uint8_t *bd_addr)
{
  struct tcu_mng_ssp_set_request request;
  struct tcu_mng_ssp_set_response response;
  enum tcu_func_status status;
  enum bt_api_result result;

  request.opcode = HCI_User_Confirmation_Request_Reply_Opcode;
  request.parameter_length = 0x06;
  memcpy(&request.bd_addr[0], bd_addr, TCU_SIZE_BDADDR);

  status = TcuMngSspSet_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngSspSet_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_MNG_SSP_SET_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_SSP_SET_RESP failed\n");
    return result;
  }
  if (response.status != TCU_MNG_SUCCESSFULL) {
    printf("TCU_MNG_SSP_SET_RESP failed, status %02x\n", status);
    return BT_RESULT_ERROR;
  }

  printf("\n");
  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_mng_smp_user_confirmation_reject(uint8_t *bd_addr)
{
  struct tcu_mng_ssp_set_request request;
  struct tcu_mng_ssp_set_response response;
  enum tcu_func_status status;
  enum bt_api_result result;

  request.opcode = HCI_User_Confirmation_Negative_Reply_Opcode;
  request.parameter_length = 0x06;
  memcpy(&request.bd_addr[0], bd_addr, TCU_SIZE_BDADDR);

  status = TcuMngSspSet_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngSspSet_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_MNG_SSP_SET_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_SSP_SET_RESP failed\n");
    return result;
  }
  if (response.status != TCU_MNG_SUCCESSFULL) {
    printf("TCU_MNG_SSP_SET_RESP failed, status %02x\n", status);
    return BT_RESULT_ERROR;
  }

  printf("\n");
  return BT_RESULT_SUCCESS;
}

const uint8_t automatic_confirmation_table[4][4] = {
    {
     1, 1, 0, 1,
    },
    {
     0, 0, 0, 0,
    },
    {
     0, 0, 0, 1,
    },
    {
     1, 1, 1, 1,
    },
};

static enum bt_api_result HCI_IO_User_Confirmation_Request_Event_handler(struct HCI_IO_User_Confirmation_Request_Event *event)
{
  uint8_t automatic_confirmation = automatic_confirmation_table[local_io_capabilty][remote_io_capabilty];
  int ret =0;

  printf("automatic_confirmation %d\n", automatic_confirmation);

  if (automatic_confirmation) {
    return bt_api_mng_smp_user_confirmation_accept(event->bd_addr);
  }

  if (local_io_capabilty == TCU_MNG_DISPLAY_ONLY) {
    //bt_application_mng_smp_show_pass_key_callback(event->bd_addr, event->pass_key);
  }

  if (local_io_capabilty == TCU_MNG_KEYBOARD_ONLY || local_io_capabilty == TCU_MNG_DISPLAY_YES_NO) {
   // ret = bt_application_mng_smp_confirmation_request_callback(event->bd_addr, event->pass_key);
    if (ret) {
      printf("bt_application_mng_smp_confirmation_request_callback() failed\n");
     // return bt_api_mng_smp_user_confirmation_reject(event->bd_addr);
    }
  }

  return BT_RESULT_SUCCESS;
}

static enum bt_api_result HCI_Simple_Pairing_Complete_Event_handler(struct HCI_Simple_Pairing_Complete_Event *event)
{
  uint8_t automatic_confirmation = automatic_confirmation_table[local_io_capabilty][remote_io_capabilty];

  if (!automatic_confirmation) {
    if (local_io_capabilty == TCU_MNG_DISPLAY_ONLY || local_io_capabilty == TCU_MNG_DISPLAY_YES_NO) {
     // bt_application_mng_smp_hide_pass_key_callback();
    }
  }

  if (event->status) {
   // bt_application_mng_smp_pairing_failed_callback();
  } else {
   // bt_application_mng_smp_pairing_success_callback();
  }

  local_io_capabilty = TCU_MNG_INVALID_SETTING;
  remote_io_capabilty = TCU_MNG_INVALID_SETTING;

  printf("status %02x\n", event->status);
  return BT_RESULT_SUCCESS;
}

static enum bt_api_result TcuMngSspInfoHandler(uint8_t *buf)
{
  struct tcu_mng_ssp_info_event event;
  enum tcu_func_status status;

  status = extract_mng_data(buf, (void *)&event);
  if (status) {
    printf("extract_mng_data() failed, status %d\n", status);
    return BT_RESULT_ERROR;
  }

  printf("opcode %02x\n", event.opcode);

  /* todo: handling */
  switch (event.opcode) {
  case HCI_Encryption_Key_Refresh_Complete_Event:
    return HCI_Encryption_Key_Refresh_Complete_Event_handler(&event);
  case HCI_IO_Capability_Request_Event:
    return HCI_IO_Capability_Request_Event_handler(&event.details.HCI_IO_Capability_Request_Event);
  case HCI_IO_Capability_Response_Event:
    return HCI_IO_Capability_Response_Event_handler(&event.details.HCI_IO_Capability_Response_Event);
  case HCI_Simple_Pairing_Complete_Event:
    return HCI_Simple_Pairing_Complete_Event_handler(&event.details.HCI_Simple_Pairing_Complete_Event);
  case HCI_User_Passkey_Request:
    return HCI_User_Passkey_Request_handler(&event);
  case HCI_User_Passkey_Notification:
    return HCI_User_Passkey_Notification_handler(&event);
  case HCI_IO_User_Confirmation_Request_Event:
    return HCI_IO_User_Confirmation_Request_Event_handler(&event.details.HCI_IO_User_Confirmation_Request_Event);
  default:
    break;
  }

  return BT_RESULT_ERROR;
}

static enum bt_api_result reject_pin_request(uint8_t *bd_addr)
{
  struct tcu_mng_pin_write_request request;
  enum tcu_mng_status response;
  enum tcu_func_status status;
  enum bt_api_result result;

  memcpy(&request.bd_addr[0], bd_addr, TCU_SIZE_BDADDR);
  request.length_of_pin_code = 0;
  status = TcuMngPinWrite_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngConnectionAccept_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_MNG_PIN_WRITE_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_PIN_WRITE_RESP failed\n");
    return result;
  }
  if (response != TCU_MNG_SUCCESSFULL) {
    printf("TCU_MNG_PIN_WRITE_RESP failed, status %02x\n", status);
    return BT_RESULT_ERROR;
  }

  printf("\n");
  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_mng_pin_write_confirm(uint8_t *bd_addr, uint8_t *pin_code, uint8_t pin_code_length)
{
  struct tcu_mng_pin_write_request request;
  enum tcu_mng_status response;
  enum tcu_func_status status;
  enum bt_api_result result;

  memcpy(&request.bd_addr[0], bd_addr, TCU_SIZE_BDADDR);
  request.length_of_pin_code = pin_code_length;
  memcpy(&request.pin_code, pin_code, pin_code_length);
  status = TcuMngPinWrite_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngConnectionAccept_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_MNG_PIN_WRITE_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_PIN_WRITE_RESP failed\n");
    return result;
  }
  if (response != TCU_MNG_SUCCESSFULL) {
    printf("TCU_MNG_PIN_WRITE_RESP failed, status %02x\n", status);
    return BT_RESULT_ERROR;
  }

  printf("\n");
  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_mng_pin_write_reject(uint8_t *bd_addr)
{
  printf("\n");
  return reject_pin_request(bd_addr);
}

static enum bt_api_result TcuMngPinRequestHandler(uint8_t *buf)
{
#if 0
  struct tcu_mng_pin_request_event event;
  enum tcu_func_status status;
  int ret;

  status = extract_mng_data(buf, (void *)&event);
  if (status) {
    printf("extract_mng_data() failed, status %d\n", status);
    return BT_RESULT_ERROR;
  }

  ret = bt_application_mng_pin_write_request_callback(event.bd_addr);
  if (!ret) {
    printf("\n");
    return BT_RESULT_SUCCESS;
  }

  /* reject pin request immediately */
  return reject_pin_request(event.bd_addr);
#endif
  return 0;
}

static enum bt_api_result TcuMngRemoreDevNameAutoNotifyHandler(uint8_t *buf)
{
#if 0
  struct tcu_mng_remote_device_name_auto_notify_event event;
  enum tcu_func_status status;

  status = extract_mng_data(buf, (void *)&event);
  if (status != TCU_FUNC_SUCCESS) {
    return BT_RESULT_ERROR;
  }

  bt_application_mng_device_name_notification_callback(&event.bd_addr[0], event.device_name, event.length_of_device_name);

  printf("bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", event.bd_addr[0], event.bd_addr[1], event.bd_addr[2], event.bd_addr[3], event.bd_addr[4], event.bd_addr[5]);
#endif
  return BT_RESULT_SUCCESS;
}

#if 0
static enum bt_api_result TcuMngSniffSubratingHandler(uint8_t *buf)
{
  struct tcu_mng_rcv_sniff_subrating_event event;
  enum tcu_func_status status;

  status = extract_mng_data(buf, (void *)&event);
  if (status) {
    printf("extract_mng_data() failed, status %d\n", status);
    return BT_RESULT_ERROR;
  }

  bt_api_mng_sniff_mode_changed_callback(&event);

  return BT_RESULT_SUCCESS;
}
#endif

enum bt_api_result bt_api_mng_init(uint8_t const *device_name, uint8_t device_name_len)
{
  struct tcu_mng_init_request request;
  struct tcu_mng_init_response response;
  enum tcu_func_status status;
  enum bt_api_result result;

  request.device_name = device_name;
  request.device_name_len = device_name_len;

  status = TcuMngInit_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngInit_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_MNG_INIT_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_INIT_RESP failed\n");
    return result;
  }

  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_mng_set_scan(enum bt_api_mng_scan_visibility visible, enum bt_api_mng_scan_discoverability discoverable)
{
  enum tcu_func_status status;
  struct tcu_mng_set_scan_request req;
  struct tcu_mng_set_scan_response response;
  enum bt_api_result result;

  uint8_t scan_mode;

  scan_mode = (uint8_t)(visible << 1);
  scan_mode |= (uint8_t)discoverable;

  memset(&req, 0, sizeof(req));
  req.scan_mode = scan_mode;

  status = TcuMngSetScan_Request(&req);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngSetScan_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_MNG_SET_SCAN_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_SET_SCAN_RESP failed\n");
    return result;
  }
  if (response.status != 0x00) {
    printf("TCU_MNG_SET_SCAN_RESP failed, status %02x\n", status);
    return BT_RESULT_ERROR;
  }

  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_mng_discover_remote_devices(void)
{
  enum tcu_func_status status;
  struct tcu_mng_discover_remote_device_request request;
  struct tcu_accept_event event;
  enum bt_api_result result;

  request.max_number_of_reports = 0x10;

  status = TcuMngDiscoverRemoteDevice_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngConnectionAccept_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_ACCEPT, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&event));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_ACCEPT failed\n");
    return result;
  }
  if (event.status != TCU_MNG_SUCCESSFULL) {
    printf("TCU_ACCEPT failed, status %02x\n", event.status);
    return BT_RESULT_ERROR;
  }
  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_mng_discover_remote_service(uint8_t *bd_addr)
{
  struct tcu_mng_discover_remote_service_request request;
  enum tcu_func_status status;
  struct tcu_accept_event event;
  enum bt_api_result result;
  int ret =0;

  printf("bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", PRINT_BD_ADDR(bd_addr));

  memcpy(request.bd_addr, bd_addr, TCU_SIZE_BDADDR);
#if 0
  ret = key_store_bt_retrieve_link_key(bd_addr, request.link_key);
#endif
  if (!ret) {
    request.use_of_link_key = 1;
  } else {
    request.use_of_link_key = 0;
  }
  request.security_Mode = 0;

  status = TcuMngDiscoverRemoteService_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngDiscoverRemoteService_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_ACCEPT, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&event));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_ACCEPT failed\n");
    return result;
  }
  if (event.status != TCU_MNG_SUCCESSFULL) {
    printf("TCU_ACCEPT failed, status %02x\n", event.status);
    return BT_RESULT_ERROR;
  }
  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_mng_read_rssi(uint8_t *bd_addr, uint16_t *rssi)
{
  struct tcu_mng_read_rssi_request request;
  struct tcu_mng_read_rssi_response response;
  enum tcu_func_status status;
  enum bt_api_result result;

  memcpy(request.bd_addr, bd_addr, TCU_SIZE_BDADDR);

  status = TcuMngReadRssi_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngReadRssi_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_MNG_READ_RSSI_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_READ_RSSI_RESP failed\n");
    return result;
  }
  if (response.status != TCU_MNG_SUCCESSFULL) {
    printf("TCU_MNG_READ_RSSI_RESP failed, status %02x\n", status);
    return BT_RESULT_ERROR;
  }
  *(uint16_t *)rssi = response.rssi;
  return BT_RESULT_SUCCESS;
}

#if 0
enum bt_api_result bt_api_mng_sniff_mode_enable(struct tcu_mng_sniff_mode_control_request *request)
{
  enum tcu_func_status status;
  enum tcu_mng_status response;

  status = TcuMngSniffModeControl_Request(request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngSniffModeControl_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  status = bt_wait_for_specific_event(TCU_MNG_SNIFF_MODE_CONTROL_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (status != TCU_FUNC_SUCCESS) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_SNIFF_MODE_CONTROL_RESP failed\n");
    return BT_RESULT_ERROR;
  }
  if (response != TCU_MNG_SUCCESSFULL) {
    printf("TCU_MNG_SNIFF_MODE_CONTROL_RESP failed, status %02x\n", status);
    return BT_RESULT_ERROR;
  }
  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_mng_sniff_mode_disable(uint8_t *bd_addr)
{
  enum tcu_func_status status;
  enum tcu_mng_status response;

  status = TcuMngExitSniffModeControl_Request(bd_addr);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngExitSniffModeControl_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  status = bt_wait_for_specific_event(TCU_MNG_EXIT_SNIFF_MODE_CONTROL_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (status != TCU_FUNC_SUCCESS) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_EXIT_SNIFF_MODE_CONTROL_RESP failed\n");
    return BT_RESULT_ERROR;
  }
  if (response != TCU_MNG_SUCCESSFULL) {
    printf("TCU_MNG_EXIT_SNIFF_MODE_CONTROL_RESP failed, status %02x\n", status);
    return BT_RESULT_ERROR;
  }
  return BT_RESULT_SUCCESS;
}
#endif

enum bt_api_result bt_api_mng_set_sdp_for_di(struct tcu_mng_set_di_sdp_record_request *config)
{
  enum tcu_func_status status;
  enum bt_api_result result;
  enum tcu_mng_status response;

  status = TcuMngSetDiSdpRecord_Request(config);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuMngSetDiSdpRecord_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_MNG_SET_DI_SDP_RECORD_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_SET_DI_SDP_RECORD_RESP failed\n");
    return result;
  }
  if (response) {
    printf("status () @ TCU_MNG_SET_DI_SDP_RECORD_RESP failed\n");
    return BT_RESULT_ERROR;
  }

  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_mng_set_class_of_device(uint8_t *class_of_device)
{
  uint8_t request[] = {0x0d, 0x00, 0x00, 0xe1, 0x3d, 0x06, 0x00, 0x24, 0x0c, 0x03, 0x00, 0x04, 0x20};
  struct tcu_mng_ssp_set_response response;
  enum bt_api_result result;

  request[10] = class_of_device[0];
  request[11] = class_of_device[1];
  request[12] = class_of_device[2];

  tcu_application_send(&request[0], sizeof(request));

  result = bt_wait_for_specific_event(TCU_MNG_SSP_SET_RESP, TCU_MNG_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("bt_wait_for_specific_event() @ TCU_MNG_SSP_SET_RESP failed\n");
    return BT_RESULT_ERROR;
  }

  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_mng_event_handler(uint8_t *buf)
{
  uint16_t command = PARSE_CMD_MESSAGE(buf);

  switch (command) {
  case GET_CMD_MESSAGE(TCU_MNG_CONNECTION_REQUEST_EVENT):
    return TcuMngConnectionRequestHandler(buf);
  case GET_CMD_MESSAGE(TCU_MNG_CONNECTION_STATUS_EVENT):
    return TcuMngConnectionStatusHandler(buf);
  case GET_CMD_MESSAGE(TCU_MNG_REMOTE_DEVICE_NAME_AUTO_NOTIFY_EVENT):
    return TcuMngRemoreDevNameAutoNotifyHandler(buf);
  case GET_CMD_MESSAGE(TCU_MNG_DISCOVER_REMOTE_DEVICE_RESULT_EVENT):
    return TcuMngDiscoverRemoteDeviceResultHandler(buf);
  case GET_CMD_MESSAGE(TCU_MNG_DISCOVER_REMOTE_DEVICE_COMPLETE_EVENT):
    return TcuMngDiscoverRemoteDeviceCompleteHandler(buf);
  case GET_CMD_MESSAGE(TCU_MNG_DISCOVER_REMOTE_SERVICE_EVENT):
    return TcuMngDiscoverRemoteServiceHandler(buf);
  case GET_CMD_MESSAGE(TCU_MNG_SSP_INFO_EVENT):
    return TcuMngSspInfoHandler(buf);
  case GET_CMD_MESSAGE(TCU_MNG_PIN_REQUEST_EVENT):
    return TcuMngPinRequestHandler(buf);
#if 0
  case GET_CMD_MESSAGE(TCU_MNG_RCV_SNIFF_SUBRATING_EVENT):
    return TcuMngSniffSubratingHandler(buf);
#endif

  default:
    printf("unhandled BT MNG event 0x%04x received\n", command);
    break;
  }

  printf("parsing error, unknown command %04x\n", command);
  return BT_RESULT_ERROR;
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
