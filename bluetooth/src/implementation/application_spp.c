/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "xbee.h"
#include "application_spp.h"
#include "board.h"
#include "applog.h"
#include "timer.h"
#include "key_store_bt_api.h"
#include "nvm_api.h"
#include "timer_api.h"
#include "internal_apis.h"
#include "bt_api_spp.h"
#include "bt_api_mng.h"
#include "tcu_api.h"
#include "hci_api.h"
#include "adc_al.h"
#include "tare.h"
#include "weighting.h"
#include "power_manager.h"
#include "calibrator.h"

/* Defining this macro disables all debug messages. */
//#define printf(fmt, args...)    /* NOPRINTF */

#define  MAX_BUFFER_SIZE_SPP (543+16)

static uint8_t tcu_implementation_buffer[MAX_BUFFER_SIZE_SPP];
uint8_t *tcu_buffer = &tcu_implementation_buffer[0];
uint32_t tcu_buffer_len = sizeof(tcu_implementation_buffer);
uint8_t *hci_buffer = &tcu_implementation_buffer[0];
uint32_t hci_buffer_len = sizeof(tcu_implementation_buffer);

uint8_t device_name[33];
const int device_name_len = sizeof(device_name) - 1;

int volatile platform_alignment_tool_ready = 0;

struct command
{
  char *description;
  void (*function)(void);
};

#if 0
static void device_search(void);
static void device_select(void);
static void device_connect(void);
static void device_disconnect(void);
static void device_send_hello_world(void);
#endif

#if 0
static void device_speed_test(void);
static void key_store_delete(void);
#endif

#if 0
static struct command commands[] = {
    {
     "Device search",
     device_search,
    },
    {
     "Device select",
     device_select,
    },
    {
     "Device connect",
     device_connect,
    },
    {
     "Device disconnect",
     device_disconnect,
    },
    {
     "Device send 'Hello world'",
     device_send_hello_world,
    },
    {
     "Speed test",
     device_speed_test,
    },
    {
     "Key store delete",
     key_store_delete,
    },
};
#endif


#define NUM_COMMANDS (sizeof(commands) / sizeof(commands[0]))

enum state { state_idle,
             state_discovering_devices,
             discovering_services,
             state_connecting,
             state_device_selection,
             state_spp_connected,
             state_speed_test };
static enum state spp_device_state = state_idle;
static uint8_t spp_bd_addr[TCU_SIZE_BDADDR];

#define MAX_NUM_DEVICES 16
static int device_count;
static uint8_t searched_bd_addr[MAX_NUM_DEVICES][TCU_SIZE_BDADDR];

static int pin_code_requested;
static uint8_t pin_bd_addr[TCU_SIZE_BDADDR];

static int pass_key_requested;
static uint8_t pass_key_bd_addr[TCU_SIZE_BDADDR];

static int confirmation_requested;
static uint8_t confirmation_bd_addr[TCU_SIZE_BDADDR];

static int accept_connection;
static uint8_t accept_bd_addr[TCU_SIZE_BDADDR];
#define NUM_KEY_BT_ELEMENTS 16

struct nvm_partition spp_partitions[] = {
    {nvm_key_store_bt_index, sizeof(struct key_store_bt_index_element) * NUM_KEY_BT_ELEMENTS},
    {nvm_key_store_bt_data, sizeof(struct key_store_bt_element) * NUM_KEY_BT_ELEMENTS},
    {
     nvm_partition_end,
    }, /* end marker - don't remove */
};

extern struct nvm_partition other_partitions[];
extern struct nvm_partition system_partitions[];

 struct nvm_partition *const nvm_partitions[] = {
    &system_partitions[0],
    &spp_partitions[0],
    &other_partitions[0],
    NULL, /* end marker - don't remove */
};

//static int current_command = 0;

#if 0
static void device_search(void)
{
  enum bt_api_result bt_ret;

  if (spp_device_state != state_idle) {
    printf("cannot execute command, currently in state %d\n", spp_device_state);
    return;
  }

  device_count = 0;
  bt_ret = bt_api_mng_discover_remote_devices();
  if (bt_ret != BT_RESULT_SUCCESS) {
    printf("bt_api_mng_discover_remote_devices() failed\n");
    return;
  }
  spp_device_state = state_discovering_devices;

  printf("\n");
}
#endif

