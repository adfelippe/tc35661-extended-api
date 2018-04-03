/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "key_store_bt_api.h"
#include "nvm_api.h"
//#include "board_specific.h"
//#include "debug.h"
#include "crc8.h"
#include "internal_apis.h"

/* Defining this macro disables all debug messages. */
#define printf(fmt, args...)	/* NOPRINTF */

#define NUM_KEY_ELEMENTS 16
#define NUM_KEY_BT_ELEMENTS 16
/*
* The data storage is split up into 2 parts: an index part and a data part.
* The index part is always kept in memory, because access to the NVM may be
* slow. Also the corresponding data in one entry may be huge.
*
* The index part is used to look-up the data from the data part, which is only
* referenced if actual data is read or written.
*/

static struct key_store_bt_index_element key_index[NUM_KEY_BT_ELEMENTS];

int key_store_bt_reset(void)
{
  int i;
  int ret;

  for (i = 0; i < NUM_KEY_ELEMENTS; i++) {
    struct key_store_bt_index_element *e = &key_index[i];
    memset(e, 0, sizeof(*e));
    e->crc8 = crc(((uint8_t *)e) + 1, sizeof(*e) - 1);
    ret = nvm_write(nvm_key_store_bt_index, sizeof(*e) * i, sizeof(*e), (uint8_t *)e);
    if (ret) {
      printf("nvm_write() failed @ key_index %d\n", i);
    }
  }

  return 0;
}

int key_store_bt_init(void)
{
  int i;
  int ret;

  for (i = 0; i < NUM_KEY_BT_ELEMENTS; i++) {
    struct key_store_bt_index_element *e = &key_index[i];

      printf("nvm_read() started %d ************************\n",i);
    ret = nvm_read(nvm_key_store_bt_index, sizeof(*e) * i, sizeof(*e), (uint8_t *)e);
    if (ret) {
      printf("nvm_read() failed @ key_index %d\n", i);
      return -1;
    }

      printf("nvm_read() success\n");
      sleep(1);
    uint8_t crc8 = crc(((uint8_t *)e) + 1, sizeof(*e) - 1);
    if (crc8 != e->crc8) {
      printf("crc mismatch @ key_index %d. erasing entry\n", i);
      memset(e, 0, sizeof(*e));
      e->crc8 = crc(((uint8_t *)e) + 1, sizeof(*e) - 1);
      printf("NVM_WRITE*****************\n");
      ret = nvm_write(nvm_key_store_bt_index, sizeof(*e) * i, sizeof(*e), (uint8_t *)e);
      if (ret) {
        printf("nvm_read() failed @ key_index %d\n", i);
      }
    }
    printf("key_index %d: in_use %d\n", i, e->in_use);
  }

  printf("\n");
  return 0;
}

int key_store_bt_store_link_key(const uint8_t *bd_address, const uint8_t *link_key)
{
  struct key_store_bt_element key_element;
  int i;
  int ret;

  /* try to find if an entry is already present */
  for (i = 0; i < NUM_KEY_BT_ELEMENTS; i++) {
    if (key_index[i].in_use) {
      ret = memcmp(bd_address, key_index[i].bd_address, TCU_SIZE_BDADDR);
      if (!ret)
        break;
    }
  }
  if (i != NUM_KEY_BT_ELEMENTS) {
    /* entry is already present, update */
    printf("found existing member for address. updating\n");
    goto out;
  }

  /* try to find if an empty entry */
  for (i = 0; i < NUM_KEY_BT_ELEMENTS; i++) {
    if (!key_index[i].in_use) {
      break;
    }
  }
  if (i == NUM_KEY_BT_ELEMENTS) {
    /* no more free entries. fixme.  */
    printf("no more free entries\n");
    return -2;
  }

out:
  /* write key_index */
  memcpy(key_index[i].bd_address, bd_address, TCU_SIZE_BDADDR);
  key_index[i].in_use = 1;
  key_index[i].crc8 = crc(((uint8_t *)&key_index[i]) + 1, sizeof(key_index[i]) - 1);
  ret = nvm_write(nvm_key_store_bt_index, sizeof(struct key_store_bt_index_element) * i, sizeof(struct key_store_bt_index_element), (uint8_t *)&key_index[i]);
  if (ret) {
    printf("nvm_write() failed @ key_index\n");
  }

  /* write data */
  memset(&key_element, 0, sizeof(key_element));
  memcpy(key_element.link_key, link_key, TCU_SIZE_LINKKEY);
  key_element.crc8 = crc(((uint8_t *)&key_element) + 1, sizeof(key_element) - 1);

  ret = nvm_write(nvm_key_store_bt_data, sizeof(struct key_store_bt_element) * i, sizeof(struct key_store_bt_element), (uint8_t *)&key_element);
  if (ret) {
    printf("nvm_write() failed @ data\n");
    return -1;
  }

  printf("stored @ key_index %d, crc %02x\n", i, key_index[i].crc8);

  return 0;
}

int key_store_bt_delete_link_key(const uint8_t *bd_address)
{
  int i;
  int ret;

  printf("bd_address %02x:%02x:%02x:%02x:%02x:%02x\n", bd_address[0], bd_address[1], bd_address[2], bd_address[3], bd_address[4], bd_address[5]);

  for (i = 0; i < NUM_KEY_BT_ELEMENTS; i++) {
    if (key_index[i].in_use) {
      ret = memcmp(bd_address, key_index[i].bd_address, TCU_SIZE_BDADDR);
      if (!ret)
        break;
    }
  }
  if (i == NUM_KEY_BT_ELEMENTS) {
    printf("member not found\n");
    return -1;
  }

  memset(&key_index[i], 0, sizeof(key_index[i]));
  key_index[i].crc8 = crc(((uint8_t *)&key_index[i]) + 1, sizeof(key_index[i]) - 1);

  ret = nvm_write(nvm_key_store_bt_index, sizeof(struct key_store_bt_index_element) * i, sizeof(struct key_store_bt_index_element), (uint8_t *)&key_index[i]);
  if (ret) {
    printf("nvm_write() failed @ key_index\n");
  }

  printf("cleared @ key_index %d, crc %02x\n", i, key_index[i].crc8);
  return 0;
}

int key_store_bt_retrieve_link_key(const uint8_t *bd_address, uint8_t *link_key)
{
  struct key_store_bt_element key_element;
  int i;
  int ret;

  printf("bd_address %02x:%02x:%02x:%02x:%02x:%02x\n", bd_address[0], bd_address[1], bd_address[2], bd_address[3], bd_address[4], bd_address[5]);

  for (i = 0; i < NUM_KEY_BT_ELEMENTS; i++) {
    if (key_index[i].in_use) {
      ret = memcmp(bd_address, key_index[i].bd_address, TCU_SIZE_BDADDR);
      if (!ret)
        break;
    }
  }
  if (i == NUM_KEY_BT_ELEMENTS) {
    printf("member not found\n");
    return -1;
  }

  ret = nvm_read(nvm_key_store_bt_data, sizeof(struct key_store_bt_element) * i, sizeof(struct key_store_bt_element), (uint8_t *)&key_element);
  if (ret) {
    printf("nvm_read() failed @ data\n");
    return -1;
  }

  uint8_t crc8 = crc(((uint8_t *)&key_element) + 1, sizeof(key_element) - 1);
  if (crc8 != key_element.crc8) {
    printf("crc failed. erasing entry\n");
    key_store_bt_delete_link_key(bd_address);
    return -1;
  }

  printf("found @ key_index %d\n", i);

  memcpy(link_key, key_element.link_key, sizeof(key_element.link_key));

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
