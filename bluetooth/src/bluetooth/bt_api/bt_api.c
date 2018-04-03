/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/
#include <stdio.h>
#include "bt_api.h"
#include "bt_api_internal.h"
#include "tcu_api.h"
//#include "debug_api.h"

/* Defining this macro disables all debug messages. */
#define printf(fmt, args...)	/* NOPRINTF */

#ifdef __ICCARM__
__weak enum tcu_func_status extract_spp_data(uint8_t *buf, void *data)
{
  return TCU_FUNC_ERROR;
}
#endif


static enum tcu_func_status extract_bt_data(uint8_t *buf, void *data)
{
  uint8_t service_id = PARSE_CMD_SERVICE_ID(buf);
  enum tcu_func_status status;
	printf("Service ID =%d\n",service_id);
  switch (service_id) {
  case TCU_BT_SERVICE_MNG:
    status = extract_mng_data(buf, data);
    break;
  case TCU_BT_SERVICE_SPP:
    status = extract_spp_data(buf, data);
    break;
  case TCU_BT_SERVICE_VEN:
    status = extract_ven_data(buf, data);
    break;
  default:
    return TCU_FUNC_ERROR;
  }

  return status;
}

enum bt_api_result bt_wait_for_specific_event(uint32_t expected_event, uint32_t timeout_ms, void *ptr)
{
  uint8_t *buf = tcu_buffer;
  uint8_t expected_service_id = GET_CMD_SERVICE_ID(expected_event);
  uint8_t expected_opcode = GET_CMD_OPCODE(expected_event);
  enum tcu_func_status status;
  int ret;


  ret = tcu_application_wait_for_specific_event(expected_service_id, expected_opcode, timeout_ms, buf, tcu_buffer_len);
  if (ret) {
    printf("tcu_application_wait_for_specific_event() failed\n");
    return BT_RESULT_ERROR;
  }

  status = extract_bt_data(buf, ptr);
  if (status) {
    printf("extract_bt_data() failed, status %d\n", status);
    return BT_RESULT_ERROR;
  }

  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_event_handler(uint8_t *data)
{
  uint8_t service_id = PARSE_CMD_SERVICE_ID(data);
  enum bt_api_result ret;

  switch (service_id) {
  case TCU_BT_SERVICE_VEN:
    ret = bt_ven_event_handler(data);
    if (ret != BT_RESULT_SUCCESS) {
      printf("bt_ven_event_handler() failed, ret %d\n", ret);
    }
    return ret;
  case TCU_BT_SERVICE_MNG:
    ret = bt_mng_event_handler(data);
    if (ret != BT_RESULT_SUCCESS) {
      printf("bt_mng_event_handler() failed, ret %d\n", ret);
    }
    return ret;
  case TCU_BT_SERVICE_SPP:
    ret = bt_spp_event_handler(data);
    if (ret != BT_RESULT_SUCCESS) {
      printf("bt_spp_event_handler() failed, ret %d\n", ret);
    }
    return ret;

  default: {
    break;
  }
  }

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