#if 0
static int current_selection = -1;
#endif

#if 0
static void key_store_delete(void)
{
  int ret;

  if (spp_device_state != state_idle) {
    printf("cannot execute command, currently in state %d\n", spp_device_state);
    return;
  }

  ret = key_store_bt_reset();
  if (ret) {
    printf("key_store_bt_reset() failed\n");
    return;
  }

  printf("\n");
}
#endif

#if 0
static void device_disconnect(void)
{
  int ret;

  if (spp_device_state != state_spp_connected) {
    printf("cannot execute command, currently in state %d\n", spp_device_state);
    return;
  }

  ret = bt_api_spp_disconnect();
  if (ret) {
    printf("bt_api_spp_disconnect() failed\n");
    return;
  }
}
#endif


#if 0
static void device_connect(void)
{
  int ret;

  if (spp_device_state != state_idle) {
    printf("cannot execute command, currently in state %d\n", spp_device_state);
    return;
  }

  if (current_selection == -1) {
    printf("no device selected\n");
    return;
  }

  ret = bt_api_mng_discover_remote_service(&searched_bd_addr[current_selection][0]);
  if (ret) {
    printf("bt_api_mng_discover_remote_service() failed\n");
    return;
  }

  spp_device_state = discovering_services;

  printf("\n");
}
#endif


#if 0
static void device_select(void)
{
  if (spp_device_state != state_idle) {
    printf("cannot execute command, currently in state %d\n", spp_device_state);
    return;
  }

  if (device_count == 0) {
    printf("device count is 0, search for devices first\n");
    return;
  }

  if (device_count == 1) {
    printf("only one device found, automatically selecting device 0\n");
    current_selection = 0;
    spp_device_state = state_idle;
    return;
  }

  spp_device_state = state_device_selection;
  printf("\n");
}
#endif

static uint8_t speed_test_buf[543];
static int speed_test_counter;
struct timer_data speed_test_timer;

#if 0
static void device_send_hello_world(void)
{
  uint8_t buf[] = {
      'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd',
  };
  int ret;

  if (spp_device_state != state_spp_connected) {
    printf("cannot execute command, currently in state %d\n", spp_device_state);
    return;
  }

  ret = bt_api_spp_data_transfer(buf, sizeof(buf));
  if (ret) {
    printf("bt_api_spp_data_transfer() failed\n");
    return;
  }
}
#endif

#if 0
static int speed_test_timer_callback(const void *argument)
{
  int ret =0;

  printf("speed_test_counter %d\n", speed_test_counter);
  speed_test_counter = 0;
#if 1
  ret = timer_start(&speed_test_timer, 10 * 1000);
#endif
  if (ret) {
    printf("timer_start() failed\n");
    return -1;
  }
  return 0;
}
#endif

#if 0
static void device_speed_test(void)
{
  int ret;
  int i;

  if (spp_device_state != state_spp_connected) {
    printf("cannot execute command, currently in state %d\n", spp_device_state);
    return;
  }

  speed_test_timer.function = speed_test_timer_callback;
  speed_test_timer.argument = (const void *)NULL;

  spp_device_state = state_speed_test;

  printf("starting speed test\n");

  for (i = 0; i < sizeof(speed_test_buf); i++) {
    speed_test_buf[i] = i;
  }

  ret = timer_start(&speed_test_timer, 10 * 1000);
  if (ret) {
    printf("timer_start() failed\n");
    return;
  }

  ret = bt_api_spp_data_transfer(speed_test_buf, sizeof(speed_test_buf));
  if (ret) {
    printf("bt_api_spp_data_transfer() failed\n");
    return;
  }
}
#endif


int bt_application_mng_pin_write_request_callback(uint8_t *bd_addr)
{
  /* return -1 if pin request should be rejected immediately */

  pin_code_requested = 1;
  memcpy(&pin_bd_addr[0], bd_addr, TCU_SIZE_BDADDR);

  printf("\n");

  return 0;
}

#if 0
static void confirm_pin_code(void)
{
  uint8_t pin[] = {'1', '2', '3', '4'};
  enum bt_api_result ret;

  ret = bt_api_mng_pin_write_confirm(pin_bd_addr, &pin[0], sizeof(pin));
  if (ret) {
    printf("bt_api_mng_pin_write_confirm() failed\n");
  }

  printf("\n");
}

