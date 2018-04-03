/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/
#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hci_api_internal.h"
//#include "debug_api.h"

/* Defining this macro disables all debug messages. */
#define printf(fmt, args...)	/* NOPRINTF */

static const struct hci_api_dbus_write_command hci_loc_dbus_write_rf_patch[] = {
    {0xAE, 0x18, 0xCD},
	{0xEE, 0x00, 0xD9},
	{0xFA, 0xA4, 0xA4},
	{0x81, 0xF3, 0xC7},
	{0x63, 0xF4, 0x04},
	{0x83, 0x00, 0xE6},
	{0x64, 0xAF, 0x00},
	{0x84, 0xFF, 0x80},
	{0x65, 0xB0, 0x00},
	{0x85, 0x1B, 0x60},
	{0x66, 0xED, 0x00},
	{0x86, 0x6E, 0x00},
	{0x67, 0xEA, 0x0E},
	{0x87, 0x80, 0x80},
	{0x68, 0xF4, 0x49},
	{0x88, 0x00, 0xEE},
	{0x69, 0xF4, 0x27},
	{0x89, 0x00, 0xFE},
	{0x6A, 0x00, 0xFF},
	{0x8A, 0x00, 0x00},
	{0x6B, 0xEA, 0x09},
	{0x8B, 0x80, 0x00},
	{0x6C, 0xF4, 0x01},
	{0x8C, 0x00, 0x80},
	{0x6D, 0xE2, 0x01},
	{0x8D, 0x08, 0x00},
	{0x6E, 0xA4, 0x01},
	{0x8E, 0x80, 0x00},
	{0x6F, 0x00, 0xFF},
	{0x8F, 0x00, 0x00},
	{0xA0, 0x21, 0x10},
	{0x81, 0xD3, 0xC7},
	{0x82, 0xA8, 0x00},
	{0x63, 0xF4, 0x04},
	{0x83, 0x00, 0xE2},
	{0x64, 0xAF, 0x00},
	{0x84, 0xF8, 0x80},
	{0x65, 0xB0, 0x00},
	{0x85, 0x18, 0x60},
	{0x66, 0xED, 0x00},
	{0x86, 0x6E, 0x70},
	{0x67, 0xEA, 0x0E},
	{0x87, 0x80, 0x80},
	{0x68, 0xF4, 0x14},
	{0x88, 0x00, 0xE6},
	{0x69, 0xF4, 0x04},
	{0x89, 0x00, 0xFE},
	{0x6A, 0x00, 0xFF},
	{0x8A, 0x00, 0x00},
	{0x6B, 0xEA, 0x02},
	{0x8B, 0x80, 0x00},
	{0x6C, 0xF4, 0x01},
	{0x8C, 0x00, 0x80},
	{0x6D, 0xE2, 0x01},
	{0x8D, 0x08, 0x00},
	{0x6E, 0xA4, 0x01},
	{0x8E, 0x80, 0x00},
	{0x6F, 0x00, 0xFF},
	{0x8F, 0x00, 0x00},
	{0xA0, 0x21, 0x00},
};
#define HCI_LOC_DBUS_WRITE_RF_PATCH_NUM_ENTRIES (int)(sizeof(hci_loc_dbus_write_rf_patch) / sizeof(hci_loc_dbus_write_rf_patch[0]))

