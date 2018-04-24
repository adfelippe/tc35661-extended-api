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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "hci_api.h"
#include "hci_api_internal.h"
#include "bluetooth_serial.h"
//#include "debug_api.h"

/* Defining this macro disables all debug messages. */
//#define printf(fmt, args...)	/* NOPRINTF */

#define SET_OCF_OGF(x) \
  (x >> 8) & 0xff, (x & 0xff)

#define OCF_OGF_HCI_RESET 0x030C
#define OCF_OGF_HCI_WRITE_BD_ADDRESS 0x1310
#define OCF_GF_HCI_WRITE_SCAN_ENABLE 0x1A0C
#define OCF_GF_HCI_WRITE_PAGE_SCAN_ACTIVITY 0x1C0C
#define OCF_GF_HCI_SET_EVENT_FILTER 0x050C
#define OCF_GF_HCI_ENABLE_DEVICE_UNDER_TEST_MODE 0x0318
#define OCF_OGF_HCI_LOC_OPERATION_MODE_SET 0x03FC
#define OCF_OGF_HCI_SET_HOPPING_MODE 0x08FC
#define OCF_OGF_HCI_LOC_SET_WHITENING_MODE 0x08FC
#define OCF_OGF_HCI_NON_MODULATED_CARRIER 0x03FC
#define OCF_OGF_HCI_LOC_TX_MODE_SET 0x03FC
#define OCF_OGF_HCI_LOC_TX_SLOT_LENGTH 0x03FC
#define OCF_OGF_HCI_LOC_SENDER_RECEIVER 0x03FC
#define OCF_OGF_HCI_SET_AFH_HOST_CHANNEL 0x3F0C


#define HCI_EVENT_CODE_COMMAND_COMPLETE_EVENT 0x0E
#define HCI_EVENT_CODE_COMMAND_COMPLETE_EXT_EVENT 0x0F

#define CMD_I2C_ENABLE 0x5b
#define CMD_EEPROM_WRITE_ENABLE 0x83
#define CMD_EEPROM_READ 0x88
#define CMD_EEPROM_WRITE 0x89
#define CMD_GET_FIRMWARE_VERSION 0x0d
#define CMD_SET_DEEP_SLEEP 0x68
#define CMD_BLE_SET_RTSCTS 0xa2

#define HCI_RTS_CTS 0x93
#define HCI_SET_MODE 0x99
#define HCI_LOC_DBUS_WRITE_CMD 0xc2

#define M2_SET_CMD 0xa0
#define M2_GET_CMD 0xa1

#define EXT_EVENT_MAX_SIZE 10

static int check_m2(uint8_t cmd);

struct hci_command_complete_event
{
  uint8_t packet_type;
  uint8_t event_code;
  uint8_t command_length;
  uint8_t num_hci_packets;
  uint16_t ocf_ogf;
  uint8_t status;
  uint8_t command;
};

struct hci_command_complete_ext_event
{
  uint8_t packet_type;
  uint8_t event_code;
  uint8_t command_length;
  uint8_t status;
  uint8_t num_hci_packets;
  uint16_t ocf_ogf;
  uint8_t command;
};

struct hci_command_status_event
{
  uint8_t packet_type;
  uint8_t event_code;
  uint8_t command_length;
  uint8_t status;
  uint8_t num_hci_packets;
  uint8_t ocf;
  uint8_t ocf_ogf;
};

struct hci_command_event
{
  uint8_t packet_type;
  uint8_t event_code;
  uint8_t command_length;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t command;
  uint8_t status;
};

struct hci_m2_xet_event
{
  uint8_t packet_type;
  uint8_t event_code;
  uint8_t command_length;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t command;
  uint8_t reserved[3];
  uint8_t acceptor;
  uint8_t information_id;
  uint8_t command_result;
  uint8_t data_type;
  uint8_t *data;
};

uint8_t extended_event_data[EXT_EVENT_MAX_SIZE];

static int parse_hci_command_complete_event(uint8_t *buf, int len, struct hci_command_complete_event *event)
{

  if (len != 7 && len != 8) {
    printf("event length is different than expected (7 or 8)\n");
    return -1;
  }

  event->packet_type = *buf++;
  if (event->packet_type != HCI_PACKET_TYPE_EVENT_PACKET) {
    printf("HCI_PACKET_TYPE_EVENT_PACKET failed\n");
    return -1;
  }

  event->event_code = *buf++;
  if (event->event_code != HCI_EVENT_CODE_COMMAND_COMPLETE_EVENT) {
    printf("HCI_EVENT_CODE_COMMAND_COMPLETE_EVENT failed: 0x%.2X\n", event->event_code);
    return -1;
  }

  event->command_length = *buf++;
  if (event->command_length != 0x04 && event->command_length != 0x05) {
    printf("expected length != 4 OR 5\n");
    return -1;
  }
  event->num_hci_packets = *buf++;
  event->ocf_ogf = ((buf[0] << 8) | buf[1]);
  buf += 2;
  event->status = *buf++;
  event->command = *buf++;

  return 0;
}