static void reject_pin_code(void)
{
  enum bt_api_result ret;

  ret = bt_api_mng_pin_write_reject(pin_bd_addr);
  if (ret) {
    printf("bt_api_mng_pin_write_reject() failed\n");
  }

  printf("\n");
}

static int key_left(void)
{
  //board_led_off(3);

  if (pin_code_requested) {
    confirm_pin_code();
    pin_code_requested = 0;
    return 0;
  }
  if (accept_connection) {
    bt_api_connection_request_confirm(accept_bd_addr);
    accept_connection = 0;
    printf("accepting connection request\n");
    return 0;
  }
  if (pass_key_requested) {
    uint32_t pass_key = 0x1234;
    /* this limbo is necessary, so you can set a breakpoint on the next line
    and manipulate the pass_key via the live watch window. the value ends up
    in a register and cannot be manipulated if the next statement is the function
    call... */
    pass_key++;
    pass_key--;
    bt_api_mng_smp_passkey_input_request_confirm(pass_key_bd_addr, pass_key);
    pass_key_requested = 0;
    printf("sending pass key\n");
    return 0;
  }
  if (confirmation_requested) {
    bt_api_mng_smp_user_confirmation_accept(confirmation_bd_addr);
    confirmation_requested = 0;
    printf("confirmation accepted\n");
    return 0;
  }

  current_command--;
  if (current_command < 0)
    current_command = NUM_COMMANDS - 1;
  printf("%s [is_connected %d]\n", commands[current_command].description, spp_device_state == state_spp_connected ? 1 : 0);

  return 0;
}

static int key_right(void)
{
 // board_led_off(3);

  if (pin_code_requested) {
    reject_pin_code();
    pin_code_requested = 0;
    return 0;
  }
  if (accept_connection) {
    bt_api_connection_request_reject(accept_bd_addr);
    accept_connection = 0;
    printf("rejecting connection request\n");
    return 0;
  }
  if (pass_key_requested) {
    bt_api_mng_smp_passkey_input_request_reject(pass_key_bd_addr);
    pass_key_requested = 0;
    printf("rejecting pass key request\n");
    return 0;
  }
  if (confirmation_requested) {
    bt_api_mng_smp_user_confirmation_reject(confirmation_bd_addr);
    confirmation_requested = 0;
    printf("confirmation rejected\n");
    return 0;
  }

  current_command++;
  if (current_command >= NUM_COMMANDS)
    current_command = 0;
  printf("%s [is_connected %d]\n", commands[current_command].description, spp_device_state == state_spp_connected ? 1 : 0);

  return 0;
}

static int key_up(void)
{
  if (spp_device_state != state_device_selection)
    return 0;

  current_selection++;
  if (current_selection >= device_count)
    current_selection--;

  printf("device %d\n", current_selection);

  return 0;
}

static int key_down(void)
{
  if (spp_device_state != state_device_selection)
    return 0;

  current_selection--;
  if (current_selection < 0)
    current_selection = device_count - 1;

  printf("device %d\n", current_selection);

  return 0;
}


static int key_select(void)
{
  if (spp_device_state == state_device_selection) {
    printf("selected device %d\n", current_selection);
    spp_device_state = state_idle;
    return 0;
  }

  printf("Executing %s\n", commands[current_command].description);
  commands[current_command].function();
  return 0;
}
#endif
int bt_application_mng_smp_passkey_input_request_callback(uint8_t *bd_addr)
{
  /* return -1 if pass key request should be rejected immediately */
  pass_key_requested = 1;
  memcpy(&pass_key_bd_addr[0], bd_addr, TCU_SIZE_BDADDR);
  printf("press left to enter pass key and right to reject pass key request from bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", PRINT_BD_ADDR(bd_addr));
  //board_led_on(3);
  return 0;
}

int bt_application_mng_smp_confirmation_request_callback(uint8_t *bd_addr, uint32_t pass_key)
{
  /* return -1 if confirmation request should be rejected immediately */
  confirmation_requested = 1;
  memcpy(&confirmation_bd_addr[0], bd_addr, TCU_SIZE_BDADDR);
  printf("press left to confirm and right to reject confirmation request from bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", PRINT_BD_ADDR(bd_addr));
 // board_led_on(3);
  return 0;
}