static const uint8_t hci_m2_set_rf_patch_0[] = {
    0x01, 0x08, 0xfc, 0x2f, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x14, 0x55, 0xff, 0x10, 0x25, 0x03, 0x9c, 0xad, 0x09, 0x00, 0xe0, 0x7b, 0x00, 0x28, 0x01, 0xd0, 0x02, 0x21, 0x38, 0xe0, 0xbb, 0xf7, 0xab, 0xfe, 0x4a, 0xe1, 0x01, 0xd0, 0xe5, 0xf7, 0x7e, 0xff, 0x01, 0x2e, 0x00, 0xd0, 0x00, 0x27, 0x20, 0x7c, 0x04, 0x28,
};
static const uint8_t hci_m2_set_rf_patch_1[] = {
    0x01, 0x08, 0xfc, 0xee, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x14, 0x56, 0xff, 0x10, 0xe4, 0x00, 0x6B, 0x05, 0x00, 0x26, 0x4a, 0x05, 0x20, 0x30, 0xb4, 0x10, 0x80, 0x26, 0x48, 0x25, 0x49, 0x88, 0x82, 0xff, 0x20, 0xc8, 0x82, 0xc8, 0x8a, 0xc0, 0x07, 0xfc, 0xd1, 0x01, 0x20, 0x80, 0x03, 0x88, 0x82, 0x48, 0x8b, 0x00, 0x09, 0x00, 0x01, 0x08, 0x21, 0x01, 0x43, 0x51, 0x83, 0xff, 0x21, 0xff, 0x31, 0x91, 0x83, 0x1d, 0x4c, 0x00, 0x22, 0xa2, 0x80, 0xff, 0x23, 0xe4, 0x33, 0xe3, 0x80, 0x0c, 0x24, 0x04, 0x43, 0x1a, 0x48, 0x84, 0x81, 0xc1, 0x81, 0x82, 0x82, 0xc3, 0x82, 0x19, 0x49, 0x08, 0x88, 0x01, 0x23, 0x02, 0x1c, 0x9a, 0x43, 0x0a, 0x80, 0x17, 0x4a, 0x03, 0x23, 0x5b, 0x04, 0x5a, 0x62, 0x60, 0x32, 0xda, 0x62, 0x16, 0x4a, 0x14, 0x4d, 0x5a, 0x66, 0x15, 0x4a, 0x17, 0x4c, 0x9a, 0x66, 0x15, 0x4a, 0x5a, 0x67, 0xda, 0x1d, 0xf9, 0x32, 0x9d, 0x62, 0x54, 0x62, 0x08, 0x34, 0xd4, 0x62, 0x95, 0x62, 0x13, 0x4a, 0x12, 0x4c, 0x54, 0x62, 0x13, 0x4c, 0x94, 0x62, 0x13, 0x4c, 0xd4, 0x62, 0x1b, 0x68, 0x12, 0x4a, 0x93, 0x42, 0xfd, 0xd1, 0x08, 0x80, 0x30, 0xbc, 0x70, 0x47, 0x20, 0x02, 0x06, 0x00, 0xc0, 0x08, 0x06, 0x00, 0xbe, 0x2f, 0x00, 0x00, 0x60, 0x03, 0x06, 0x00, 0x60, 0x0b, 0x06, 0x00, 0xa0, 0x02, 0x06, 0x00, 0x0b, 0x20, 0x00, 0x20, 0x00, 0x20, 0x18, 0x20, 0x07, 0xa0, 0x01, 0x21, 0x00, 0xa0, 0x1d, 0x21, 0x0c, 0xa0, 0x01, 0x21, 0x0f, 0x20, 0x00, 0x20, 0x01, 0x20, 0x01, 0x20, 0x40, 0x01, 0x06, 0x00, 0x00, 0x20, 0x1d, 0x20, 0x03, 0x20, 0x01, 0x20, 0xff, 0x0f, 0x00, 0x00,
};
static const uint8_t hci_m2_set_rf_patch_2[] = {0x01, 0x08, 0xfc, 0x0b, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x14, 0x57, 0xff, 0x02, 0x03, 0x01};
static const uint8_t hci_m2_set_rf_patch_3[] = {0x01, 0x08, 0xfc, 0x09, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x14, 0x01, 0xff, 0x00};