static int parse_hci_command_complete_ext_event(uint8_t *buf, int len, struct hci_command_complete_ext_event *event)
{

  if (len != 7 && len != 8) {
    printf("event length is different than expected (7 or 8)\n");
    return -1;
  }

  event->packet_type = *buf++;
  if (event->packet_type != HCI_PACKET_TYPE_EVENT_PACKET) {
    printf("HCI_PACKET_TYPE_EVENT_PACKET failed\n");
    return -1;
  }

  event->event_code = *buf++;
  if (event->event_code != HCI_EVENT_CODE_COMMAND_COMPLETE_EXT_EVENT) {
    printf("HCI_EVENT_CODE_COMMAND_COMPLETE_EXT_EVENT failed: 0x%.2X\n", event->event_code);
    return -1;
  }

  event->command_length = *buf++;
  if (event->command_length != 0x04 && event->command_length != 0x05) {
    printf("expected length != 4 OR 5\n");
    return -1;
  }
  event->status = *buf++;
  event->num_hci_packets = *buf++;
  event->ocf_ogf = ((buf[0] << 8) | buf[1]);
  buf += 2;
  event->command = *buf++;

  return 0;
}

static int parse_hci_command_event(uint8_t *buf, int len, struct hci_command_event *event)
{
  event->packet_type = *buf++;
  if (event->packet_type != HCI_PACKET_TYPE_EVENT_PACKET) {
    printf("HCI_PACKET_TYPE_EVENT_PACKET failed\n");
    return -1;
  }
  event->event_code = *buf++;
  event->command_length = *buf++;
  event->reserved1 = *buf++;
  event->reserved2 = *buf++;
  event->command = *buf++;
  event->status = *buf++;

  return 0;
}

int parse_hci_m2_xet_event(uint8_t *buf, int len, struct hci_m2_xet_event *event)
{
  event->packet_type = *buf++;

  if(event->packet_type != HCI_PACKET_TYPE_EVENT_PACKET)
  {
    printf("HCI_PACKET_TYPE_EVENT_PACKET failed\n");
    return -1;
  }

  event->event_code = *buf++;

  if(event->event_code != 0xff)
  {
    printf("0xff failed\n");
    return -1;
  }

  event->command_length = *buf++;

  event->reserved1 = *buf++;
  event->reserved2 = *buf++;
  event->command = *buf++;
  event->reserved[0] = *buf++;
  event->reserved[1] = *buf++;
  event->reserved[2] = *buf++;
  event->acceptor = *buf++;
  event->information_id = *buf++;
  event->command_result = *buf++;
  event->data_type = *buf++;
  event->data = buf;

  return 0;
}

static int wait_for_command_complete_event(uint16_t expected_ocf_ogf)
{
  int len;
  uint8_t *ptr = hci_buffer;
  struct hci_command_complete_event event;
  int ret;


  len = hci_application_receive_response(ptr, hci_buffer_len);

  if (len < 0) {
    printf("hci_receive_response() failed\n");
    return -1;
  }

  ret = parse_hci_command_complete_event(ptr, len, &event);

  if (ret) {
    printf("parse_hci_command_complete_event() failed\n");
    return -1;
  }

  if (event.ocf_ogf != expected_ocf_ogf) {
    printf("expected ocf 0x%02x, got 0x%0x2\n", expected_ocf_ogf, event.ocf_ogf);
    return -1;
  }

  if (event.status) {
    printf("status 0x%02x is not 0\n", ptr[6]);
    return -1;
  }

  if (len == 7) {
  	printf("<EVENT> %.2X %.2X %.2X %.2X %.2X %.2X %.2X\n",
  		event.packet_type,
		event.event_code,
		event.command_length,
		event.num_hci_packets,
		(uint8_t)(event.ocf_ogf >> 8),
		(uint8_t)(event.ocf_ogf & 0xFF),
		event.status);
  } else if (len == 8) {
	printf("<EVENT> %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X\n",
	 	event.packet_type,
		event.event_code,
		event.command_length,
		event.num_hci_packets,
		(uint8_t)(event.ocf_ogf >> 8),
		(uint8_t)(event.ocf_ogf & 0xFF),
		event.status,
		event.command);
  } else {
	  printf("<EVENT> size is different than expected!\n");
	  return -1;
  }

  return 0;
}

static int wait_for_command_complete_ext_event(uint16_t expected_ocf_ogf)
{
  int len;
  uint8_t *ptr = hci_buffer;
  struct hci_command_complete_ext_event event;
  uint8_t extended_event_data[9];
  int ret, i;

  len = hci_application_receive_ext_response(ptr, hci_buffer_len);

  if (len < 0) {
    printf("hci_application_receive_ext_response() failed\n");
    return -1;
  }

  ret = parse_hci_command_complete_ext_event(ptr, len, &event);

  if (ret) {
    printf("parse_hci_command_complete_ext_event() failed\n");
    return -1;
  }

  if (event.ocf_ogf != expected_ocf_ogf) {
    printf("expected ocf 0x%02x, got 0x%0x2\n", expected_ocf_ogf, event.ocf_ogf);
    return -1;
  }

  if (event.status) {
    printf("status 0x%02x is not 0\n", ptr[6]);
    return -1;
  }

  if (len == 7) {
  	printf("<EVENT> %.2X %.2X %.2X %.2X %.2X %.2X %.2X\n",
  		event.packet_type,
		event.event_code,
		event.command_length,
		event.status,
		event.num_hci_packets,
		(uint8_t)(event.ocf_ogf >> 8),
		(uint8_t)(event.ocf_ogf & 0xFF));
  } else if (len == 8) {
	printf("<EVENT> %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X\n",
	 	event.packet_type,
		event.event_code,
		event.command_length,
		event.status,
		event.num_hci_packets,
		(uint8_t)(event.ocf_ogf >> 8),
		(uint8_t)(event.ocf_ogf & 0xFF),
		event.command);
  } else {
	  printf("<EVENT> size is different than expected!\n");
	  return -1;
  }

  // Get next round of the EXT Event
  for (i = 0; i < 9; i++) {
	  ret = bt_read(&extended_event_data[i], 1, 250);
  }
  printf("<EXT_EVENT> ");
  for (i = 0; i < 9; i++) {
    printf("%.2X ", extended_event_data[i]);
  }
  printf("\n");

  if (ret >= 0)
    return 0;
  else
    return -1;
}