void bt_application_mng_smp_show_pass_key_callback(uint8_t *bd_addr, uint32_t pass_key)
{
  printf("showing pass key %d for bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", pass_key, PRINT_BD_ADDR(bd_addr));
}

void bt_application_mng_smp_hide_pass_key_callback(void)
{
  printf("hiding pass key\n");
}

void bt_application_mng_smp_pairing_success_callback(void)
{
  printf("pairing succeeded\n");
}

extern void bt_application_mng_smp_pairing_failed_callback()
{
  printf("pairing failed\n");
}

void bt_application_mng_capability_callback(enum tcu_mng_smp_io_capability *io_capability,
                                            enum tcu_mng_smp_authentication_requirement *authentication_requirement)
{
  //  *io_capability = TCU_MNG_DISPLAY_ONLY;
  //  *io_capability = TCU_MNG_DISPLAY_YES_NO;
  //  *io_capability = TCU_MNG_KEYBOARD_ONLY;
  *io_capability = TCU_MNG_NO_INPUT_NO_OUTPUT;

  //  *authentication_requirement = TCU_MNG_SMP_MITM_PROTECTION_REQUIRED_DEDICATED_BONDINGIO_CAPABILITIES;
  //  *authentication_requirement = TCU_MNG_SMP_MITM_PROTECTION_NOT_REQUIREDGENERAL_BONDING;
  *authentication_requirement = TCU_MNG_SMP_MITM_PROTECTION_REQUIREDGENERAL_BONDING_IO_CAPABILITIES;

  printf("\n");
}

#define PRINT_BD_ADDR(x) \
  x[0], x[1], x[2], x[3], x[4], x[5]

int bt_application_mng_connection_request_callback(uint8_t *bd_addr)
{
  /* return -1 if connection request should be rejected immediately */

  accept_connection = 1;
  memcpy(&accept_bd_addr[0], bd_addr, TCU_SIZE_BDADDR);
  printf("press  left to confirm and right to reject connection request from bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", PRINT_BD_ADDR(bd_addr));
  //board_led_on(3);
  return 0;
}

void bt_application_mng_discover_remote_devices_done_callback(void)
{
  spp_device_state = state_idle;
  printf("device discovery done\n");
}

void bt_application_mng_discover_remote_service_done_callback(uint8_t *bd_addr, uint8_t number_of_spp_channels, uint8_t *spp_server_channel_numbers)
{
  enum bt_api_result bt_ret;

  printf("bd_addr %02x:%02x:%02x:%02x:%02x:%02x, service_count %d\n", PRINT_BD_ADDR(bd_addr), number_of_spp_channels);

  if (!number_of_spp_channels) {
    printf("cannot connect, no TCU_MNG_SERVICE_SPP channels found\n");
    spp_device_state = state_idle;
    return;
  }

  if (number_of_spp_channels > 1) {
    printf("devices support more than 1 TCU_MNG_SERVICE_SPP connection, using connection 0\n");
  }

  bt_ret = bt_api_spp_connect(bd_addr, spp_server_channel_numbers[0]);
  if (bt_ret != BT_RESULT_SUCCESS) {
    printf("bt_api_spp_connect() failed\n");
    spp_device_state = state_idle;
    return;
  }
}

void bt_application_mng_discover_remote_devices_callback(struct tcu_mng_discover_remote_dev_result_event *event)
{
  printf("found bd_addr %02x:%02x:%02x:%02x:%02x:%02x, name '%s' -> device_count %d\n", event->bd_addr[0], event->bd_addr[1], event->bd_addr[2], event->bd_addr[3], event->bd_addr[4], event->bd_addr[5], event->name, device_count);

  if (device_count == MAX_NUM_DEVICES) {
    printf("increase MAX_NUM_DEVICES\n");
    return;
  }


  memcpy(&searched_bd_addr[device_count][0], event->bd_addr, TCU_SIZE_BDADDR);
  device_count++;
}

void bt_application_mng_device_name_notification_callback(uint8_t *bd_addr, uint8_t *device_name, uint8_t length_of_device_name)
{
  printf("bd_addr %02x:%02x:%02x:%02x:%02x:%02x -> '%s'\n", bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5], device_name);
}