static const uint8_t hci_m2_set_uuid_char_desc_patch_0[] = {0x01, 0x08, 0xfc, 0x2f, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x14, 0x55, 0xff, 0x10, 0x25, 0x00, 0x38, 0xf8, 0x0c, 0x00, 0x05, 0x32, 0x87, 0xf7, 0xe1, 0xfc, 0x00, 0x28, 0x79, 0xd0, 0x13, 0x28, 0x1a, 0xd0, 0x08, 0x4c, 0x2f, 0x25, 0x61, 0x68, 0x6d, 0x01, 0x49, 0x19, 0x89, 0x7d, 0x00, 0x29, 0x6f, 0xd1, 0x02, 0x1c};
static const uint8_t hci_m2_set_uuid_char_desc_patch_1[] = {0x01, 0x08, 0xfc, 0xfa, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x14, 0x56, 0xff, 0x10, 0xf0, 0x00, 0x72, 0x05, 0x00, 0xff, 0xb5, 0x91, 0xb0, 0x00, 0x20, 0x01, 0x23, 0x0f, 0x90, 0x11, 0x1c, 0x05, 0x93, 0x1c, 0x9a, 0x00, 0x23, 0x0d, 0x90, 0x03, 0x93, 0x00, 0x93, 0x07, 0x90, 0x06, 0x90, 0x10, 0x70, 0x99, 0x4a, 0x2f, 0x23, 0x12, 0x68, 0x5b, 0x01, 0xd2, 0x18, 0x90, 0x75, 0x11, 0xa8, 0x05, 0xc8, 0x00, 0x27, 0x00, 0x26, 0x00, 0x25, 0x90, 0x42, 0x02, 0xd8, 0x11, 0x98, 0x00, 0x28, 0x07, 0xd1, 0x11, 0x98, 0x1b, 0x9a, 0x10, 0x80, 0x01, 0x20, 0x15, 0xb0, 0xf0, 0xbc, 0x08, 0xbc, 0x18, 0x47, 0x14, 0x98, 0x02, 0x28, 0x05, 0xd1, 0x48, 0x78, 0x09, 0x78, 0x00, 0x02, 0x08, 0x43, 0x06, 0x1c, 0x06, 0xe0, 0x14, 0x98, 0x10, 0x28, 0x0d, 0xd1, 0x10, 0x22, 0x08, 0xa8, 0xf1, 0xf7, 0xe1, 0xff, 0x0f, 0x21, 0x07, 0x20, 0x0e, 0x90, 0x10, 0x91, 0x77, 0xf0, 0x88, 0xff, 0xff, 0x28, 0x03, 0xd1, 0x0e, 0x20, 0xe1, 0xe7, 0x04, 0x20, 0xdf, 0xe7, 0x6e, 0xf0, 0xcc, 0xfa, 0x02, 0x90, 0x0f, 0x99, 0xca, 0xe0, 0x0d, 0x98, 0x44, 0x23, 0x7c, 0x49, 0x58, 0x43, 0x40, 0x18, 0x0c, 0x99, 0xc9, 0x00, 0x44, 0x58, 0x40, 0x18, 0x04, 0x90, 0xb8, 0xe0, 0x14, 0x98, 0xa2, 0x7c, 0x10, 0x28, 0x08, 0xd1, 0x10, 0x2a, 0x11, 0xd1, 0xa1, 0x1c, 0x08, 0xa8, 0xf1, 0xf7, 0x8f, 0xff, 0x00, 0x06, 0x00, 0x0e, 0x09, 0xe0, 0x14, 0x98, 0x02, 0x28, 0x07, 0xd1, 0x02, 0x2a, 0x05, 0xd1, 0x61, 0x88, 0x01, 0x20, 0xb1, 0x42, 0x00, 0xd1, 0x00, 0x20, 0x05, 0x90, 0x05, 0x98, 0x00, 0x28, 0x71, 0xd1, 0x20, 0x88, 0x11, 0x99, 0x88, 0x42, 0x6d, 0xd3, 0x12, 0x99, 0x88, 0x42, 0x6a, 0xd8, 0x06, 0x90};
static const uint8_t hci_m2_set_uuid_char_desc_patch_2[] = {0x01, 0x08, 0xfc, 0xfa, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x14, 0x56, 0xff, 0x10, 0xf0, 0xec, 0x72, 0x05, 0x00, 0xe0, 0x8a, 0x07, 0x99, 0x02, 0x30, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x29, 0x05, 0xd1, 0x62, 0x49, 0x2f, 0x22, 0x09, 0x68, 0x52, 0x01, 0x89, 0x18, 0x08, 0x76, 0x61, 0x48, 0x86, 0x42, 0x08, 0xd0, 0x01, 0x30, 0x86, 0x42, 0x05, 0xd0, 0x02, 0x38, 0x86, 0x42, 0x02, 0xd0, 0x01, 0x38, 0x86, 0x42, 0x22, 0xd1, 0x14, 0x98, 0x02, 0x28, 0x1f, 0xd1, 0x01, 0x20, 0x00, 0x2f, 0x00, 0x90, 0x04, 0xd1, 0xe0, 0x8a, 0x01, 0x27, 0x01, 0x06, 0x09, 0x0e, 0x03, 0x91, 0xe0, 0x8a, 0x03, 0x99, 0x88, 0x42, 0x09, 0xd1, 0x51, 0x48, 0x01, 0x68, 0x2f, 0x20, 0x40, 0x01, 0x08, 0x18, 0x82, 0x7d, 0x02, 0x32, 0x40, 0x2a, 0x6c, 0xd8, 0x4b, 0xe0, 0x4c, 0x48, 0x2f, 0x22, 0x00, 0x68, 0x52, 0x01, 0x80, 0x18, 0x80, 0x7d, 0x00, 0x28, 0x63, 0xd1, 0x83, 0xe0, 0x02, 0x99, 0x20, 0x1c, 0x77, 0xf0, 0xc3, 0xfe, 0x01, 0x90, 0x77, 0xf0, 0xae, 0xfe, 0x01, 0x99, 0x23, 0x29, 0x06, 0xd0, 0x07, 0x99, 0x00, 0x29, 0x7e, 0xd1, 0x21, 0x88, 0x1b, 0x9a, 0x11, 0x80, 0x5f, 0xe7, 0x6b, 0x46, 0x18, 0x8b, 0x21, 0x88, 0x00, 0x09, 0x00, 0x01, 0x08, 0x1a, 0x01, 0x28, 0x06, 0xdd, 0xa0, 0x8a, 0xc0, 0x07, 0x12, 0xd1, 0x07, 0x98, 0x00, 0x28, 0x6c, 0xd1, 0x08, 0xe0, 0x04, 0x98, 0x00, 0x68, 0x80, 0x69, 0x00, 0x78, 0x80, 0x07, 0x08, 0xd4, 0x07, 0x98, 0x00, 0x28, 0x62, 0xd1, 0x20, 0x88, 0x1b, 0x9a, 0x10, 0x80, 0x02, 0x20, 0x42, 0xe7, 0x27, 0xe0, 0x00, 0x2f, 0x04, 0xd1, 0xe0, 0x8a, 0x01, 0x27, 0x01, 0x06, 0x09, 0x0e, 0x03, 0x91, 0xe0, 0x8a, 0x03, 0x99, 0x88, 0x42, 0x2a, 0xd1};
static const uint8_t hci_m2_set_uuid_char_desc_patch_3[] = {0x01, 0x08, 0xfc, 0xc6, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x14, 0x56, 0xff, 0x10, 0xbc, 0xd8, 0x73, 0x05, 0x00, 0x2a, 0x48, 0x01, 0x68, 0x2f, 0x20, 0x40, 0x01, 0x08, 0x18, 0x82, 0x7d, 0x02, 0x32, 0x40, 0x2a, 0x48, 0xd8, 0x49, 0x19, 0x0b, 0x22, 0xd2, 0x01, 0x89, 0x18, 0x6b, 0x46, 0x1a, 0x7e, 0x0b, 0x1c, 0x20, 0x33, 0x9a, 0x75, 0x6b, 0x46, 0x5a, 0x7e, 0x0b, 0x1c, 0x20, 0x33, 0xda, 0x75, 0x81, 0x7d, 0x02, 0x35, 0x2d, 0x06, 0x2d, 0x0e, 0x02, 0x31, 0x81, 0x75, 0x01, 0x20, 0x05, 0x90, 0xe4, 0x69, 0x00, 0x2c, 0x00, 0xd0, 0x43, 0xe7, 0x0c, 0x99, 0x01, 0x31, 0x09, 0x06, 0x09, 0x0e, 0x0c, 0x91, 0x0a, 0xe0, 0x29, 0xe0, 0x26, 0xe0, 0x15, 0x48, 0x2f, 0x22, 0x00, 0x68, 0x52, 0x01, 0x80, 0x18, 0x80, 0x7d, 0x00, 0x28, 0x1e, 0xd1, 0x15, 0xe0, 0x0e, 0x98, 0x0c, 0x99, 0x81, 0x42, 0x00, 0xd8, 0x22, 0xe7, 0x0d, 0x98, 0x10, 0x99, 0x01, 0x30, 0x00, 0x06, 0x00, 0x0e, 0x88, 0x42, 0x0d, 0x90, 0x00, 0xd8, 0x17, 0xe7, 0x09, 0x48, 0x2f, 0x22, 0x00, 0x68, 0x52, 0x01, 0x80, 0x18, 0x80, 0x7d, 0x00, 0x28, 0x04, 0xd1, 0x11, 0x98, 0x1b, 0x9a, 0x10, 0x80, 0x0a, 0x20, 0xe7, 0xe6, 0x00, 0x98, 0x00, 0x28, 0x01, 0xd1, 0x00, 0x20, 0xe2, 0xe6, 0x13, 0x20, 0xe0, 0xe6, 0xbc, 0x5c, 0x04, 0x00, 0x00, 0x63, 0x04, 0x00, 0x02, 0x28, 0x00, 0x00};
static const uint8_t hci_m2_set_uuid_char_desc_patch_4[] = {0x01, 0x08, 0xfc, 0x0b, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x14, 0x57, 0xff, 0x02, 0x00, 0x01};
static const uint8_t hci_m2_set_uuid_char_desc_patch_5[] = {0x01, 0x08, 0xfc, 0x2f, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x14, 0x55, 0xff, 0x10, 0x25, 0x01, 0x34, 0x6e, 0x0d, 0x00, 0x0f, 0x20, 0x06, 0x90, 0xf8, 0xf7, 0xa7, 0xf9, 0xff, 0x28, 0x01, 0xd1, 0x0e, 0x20, 0xe5, 0xe7, 0xee, 0xf7, 0xed, 0xfc, 0x44, 0x20, 0x9d, 0x4a, 0x78, 0x43, 0x10, 0x58, 0x00, 0x28, 0x01, 0xd1};
static const uint8_t hci_m2_set_uuid_char_desc_patch_6[] = {0x01, 0x08, 0xfc, 0x0b, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x14, 0x57, 0xff, 0x02, 0x01, 0x01};
static const uint8_t hci_m2_set_uuid_char_desc_patch_7[] = {0x01, 0x08, 0xfc, 0x2f, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x14, 0x55, 0xff, 0x10, 0x25, 0x02, 0x94, 0x71, 0x0d, 0x00, 0x08, 0x9a, 0x10, 0x2a, 0x01, 0xd3, 0x0f, 0x22, 0x08, 0x92, 0x07, 0x22, 0x06, 0x92, 0x64, 0xe0, 0x07, 0x9a, 0x5a, 0xe0, 0x0a, 0x99, 0x44, 0x23, 0xc9, 0x4a, 0x59, 0x43, 0x89, 0x18, 0x09, 0x9a};
static const uint8_t hci_m2_set_uuid_char_desc_patch_8[] = {0x01, 0x08, 0xfc, 0x0b, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x14, 0x57, 0xff, 0x02, 0x02, 0x01};

