/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/
#include <stdio.h>
#include "nvm_api.h"
#include "key_store_ble_api.h"
#include "internal_apis.h"
#include "board.h"
//#include "debug.h"

/* Defining this macro disables all debug messages. */
#define printf(fmt, args...)	/* NOPRINTF */

int nvm_init(int nvm_size)
{
  int i;
  int j;
  uint32_t offset = 0;

  for (i = 0;; i++) {
    struct nvm_partition *pi = nvm_partitions[i];
    if (!pi)
      break;
    for (j = 0;; j++) {
      struct nvm_partition *p = &pi[j];
      if (p->type == nvm_partition_end)
        break;
      p->offset = offset;
      if (p->len == 0) {
        p->len = nvm_size - p->offset;
      }
      offset += p->len;
      printf("i %d, j %d -> %p, len %d, offset %d, type %d\n", i, j, p, p->len, p->offset, p->type);
    }
  }

  return 0;
}

int nvm_write(enum nvm_partition_type type, uint32_t offset, uint32_t len, uint8_t *buf)
{
  struct nvm_partition *p;
  int i;
  int j;
  int ret;

  for (i = 0;; i++) {
    struct nvm_partition *pi = nvm_partitions[i];
    if (!pi)
      break;
    for (j = 0;; j++) {
      p = &pi[j];
      if (p->type == nvm_partition_end)
        break;
      if (p->type == type)
        goto found;
    }
  }

  return -1;

found:
  /* sanity checks */
  if (len > p->len) {
    printf("len %d out of bounds\n", len);
    return -1;
  }

  if (offset + len > p->len) {
    printf("offset %d + len %d out of bounds\n", offset, len);
    return -1;
  }

  ret = board_nvm_write(p->offset + offset, buf, len);
  if (ret) {
    printf("board_nvm_write() failed\n");
    return -1;
  }

  printf("@ %d, len %d\n", p->offset + offset, len);
#ifdef DEBUG
  {
    int i;
    printf("[ ");
    for (i = 0; i < len; i++) {
      printf("%02x ", buf[i]);
    }
    printf("]\n");
  }
#endif

  return 0;
}

int nvm_read(enum nvm_partition_type type, uint32_t offset, uint32_t len, uint8_t *buf)
{
  struct nvm_partition *p;
  int i;
  int j;
  int ret;

  for (i = 0;; i++) {
    struct nvm_partition *pi = nvm_partitions[i];
    if (!pi)
      break;
    for (j = 0;; j++) {
      p = &pi[j];
      printf("i %d, j %d -> %p, len %d, offset %d, type %d\n", i, j, p, p->len, p->offset, p->type);
      if (p->type == nvm_partition_end)
        break;
      if (p->type == type)
        goto found;
    }
  }

  return -1;

found:
  /* sanity checks */
  if (len > p->len) {
    printf("len %d out of bounds\n", len);
    return -1;
  }

  if (offset + len > p->len) {
    printf("offset %d + len %d out of bounds\n", offset, len);
    return -1;
  }

#if 1
  {
    int i;
    printf("board_nvm_read [ ");
    for (i = 0; i < len; i++) {
      printf("%02x ", buf[i]);
    }
    printf("]\n");
  }
#endif
  ret = board_nvm_read(p->offset + offset, buf, len);
  if (ret) {
    printf("board_nvm_read() failed\n");
    return -1;
  }

  printf("@ %d, len %d\n", p->offset + offset, len);
//#ifdef DEBUG
#if 1
  {
    int i;
    printf("[ ");
    for (i = 0; i < len; i++) {
      printf("%02x ", buf[i]);
    }
    printf("]\n");
  }
#endif
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