void bt_application_spp_data_sent_callback(void)
{
  int ret;

  if (spp_device_state == state_speed_test) {

    speed_test_counter += sizeof(speed_test_buf);

    ret = bt_api_spp_data_transfer(speed_test_buf, sizeof(speed_test_buf));
    if (ret) {
      printf("bt_api_spp_data_transfer() failed\n");
      return;
    }
    return;
  }

  printf("\n");
}

void bt_application_spp_connect_callback(uint8_t *bd_addr)
{
  memcpy(&spp_bd_addr, bd_addr, TCU_SIZE_BDADDR);
  spp_device_state = state_spp_connected;
  printf("bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
}

void bt_application_spp_disconnect_callback(void)
{
	spp_device_state = state_idle;
	printf("\n");
}

void bt_application_spp_command_received(uint8_t *buf, int len)
{
        INFO_PRINTF(0x10 | 0x0B,"received len %d\n", len);
        INFO_PRINTF(0x10 | 0x0B,"Data Recebida: %s\n", buf);

        uint8_t *SID;
        uint8_t *LID;
        long sid,lid;
        char bluetooth[100] = {'\0'};

        sid = strtol((const char *)buf, (char **)&SID, 16);
        SID = &SID[1];
        lid = strtol((const char *)SID, (char **)&LID, 16);

        switch(sid){
                case BT_SID_POWER_SUPPLY_INFORMATION:
                        switch(lid){
                                float value;
                                case BT_LID_READ_PANEL_STATUS:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth READ_PANEL_STATUS\n");
                                        get_supply_voltage(&value, PM_SRC_PANEL);
                                        sprintf(bluetooth,"%f", value);
                                        bt_api_spp_data_transfer((uint8_t *)bluetooth, strlen(bluetooth));
                                        break;
                                case BT_LID_READ_BATTERY_CHARGING_STATUS:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth READ_BATTERY_CHARGING_STATUS\n");
                                        get_supply_voltage(&value, PM_SRC_BATTERY);
                                        sprintf(bluetooth,"%f", value);
                                        bt_api_spp_data_transfer((uint8_t *)bluetooth, strlen(bluetooth));
                                        break;
                                }
                        break;
                case BT_SID_PLATFORM_ALIGNMENT_TOOL:
                        if(lid == BT_LID_PLATFORM_ALIGNMENT_TOOL)
                        {
                                INFO_PRINTF(0x10 | 0x0B,"Bluetooth PLATFORM_ALIGNMENT_TOOL\n");
                                int tare_weight[MAX_PLATFORM_CHANNEL];
                                get_platform_tare_weight(tare_weight);
                                sprintf(bluetooth,"Instant Tare: %d : %d : %d : %d",
                                        tare_weight[0],
                                        tare_weight[1],
                                        tare_weight[2],
                                        tare_weight[3]);
                                bt_api_spp_data_transfer((uint8_t *)bluetooth, strlen(bluetooth));
                        }
                        break;
                case BT_SID_GET_CURRENT_TARE:
                        if(lid == BT_LID_GET_CURRENT_TARE)
                        {
                                INFO_PRINTF(0x10 | 0x0B,"Bluetooth GET_CURRENT_TARE\n");
                        }
                        break;
                case BT_SID_CATTLE_COUNTER:
                        switch(lid){
                                case BT_LID_GET_CATTLE_COUNTER:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth GET_CATTLE_COUNTER\n");
                                        break;
                                case BT_LID_RESET_ADC_CATTLE_COUNTER:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth RESET_ADC_CATTLE_COUNTER\n");
                                        break;
                                }
                        break;
                case BT_SID_GET_LAST_WEIGHT_AND_TAG:
                        if(lid == BT_LID_GET_LAST_WEIGHT_AND_TAG)
                        {
                                INFO_PRINTF(0x10 | 0x0B,"Bluetooth GET_LAST_WEIGHT_AND_TAG\n");
                        }
                        break;
                case BT_SID_FIELD_BOX_REBOOT_SHUTDOWN:
                        switch(lid){
                                case BT_LID_REBOOT_SYSTEM:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth REBOOT_SYSTEM\n");
                                        break;
                                case BT_LID_RESET_TARE_FILE:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth RESET_TARE_FILE\n");
                                        break;
                                case BT_LID_RESET_CONFIGURATION_FILE:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth RESET_CONFIGURATION_FILE\n");
                                        break;
                                case BT_LID_FACTORY_RESET:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth FACTORY_RESET\n");
                                        break;
                                case BT_LID_RUN_FILESYSTEM_CORRUPTION_CHECK:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth RUN_FILESYSTEM_CORRUPTION_CHECK\n");
                                        break;
                                case BT_LID_SHUTDOWN_SYSTEM:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth SHUTDOWN_SYSTEM\n");
                                        break;
                                case BT_LID_FORMAT_SD_CARD:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth FORMAT_SD_CARD\n");
                                        break;
                        }
                        break;
                case BT_SID_GET_ERROR_INFORMATION:
                        if(lid == BT_LID_GET_ERROR_INFORMATION)
                        {
                                INFO_PRINTF(0x10 | 0x0B,"Bluetooth BT_LID_GET_ERROR_INFORMATION\n");
                        }
                        break;
                case BT_SID_SYSTEM_CONFIGURATION:
                        switch(lid){
                                case BT_LID_GET_FIELD_BOX_CONFIGURATION:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth GET_FIELD_BOX_CONFIGURATION\n");
                                        break;
                                case BT_LID_SET_FIELD_BOX_CONFIGURATION:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth SET_FIELD_BOX_CONFIGURATION\n");
                                        break;
                                case BT_LID_GET_RFID_READER_CONFIGURATION:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth GET_RFID_READER_CONFIGURATION\n");
                                        break;
                                case BT_LID_SET_RFID_READER_CONFIGURATION:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth SET_RFID_READER_CONFIGURATION\n");
                                        break;
                        }
                        break;
                case BT_SID_GET_LOG_INFORMATION:
                        if(lid == BT_LID_GET_LOG_INFORMATION)
                        {
                                INFO_PRINTF(0x10 | 0x0B,"Bluetooth GET_LOG_INFORMATION\n");
                        }
                        break;
                case BT_SID_CONFIGURE_PLATFORM_SLOPE:
                        switch(lid){
                                double slope;
                                static Calibrator* calibrator = NULL;
                                uint8_t step;

                                case BT_LID_ENTER_CALIBRATION_MODE:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth ENTER_CALIBRATION_MODE\n");
                                        if (calibrator == NULL)
                                        {
                                                calibrator = get_instance(BLUETOOTH_CALIBRATION);
                                        }
                                        step = calibrator->calibrate((char*)&LID[1]);

                                        if (step == 0) //Error
                                        {
                                                sprintf(bluetooth, "%s", "ACK_0");
                                        }
                                        else if (step == 1)
                                        {
                                                sprintf(bluetooth, "%s", "ACK_1");
                                        }
                                        else if (step == 2)
                                        {
                                                sprintf(bluetooth, "%s|%f", "ACK_2", get_weight_slope());
                                        }
                                        bt_api_spp_data_transfer((uint8_t *)bluetooth, strlen(bluetooth));
                                        break;
                                case BT_LID_GET_SLOPE:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth GET_SLOPE\n");
                                        slope = get_weight_slope();

                                        sprintf(bluetooth,"%f", slope);
                                        bt_api_spp_data_transfer((uint8_t *)bluetooth, strlen(bluetooth));
                                        break;
                                case BT_LID_SET_SLOPE:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth SET_SLOPE\n");
                                        errno = 0;
                                        sscanf((char *)&LID[1], "%lf", &slope);
                                        if (errno == EILSEQ)
                                        {
                                                INFO_PRINTF(0x10 | 0x0B, "INVALID SLOPE\n");
                                                break;
                                        }
                                        set_weight_slope(slope);
                                        break;
                        }
                        break;
                case BT_SID_CONFIGURE_PEN_NUMBER:
                        switch(lid){
                                case BT_LID_SET_PEN_NUMBER:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth SET_PEN_NUMBER\n");
                                        break;
                                case BT_LID_READ_PEN_NUMBER:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth READ_PEN_NUMBER\n");
                                        break;
                        }
                        break;
                case BT_SID_XBEE_NETWORK_SETTINGS:
                        switch(lid){
                                case BT_LID_READ_MAC_ADDRESS:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth READ_MAC_ADDRESS\n");
                                        break;
                                case BT_LID_START_NETWORK_DEPLOYMENT:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth START_NETWORK_DEPLOYMENT\n");
                                        break;
                                case BT_LID_GET_STATUS:
                                        INFO_PRINTF(0x10 | 0x0B,"Bluetooth GET_STATUS\n");
                                        break;
                        }
                        break;
                        default:
                                INFO_PRINTF(0x10 | 0x0B,"ERRO\n");
        }

}

void bt_application_spp_data_received_callback(uint8_t *buf, int len)
{
  /* likely speed test */
  if (len == 543) {
    return;
  }
  *(buf+len) = 0;
  bt_application_spp_command_received(buf, len);


#if 0
  /* rkdev: Reply the instant tare to the mobile. */
  {
    int tare_weight[MAX_PLATFORM_CHANNEL];
    char tare_buf[100] = {0};

    get_platform_tare_weight(tare_weight);
    sprintf(tare_buf,
                "Instant Tare: %d : %d : %d : %d.",
                        tare_weight[0],
                        tare_weight[1],
                        tare_weight[2],
                        tare_weight[3]);
    bt_api_spp_data_transfer((uint8_t *)tare_buf, strlen(tare_buf));
    printf("Instant Tare sent: %d : %d : %d : %d.\n",
                        tare_weight[0],
                        tare_weight[1],
                        tare_weight[2],
                        tare_weight[3]);
  }
#endif
}



int application_spp_tcu_event_handler(uint8_t *data)
{
  enum bt_api_result result;


  result = bt_api_event_handler(data);
  if (result)
    return -1;

  return 0;
}
#if 0
int application_spp_event_handler(uint32_t key)
{
  /* check if Chiron in SLEEP Mode: status of MCU PC6 pin */
  GPIO_SetInputEnableReg(GPIO_PC, GPIO_BIT_6, ENABLE);
  if (GPIO_ReadDataBit(GPIO_PC, GPIO_BIT_6) == GPIO_BIT_VALUE_1)
    board_led_on(0);
  else
    board_led_off(0);

  switch (key) {
  case KEY_UP_PRESS:
    key_up();
    break;
  case KEY_DOWN_PRESS:
    key_down();
    break;
  case KEY_LEFT_PRESS:
    key_left();
    break;
  case KEY_RIGHT_PRESS:
    key_right();
    break;
  case KEY_SELECT_PRESS:
    key_select();
    break;
  case KEY_UP_RELEASE:
  case KEY_DOWN_RELEASE:
  case KEY_LEFT_RELEASE:
  case KEY_RIGHT_RELEASE:
  case KEY_SELECT_RELEASE:
    break;
  default:
    printf("unknown key %d\n", key);
    break;
  }

  return 0;
}
#endif


int application_spp_setup(void)
{
  enum bt_api_result bt_ret;
  int ret;

  printf("application_spp_setup...\n");

  /* Init MNG API */
  bt_ret = bt_api_mng_init(device_name, device_name_len);

  if(bt_ret != BT_RESULT_SUCCESS)
  {
    printf("bt_api_mng_init() failed\n");
    return -1;
  }

  printf("bt_api_mng_init success\n ");

  /* Init SPP API */
  bt_ret = bt_api_spp_init();

  if(bt_ret != BT_RESULT_SUCCESS)
  {
    printf("bt_api_spp_init() failed\n");
    return -1;
  }

  printf("bt_api_spp_init success\n ");

  /* Init Key storage */
  ret = key_store_bt_init();

  if(ret)
  {
    printf("key_store_bt_init() failed\n");
    return -1;
  }

  printf("key_store_bt_init success\n ");

 /* Set Scan Parameters */
  enum bt_api_mng_scan_visibility visibility = mng_scan_visible;
  enum bt_api_mng_scan_discoverability discoverability = mng_scan_discoverable;
  ret = bt_api_mng_set_scan(visibility, discoverability);

  if(ret)
  {
    printf("bt_api_mng_set_scan() failed\n");
    return -1;
  }

  return 0;
}

void set_bt_device_name()
{
    get_xbee_self_addr_str((char*)device_name);
}

int application_setup(void)
{
  const uint8_t *version;
  int version_len;
  int ret;

  ret = board_init_bluetooth(&version, &version_len);

  if(ret)
  {
    printf("board_init_bluetooth() failed\n");
    return -1;
  }

  return application_spp_setup();
}

#if 0
__weak int application_event_handler(uint8_t arg)
{
  return application_spp_event_handler(arg);
}

__weak int application_tcu_event_handler(uint8_t *data)
{
  return application_spp_tcu_event_handler(data);
}
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