struct hci_patch
{
  uint8_t const *buf;
  int len;
};

static const struct hci_patch patches[] = {
    {hci_m2_set_rf_patch_0, sizeof(hci_m2_set_rf_patch_0)},
    {hci_m2_set_rf_patch_1, sizeof(hci_m2_set_rf_patch_1)},
    {hci_m2_set_rf_patch_2, sizeof(hci_m2_set_rf_patch_2)},
    {hci_m2_set_rf_patch_3, sizeof(hci_m2_set_rf_patch_3)},
    {hci_m2_set_uuid_char_desc_patch_0, sizeof(hci_m2_set_uuid_char_desc_patch_0)},
    {hci_m2_set_uuid_char_desc_patch_1, sizeof(hci_m2_set_uuid_char_desc_patch_1)},
    {hci_m2_set_uuid_char_desc_patch_2, sizeof(hci_m2_set_uuid_char_desc_patch_2)},
    {hci_m2_set_uuid_char_desc_patch_3, sizeof(hci_m2_set_uuid_char_desc_patch_3)},
    {hci_m2_set_uuid_char_desc_patch_4, sizeof(hci_m2_set_uuid_char_desc_patch_4)},
    {hci_m2_set_uuid_char_desc_patch_5, sizeof(hci_m2_set_uuid_char_desc_patch_5)},
    {hci_m2_set_uuid_char_desc_patch_6, sizeof(hci_m2_set_uuid_char_desc_patch_6)},
    {hci_m2_set_uuid_char_desc_patch_7, sizeof(hci_m2_set_uuid_char_desc_patch_7)},
    {hci_m2_set_uuid_char_desc_patch_8, sizeof(hci_m2_set_uuid_char_desc_patch_8)},
};
#define NUM_PATCHES (int)(sizeof(patches) / sizeof(patches[0]))

int hci_api_tc35661_update_firmware(void)
{
  int ret;
  int i;

  printf("hci_api_tc35661_update_firmware: loop HCI_LOC_DBUS_WRITE_RF_PATCH_NUM_ENTRIES...\n");

  for(i = 0; i < HCI_LOC_DBUS_WRITE_RF_PATCH_NUM_ENTRIES; i++)
  {
    const struct hci_api_dbus_write_command *w = &hci_loc_dbus_write_rf_patch[i];

    ret = hci_api_execute_dloc_write(w);

    if(ret)
    {
      printf("hci_api_execute_dloc_write() failed @ entry %d\n", i);
      return -1;
    }

    usleep(1000);
  }

  printf("hci_api_tc35661_update_firmware: loop NUM_PATCHES...\n");

  for(i = 0; i < NUM_PATCHES; i++)
  {
 	const   struct hci_patch *p = &patches[i];

 	ret = hci_api_execute_m2_set(p->buf, p->len);

 	if(ret)
 	{
      printf("hci_api_execute_m2_set() failed @ patch %d\n", i);
      return -1;
    }

 	usleep(1000);
  }

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