int hci_api_reset(void)
{
  uint8_t request[] = {0x01, SET_OCF_OGF(OCF_OGF_HCI_RESET), 0x00};
  int ret;

  printf("hci_api_reset...\n");
  ret = hci_application_send_request(&request[0], sizeof(request));

  if(ret)
  {
    printf("hci_send_request() failed\n");
    return -1;
  }

  ret = wait_for_command_complete_event(OCF_OGF_HCI_RESET);

  if(ret)
  {
    printf("wait_for_command_complete_event() failed\n");
    return ret;
  }

  return 0;
}

int hci_api_loc_set_whitening_mode(uint8_t mode)
{
	uint8_t request[] = {0x01, SET_OCF_OGF(OCF_OGF_HCI_LOC_SET_WHITENING_MODE), 0x03, 0x00, 0x21, mode};
	int ret = 0;

	printf("hci_api_loc_set_whitening_mode...\n");
	ret = hci_application_send_request(&request[0], sizeof(request));
	if(ret) {
          printf("hci_api_loc_set_whitening_mode FAILED\n");
          return -1;
        }

	ret = wait_for_command_complete_event(OCF_OGF_HCI_LOC_SET_WHITENING_MODE);
	if(ret) {
          printf("wait_for_command_complete_event() FAILED...\n");
          return ret;
        }
	printf("hci_api_loc_set_whitening_mode SUCCEEDED!\n");
	return 0;
}

int hci_api_set_afh_host_channel_classification(uint8_t* channel_mask)
{
	uint8_t request[] = {0x01, SET_OCF_OGF(OCF_OGF_HCI_SET_AFH_HOST_CHANNEL), 0x0A,
			     	channel_mask[0], channel_mask[1], channel_mask[2],
		     		channel_mask[3], channel_mask[4], channel_mask[5],
				channel_mask[6], channel_mask[7], channel_mask[8],
				channel_mask[9]};
	int ret = 0;

	printf("hci_api_set_afh_host_channel_classification...\n");
	ret = hci_application_send_request(&request[0], sizeof(request));
	if(ret) {
		printf("hci_api_set_afh_host_channel_classification FAILED...\n");
          	return -1;
        }

	ret = wait_for_command_complete_event(OCF_OGF_HCI_SET_AFH_HOST_CHANNEL);
	if(ret) {
		printf("wait_for_command_complete_event() failed\n");
		return ret;
        }
	printf("hci_api_set_afh_host_channel_classification SUCCEEDED!\n");
	return 0;
}

int hci_api_set_hopping_mode(uint8_t mode, uint8_t frequency)
{
	uint8_t request[] = {0x01, SET_OCF_OGF(OCF_OGF_HCI_SET_HOPPING_MODE), 0x04, 0x00, 0x20, mode, frequency};
	int ret = 0;

	printf("hci_api_set_hopping_mode...\n");
	ret = hci_application_send_request(&request[0], sizeof(request));
	if(ret) {
          printf("hci_api_set_hopping_mode FAILED\n");
          return -1;
        }

	ret = wait_for_command_complete_event(OCF_OGF_HCI_SET_HOPPING_MODE);
	if(ret) {
          printf("wait_for_command_complete_event() FAILED...\n");
          return ret;
        }
	printf("hci_api_set_hopping_mode SUCCEEDED!\n");
	return 0;
}

int hci_api_loc_operation_mode_set(void)
{
	uint8_t request[] = {0x01, SET_OCF_OGF(OCF_OGF_HCI_LOC_OPERATION_MODE_SET), 0x03, 0x00, 0x0B, 0x1B};
	int ret = 0;

	printf("hci_api_loc_operation_mode_set...\n");
	ret = hci_application_send_request(&request[0], sizeof(request));
	if(ret) {
          printf("hci_api_loc_operation_mode_set FAILED\n");
          return -1;
        }

	ret = wait_for_command_complete_ext_event(OCF_OGF_HCI_LOC_OPERATION_MODE_SET);
	if(ret) {
          printf("wait_for_command_complete_event() FAILED...\n");
          return ret;
        }
	printf("hci_api_loc_operation_mode_set SUCCEEDED!\n");
	return 0;
}

int hci_api_non_modulated_carrier(uint8_t frequency)
{
	uint8_t request[] = {0x01, SET_OCF_OGF(OCF_OGF_HCI_NON_MODULATED_CARRIER), 0x04, 0x00, 0x0B, 0x1C, frequency};
	int ret = 0;

	printf("hci_api_non_modulated_carrier...\n");
	ret = hci_application_send_request(&request[0], sizeof(request));
	if(ret) {
          printf("hci_api_non_modulated_carrier FAILED\n");
          return -1;
        }

	ret = wait_for_command_complete_ext_event(OCF_OGF_HCI_NON_MODULATED_CARRIER);
	if(ret) {
          printf("wait_for_command_complete_ext_event FAILED...\n");
          return ret;
        }
	printf("hci_api_non_modulated_carrier SUCCEEDED!\n");
	return 0;
}

int hci_api_loc_tx_mode_set(uint8_t payload)
{
	uint8_t request[] = {0x01, SET_OCF_OGF(OCF_OGF_HCI_LOC_TX_MODE_SET), 0x04, 0x00, 0x08, payload, 0x09};
	int ret = 0;

	printf("hci_api_loc_tx_mode_set...\n");
	ret = hci_application_send_request(&request[0], sizeof(request));
	if(ret) {
          printf("hci_api_loc_tx_mode_set FAILED\n");
          return -1;
        }

	ret = wait_for_command_complete_ext_event(OCF_OGF_HCI_LOC_TX_MODE_SET);
	if(ret) {
          printf("wait_for_command_complete_ext_event() FAILED...\n");
          return ret;
        }
	printf("hci_api_loc_tx_mode_set SUCCEEDED!\n");
	return 0;
}

int hci_api_loc_tx_slot_length(uint8_t payload_length)
{
	uint8_t request[] = {0x01, SET_OCF_OGF(OCF_OGF_HCI_LOC_TX_SLOT_LENGTH), 0x05, 0x00, 0x13, 0x22, 0x00, payload_length};
	int ret = 0;

	printf("hci_api_loc_tx_slot_length...\n");
	ret = hci_application_send_request(&request[0], sizeof(request));
	if(ret) {
          printf("hci_api_loc_tx_slot_length FAILED\n");
          return -1;
        }

	ret = wait_for_command_complete_ext_event(OCF_OGF_HCI_LOC_TX_SLOT_LENGTH);
	if(ret) {
          printf("wait_for_command_complete_event() FAILED...\n");
          return ret;
        }
	printf("hci_api_loc_tx_slot_length SUCCEEDED!\n");
	return 0;
}

int hci_api_loc_sender_receiver(void)
{
	uint8_t request[] = {0x01, SET_OCF_OGF(OCF_OGF_HCI_LOC_SENDER_RECEIVER), 0x05, 0x00, 0x13, 0x25, 0x00, 0x00};
	int ret = 0;

	printf("hci_api_loc_sender_receiver...\n");
	ret = hci_application_send_request(&request[0], sizeof(request));
	if(ret) {
          printf("hci_api_loc_sender_receiver FAILED\n");
          return -1;
        }

	ret = wait_for_command_complete_ext_event(OCF_OGF_HCI_LOC_SENDER_RECEIVER);
	if(ret) {
          printf("wait_for_command_complete_ext_event() FAILED...\n");
          return ret;
        }
	printf("hci_api_loc_sender_receiver SUCCEEDED!\n");
	return 0;
}

int hci_api_write_page_scan_activity(void)
{
	uint8_t request[] = {0x01, SET_OCF_OGF(OCF_GF_HCI_WRITE_PAGE_SCAN_ACTIVITY), 0x04, 0x00, 0x08, 0x00, 0x08};
	int ret = 0;

	printf("hci_api_write_page_scan_activity...\n");
	ret = hci_application_send_request(&request[0], sizeof(request));
	if(ret) {
          printf("hci_api_write_page_scan_activity FAILED...\n");
          return -1;
        }

	ret = wait_for_command_complete_event(OCF_GF_HCI_WRITE_PAGE_SCAN_ACTIVITY);
	if(ret) {
          printf("wait_for_command_complete_event() failed\n");
          return ret;
        }
	printf("hci_api_write_page_scan_activity SUCCEEDED!\n");
	return 0;
}

int hci_api_write_scan_enable(void)
{
	uint8_t request[] = {0x01, SET_OCF_OGF(OCF_GF_HCI_WRITE_SCAN_ENABLE), 0x01, 0x02};
	int ret = 0;

	printf("hci_api_write_scan_enable...\n");
	ret = hci_application_send_request(&request[0], sizeof(request));
	if(ret) {
          printf("hci_api_write_scan_enable FAILED...\n");
          return -1;
        }

	ret = wait_for_command_complete_event(OCF_GF_HCI_WRITE_SCAN_ENABLE);
	if(ret) {
          printf("wait_for_command_complete_event() failed\n");
          return ret;
        }
	printf("hci_application_send_request SUCCEEDED!\n");
	return 0;
}

int hci_api_set_event_filter_for_certif_test(void)
{
	uint8_t request[] = {0x01, SET_OCF_OGF(OCF_GF_HCI_SET_EVENT_FILTER), 0x03, 0x02, 0x00, 0x02};
	int ret = 0;

	printf("hci_api_set_event_filter_for_certif_test...\n");
	ret = hci_application_send_request(&request[0], sizeof(request));
	if(ret) {
          printf("hci_api_set_event_filter_for_certif_test FAILED...\n");
          return -1;
        }

	ret = wait_for_command_complete_event(OCF_GF_HCI_SET_EVENT_FILTER);
	if(ret) {
          printf("wait_for_command_complete_event() failed\n");
          return ret;
        }
	printf("hci_api_set_event_filter_for_certif_test SUCCEEDED!\n");
	return 0;
}

int hci_enable_device_under_test_mode(void)
{
	uint8_t request[] = {0x01, SET_OCF_OGF(OCF_GF_HCI_ENABLE_DEVICE_UNDER_TEST_MODE), 0x00};
	int ret = 0;

	printf("hci_enable_device_under_test_mode...\n");
	ret = hci_application_send_request(&request[0], sizeof(request));
	if(ret) {
          printf("hci_enable_device_under_test_mode FAILED\n");
          return -1;
        }

	ret = wait_for_command_complete_event(OCF_GF_HCI_ENABLE_DEVICE_UNDER_TEST_MODE);
	if(ret) {
          printf("wait_for_command_complete_event() failed\n");
          return ret;
        }
	printf("hci_enable_device_under_test_mode SUCCEEDED!\n");
	return 0;
}


int hci_api_hardware_reset_handler(void)
{
  int ret;

  ret = wait_for_command_complete_event(0x0000);
  if (ret) {
    printf("wait_for_command_complete_event() failed\n");
    return ret;
  }

  printf("\n");
  return 0;
}

int hci_api_write_bd_addr(uint8_t *bd_addr)
{
  uint8_t request[] = {0x01, SET_OCF_OGF(OCF_OGF_HCI_WRITE_BD_ADDRESS), 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  int ret;

  printf("hci_api_write_bd_addr...\n");
  memcpy(&request[4], bd_addr, 6);

  ret = hci_application_send_request(&request[0], sizeof(request));

  if(ret)
  {
    printf("hci_send_request() failed\n");
    return -1;
  }

  ret = wait_for_command_complete_event(OCF_OGF_HCI_WRITE_BD_ADDRESS);

  if(ret)
  {
    printf("wait_for_command_complete_event() failed\n");
    return ret;
  }

  return 0;
}

static const uint8_t TC35661_501_ROM[] = "8.00.72B-06 ROM=501";
static const int TC35661_501_ROM_LENGTH = sizeof(TC35661_501_ROM);
static const uint8_t TC35667_004_ROM[] = "C.00.85D-02 ROM=004";
static const int TC35667_004_ROM_LENGTH = sizeof(TC35667_004_ROM);
static const uint8_t TC35667_005_ROM_1[] = "C.00.85D-02 ROM=005";
static const int TC35667_005_ROM_LENGTH_1 = sizeof(TC35667_005_ROM_1);
static const uint8_t TC35667_005_ROM_2[] = "C.00.85E-19 ROM=005";
static const int TC35667_005_ROM_LENGTH_2 = sizeof(TC35667_005_ROM_2);

static const uint8_t UNKNOWN_VERSION[] = "UNKNOWN_VERSION";
static const int UNKNOWN_VERSION_LENGTH = sizeof(UNKNOWN_VERSION);

int hci_api_read_firmware_version(const uint8_t **version, int *version_len)
{
  uint8_t request[] = {0x01, 0x08, 0xfc, 0x09, 0x00, M2_GET_CMD, 0x00, 0x00, 0x00, 0x14, CMD_GET_FIRMWARE_VERSION, 0xff, 0x00};
  uint8_t *ptr = hci_buffer;
  struct hci_m2_xet_event event;
  int len;
  int ret;

  printf("hci_api_read_firmware_version...\n");
  ret = hci_application_send_request(&request[0], sizeof(request));

  if(ret)
  {
    printf("hci_send_request() failed\n");
    return -1;
  }

  len = hci_application_receive_response(ptr, hci_buffer_len);

  if(len < 0)
  {
    printf("hci_receive_response() failed\n");
    return -1;
  }

  ret = parse_hci_m2_xet_event(ptr, len, &event);

  if(ret)
  {
    printf("parse_hci_m2_xet_event() failed\n");
    return -1;
  }

  if(event.command != M2_GET_CMD) {
    printf("unexpected command 0x%02x\n", event.command);
    return -1;
  }

  if(event.information_id != CMD_GET_FIRMWARE_VERSION)
  {
    printf("unexpected information_id 0x%02x\n", event.information_id);
    return -1;
  }

  if(event.command_result != 0)
  {
    printf("unexpected command_result 0x%02x\n", event.command_result);
    return -1;
  }

  if(event.data_type != 0x0f)
  {
    printf("unexpected data_type 0x%02x\n", event.data_type);
    return -1;
  }

  if(!memcmp(&event.data[0], TC35661_501_ROM, TC35661_501_ROM_LENGTH))
  {
    *version = TC35661_501_ROM;
    *version_len = TC35661_501_ROM_LENGTH;
  }
  else if(!memcmp(&event.data[0], TC35667_004_ROM, TC35667_004_ROM_LENGTH))
  {
    *version = TC35667_004_ROM;
    *version_len = TC35667_004_ROM_LENGTH;
  }
  else if(!memcmp(&event.data[0], TC35667_005_ROM_1, TC35667_005_ROM_LENGTH_1))
  {
    *version = TC35667_005_ROM_1;
    *version_len = TC35667_005_ROM_LENGTH_1;
  }
  else if(!memcmp(&event.data[0], TC35667_005_ROM_2, TC35667_005_ROM_LENGTH_2))
  {
    *version = TC35667_005_ROM_2;
    *version_len = TC35667_005_ROM_LENGTH_2;
  }
  else
  {
    *version = UNKNOWN_VERSION;
    *version_len = UNKNOWN_VERSION_LENGTH;
  }

  printf("hci_api_read_firmware_version: %s.\n", *version);
  return 0;
}

int hci_api_set_deep_sleep(const struct hci_api_deep_sleep_data *data)
{
  uint8_t request[] = {
      0x01, 0x08, 0xfc, 0x1c, 0x00, M2_SET_CMD, 0x00, 0x00, 0x00, 0x14, CMD_SET_DEEP_SLEEP, 0xff, 0x10, 0x12, 0x00, 0xb8, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };
  int ret;

  request[14] = data->clkreq_signal;
  request[19] = data->notify_interface;
  request[20] = data->osciallator_drift & 0xff;
  request[21] = (data->osciallator_drift >> 8) & 0xff;
  request[22] = data->jitter_drift & 0xff;
  request[23] = (data->jitter_drift >> 8) & 0xff;
  request[24] = data->sniff_attempt_wakeup_margin & 0xff;
  request[25] = (data->sniff_attempt_wakeup_margin >> 8) & 0xff;

  ret = hci_application_send_request(&request[0], sizeof(request));
  if (ret) {
    printf("hci_send_request() failed\n");
    return -1;
  }

  ret = check_m2(CMD_SET_DEEP_SLEEP);
  if (ret) {
    printf("check_m2() failed\n");
    return -1;
  }

  printf("\n");
  return 0;
}

int hci_api_uart_rts_cts_control(int enable)
{
  uint8_t request[] = {0x01, 0x08, 0xfc, 0x03, 0x00, HCI_RTS_CTS, 0x00};
  uint8_t *ptr = hci_buffer;
  struct hci_command_event event;
  int len;
  int ret;

  /* make sure that enable byte is binary */
  request[6] = !!enable;

  ret = hci_application_send_request(&request[0], sizeof(request));
  if (ret) {
    printf("hci_send_request() failed\n");
    return -1;
  }

  len = hci_application_receive_response(ptr, hci_buffer_len);
  if (len < 0) {
    printf("hci_receive_response() failed\n");
    return -1;
  }

  ret = parse_hci_command_event(ptr, len, &event);
  if (ret) {
    printf("parse_hci_command_event() failed\n");
    return -1;
  }

  if (event.command != HCI_RTS_CTS) {
    printf("expected HCI_RTS_CTS, got 0x%0x2\n", event.command);
    return -1;
  }

  if (event.status != 0) {
    printf("status %d\n", event.status);
    return -1;
  }

  printf("\n");
  return 0;
}

int hci_api_switch_to_tcu_mode(const uint8_t *version, int version_len)
{
  uint8_t request[] = {0x01, 0x08, 0xfc, 0x03, 0x00, HCI_SET_MODE, 0x01};
  uint8_t *ptr = hci_buffer;
  struct hci_command_event event;
  int len;
  int ret;

  if(!memcmp(version, TC35661_501_ROM, TC35661_501_ROM_LENGTH))
  {
    ret = hci_api_tc35661_update_firmware();

    if(ret)
    {
      printf("hci_api_tc35661_update_firmware() failed\n");
      return -1;
    }
  }

  printf("hci_api_switch_to_tcu_mode...\n");
  sleep(2);

  ret = hci_application_send_request(&request[0], sizeof(request));

  if(ret)
  {
    printf("hci_send_request() failed\n");
    return -1;
  }

  len = hci_application_receive_response(ptr, hci_buffer_len);

  if(len < 0)
  {
    printf("hci_receive_response() failed\n");
    return -1;
  }

  ret = parse_hci_command_event(ptr, len, &event);
  if(ret)
  {
    printf("parse_hci_command_event() failed\n");
    return -1;
  }

  if(event.command != HCI_SET_MODE)
  {
    printf("expected HCI_SET_MODE, got 0x%0x2\n", event.command);
    return -1;
  }

  if(event.status != 0)
  {
    printf("status %d\n", event.status);
    return -1;
  }

  return 0;
}

static int check_m2(uint8_t cmd)
{
  uint8_t *ptr = hci_buffer;
  struct hci_m2_xet_event event;
  int len;
  int ret;

  len = hci_application_receive_response(ptr, hci_buffer_len);

  if(len < 0)
  {
    printf("hci_receive_response() failed\n");
    return -1;
  }

  ret = parse_hci_m2_xet_event(ptr, len, &event);

  if(ret)
  {
    printf("parse_hci_m2_xet_event() failed\n");
    return -1;
  }

  if(event.command != M2_SET_CMD)
  {
    printf("unexpected command 0x%02x\n", event.command);
    return -1;
  }

  if(event.information_id != cmd)
  {
    printf("unexpected information_id 0x%02x\n", event.information_id);
    return -1;
  }

  if(event.command_result != 0)
  {
    printf("unexpected command_result 0x%02x\n", event.command_result);
    return -1;
  }

  printf("\n");
  return 0;
}

int hci_api_set_i2_enable(void)
{
  uint8_t request[] = {0x01, 0x08, 0xfc, 0x0b, 0x00, M2_SET_CMD, 0x00, 0x00, 0x00, 0x14, CMD_I2C_ENABLE, 0xff, 0x02, 0x03, 0x01};
  int ret;

  printf("hci_api_set_i2_enable...\n");
  ret = hci_application_send_request(&request[0], sizeof(request));

  if(ret)
  {
    printf("hci_send_request() failed\n");
    return -1;
  }

  ret = check_m2(CMD_I2C_ENABLE);

  if(ret)
  {
    printf("check_m2() failed\n");
    return -1;
  }

  return 0;
}

int hci_api_eeprom_write_enable(void)
{
  uint8_t request[] = {0x01, 0x08, 0xfc, 0x09, 0x00, M2_SET_CMD, 0x00, 0x00, 0x00, 0x14, CMD_EEPROM_WRITE_ENABLE, 0xff, 0x00};
  int ret;

  ret = hci_application_send_request(&request[0], sizeof(request));
  if (ret) {
    printf("hci_send_request() failed\n");
    return -1;
  }

  ret = check_m2(CMD_EEPROM_WRITE_ENABLE);
  if (ret) {
    printf("check_m2() failed\n");
    return -1;
  }

  printf("\n");
  return 0;
}

int hci_api_write_eeprom(uint8_t eeprom_device_address, uint8_t addressing_16bit, uint16_t offset, uint8_t *buf, uint8_t write_length)
{
#define MAX_WRITE_LENGTH 32
  uint8_t request[20 + MAX_WRITE_LENGTH] = {0x01, 0x08, 0xfc, 0x10, 0x00, M2_SET_CMD, 0x00, 0x00, 0x00, 0x14, CMD_EEPROM_WRITE, 0xff, 0x10, 0x06, 0xa0, 0x01, 0x01, 0x06, 0x02, 0x00};
  uint8_t *ptr = hci_buffer;
  struct hci_m2_xet_event event;
  int len;
  int ret;

  if (write_length > MAX_WRITE_LENGTH) {
    printf("write_length %d out of bounds\n", write_length);
    return -1;
  }

  request[3] = 0x10 + write_length;
  request[13] = 0x06 + write_length;
  request[14] = eeprom_device_address;
  request[15] = !!addressing_16bit;
  request[17] = write_length;
  request[18] = offset & 0xff;
  request[19] = (offset >> 8) & 0xff;
  memcpy(&request[20], buf, write_length);

  ret = hci_application_send_request(&request[0], 20 + write_length);
  if (ret) {
    printf("hci_send_request() failed\n");
    return -1;
  }

  len = hci_application_receive_response(ptr, hci_buffer_len);
  if (len < 0) {
    printf("hci_receive_response() failed\n");
    return -1;
  }

  ret = parse_hci_m2_xet_event(ptr, len, &event);
  if (ret) {
    printf("parse_hci_m2_xet_event() failed\n");
    return -1;
  }

  if (event.command != M2_SET_CMD) {
    printf("unexpected command 0x%02x\n", event.command);
    return -1;
  }

  if (event.information_id != CMD_EEPROM_WRITE) {
    printf("unexpected information_id 0x%02x\n", event.information_id);
    return -1;
  }

  if (event.command_result != 0) {
    printf("unexpected command_result 0x%02x\n", event.command_result);
    return -1;
  }

  if (event.data_type != 0x00) {
    printf("unexpected data_type 0x%02x\n", event.data_type);
    return -1;
  }

  return 0;
}

int hci_api_read_eeprom(uint8_t eeprom_device_address, uint8_t addressing_16bit, uint16_t offset, uint8_t *buf, uint8_t read_length)
{
  uint8_t request[] = {0x01, 0x08, 0xfc, 0x10, 0x00, M2_GET_CMD, 0x00, 0x00, 0x00, 0x14, CMD_EEPROM_READ, 0xff, 0x10, 0x06, 0xa0, 0x01, 0x01, 0x06, 0x02, 0x00};
  uint8_t *ptr = hci_buffer;
  struct hci_m2_xet_event event;
  int len;
  int ret;

  printf("hci_api_read_eeprom...\n");
  request[14] = eeprom_device_address;
  request[15] = !!addressing_16bit;
  request[17] = read_length;
  request[18] = offset & 0xff;
  request[19] = (offset >> 8) & 0xff;
  ret = hci_application_send_request(&request[0], sizeof(request));

  if(ret)
  {
    printf("hci_send_request() failed\n");
    return -1;
  }

  len = hci_application_receive_response(ptr, hci_buffer_len);

  if(len < 0)
  {
    printf("hci_receive_response() failed\n");
    return -1;
  }

  ret = parse_hci_m2_xet_event(ptr, len, &event);

  if(ret)
  {
    printf("parse_hci_m2_xet_event() failed\n");
    return -1;
  }

  if(event.command != M2_GET_CMD)
  {
    printf("unexpected command 0x%02x\n", event.command);
    return -1;
  }

  if(event.information_id != CMD_EEPROM_READ)
  {
    printf("unexpected information_id 0x%02x\n", event.information_id);
    return -1;
  }

  if(event.command_result != 0)
  {
    printf("unexpected command_result 0x%02x\n", event.command_result);
    return -1;
  }

  if(event.data_type != 0x10)
  {
    printf("unexpected data_type 0x%02x\n", event.data_type);
    return -1;
  }

  if(event.data[0] != read_length)
  {
    printf("unexpected read length %d\n", event.data[0]);
    return -1;
  }

  memcpy(buf, &event.data[1], read_length);

  return 0;
}

int hci_api_execute_dloc_write(const struct hci_api_dbus_write_command *command)
{
  uint8_t request[] = {
      0x01, 0x03, 0xfc, 0x05, 0x00, HCI_LOC_DBUS_WRITE_CMD, 0x00, 0x00, 0x00,
  };
  uint8_t *ptr = hci_buffer;
  struct hci_command_event event;
  int len;
  int ret;

  request[6] = command->address;
  request[7] = command->lsb;
  request[8] = command->msb;

  ret = hci_application_send_request(&request[0], sizeof(request));
  if (ret) {
    printf("hci_send_request() failed\n");
    return -1;
  }

  len = hci_application_receive_response(ptr, hci_buffer_len);
  if (len < 0) {
    printf("hci_receive_response() failed\n");
    return -1;
  }

  if (ptr[1] != 0x0f) {
    printf("unexpected event_code 0x%02x\n", ptr[1]);
    return -1;
  }

  len = hci_application_receive_response(ptr, hci_buffer_len);
  if (len < 0) {
    printf("hci_receive_response() failed\n");
    return -1;
  }

  ret = parse_hci_command_event(ptr, len, &event);
  if (ret) {
    printf("parse_hci_m2_xet_event() failed\n");
    return -1;
  }

  if (event.command != HCI_LOC_DBUS_WRITE_CMD) {
    printf("expected HCI_SET_MODE, got 0x%0x2\n", event.command);
    return -1;
  }

  if (event.status != 0) {
    printf("status %d\n", event.status);
    return -1;
  }

  printf("address 0x%08x\n", command->address);

  return 0;
}

int hci_api_execute_m2_set(const uint8_t *request, int length)
{
  int ret;

  ret = hci_application_send_request(&request[0], length);
  if (ret) {
    printf("hci_send_request() failed\n");
    return -1;
  }

  ret = check_m2(request[10]);
  if (ret) {
    printf("check_m2() failed\n");
    return -1;
  }

  printf("\n");
  return 0;
}

#define CMD_BTL_SET_BAUDRATE 0x42

int hci_api_set_baudrate_tc35661(enum hci_api_baudrate baudrate)
{
  uint8_t request[] = {0x01, 0x08, 0xfc, 0x11, 0x00, M2_SET_CMD, 0x00, 0x00, 0x00, 0x14, CMD_BTL_SET_BAUDRATE, 0xff, 0x10, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t div1;
  uint8_t div2;
  int ret;

  switch (baudrate) {
  case HCI_API_BAUDRATE_115200:
    div1 = 0x001A;
    div2 = 0x60;
    break;
  case HCI_API_BAUDRATE_230400:
    div1 = 0x000D;
    div2 = 0x60;
    break;
  case HCI_API_BAUDRATE_460800:
    div1 = 0x0005;
    div2 = 0xA0;
    break;
  case HCI_API_BAUDRATE_921600:
    div1 = 0x0003;
    div2 = 0x70;
    break;
  default:
    return -1;
  }

  request[14] = (0xff & div1);
  request[15] = ((0xff00 & div1) >> 8);
  request[16] = div2;

  ret = hci_application_send_request(&request[0], sizeof(request));
  if (ret) {
    printf("hci_api_set_baudrate() failed\n");
    return -1;
  }

  ret = check_m2(CMD_BTL_SET_BAUDRATE);
  if (ret) {
    printf("check_m2() failed\n");
    return -1;
  }

  return 0;
}

#define DIVISION_RATIO_12 0x50
#define DIVISION_RATIO_13 0x60
#define DIVISION_RATIO_14 0x70
#define DIVISION_RATIO_15 0x80
#define DIVISION_RATIO_16 0x90
#define DIVISION_RATIO_17 0xa0

int hci_api_set_baudrate_tc35667(enum hci_api_baudrate baudrate)
{
  uint8_t request[] = {0x01, 0x08, 0xfc, 0x11, 0x00, M2_SET_CMD, 0x00, 0x00, 0x00, 0x14, CMD_BTL_SET_BAUDRATE, 0xff, 0x10, 0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
  uint16_t div1;
  uint8_t div2;
  int ret;

  switch (baudrate) {
  case HCI_API_BAUDRATE_115200:
    div1 = 15;
    div2 = DIVISION_RATIO_15;
    break;
  case HCI_API_BAUDRATE_230400:
    div1 = 7;
    div2 = DIVISION_RATIO_14;
    break;
  case HCI_API_BAUDRATE_460800:
    div1 = 4;
    div2 = DIVISION_RATIO_14;
    break;
  case HCI_API_BAUDRATE_921600:
    div1 = 2;
    div2 = DIVISION_RATIO_14;
    break;
  default:
    return -1;
  }

  request[14] = (0xff & div1);
  request[15] = ((0xff00 & div1) >> 8);
  request[16] = div2;

  ret = hci_application_send_request(&request[0], sizeof(request));
  if (ret) {
    printf("hci_api_set_baudrate() failed\n");
    return -1;
  }

  ret = check_m2(CMD_BTL_SET_BAUDRATE);
  if (ret) {
    printf("check_m2() failed\n");
    return -1;
  }

  return 0;
}

int hci_api_set_rtscts(void)
{
  uint8_t request[] = {0x01, 0x08, 0xfc, 0x0a, 0x00, M2_SET_CMD, 0x00, 0x00, 0x00, 0x14, CMD_BLE_SET_RTSCTS, 0xff, 0x01, 0xff};
  int ret;

 ret = hci_application_send_request(&request[0], sizeof(request));
  if (ret) {
    printf("hci_send_request() failed\n");
    return -1;
  }


  // LPS - Parece que o codigo esta errado porque não verifica ret = - 1
  // VERIFICAR !
  ret = check_m2(CMD_BLE_SET_RTSCTS);
  if (ret) {
    printf("check_m2() failed\n");
    return -1;
  }

  printf("\n");
  return 0;
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
