/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file debug_api.h
*
* @brief Contains definitions and function declarations for debug stuff used
* in the Blueooth stack.
*
* All functions must be implemented by the application environment.
*/
#ifndef __DEBUG_API_H__
#define __DEBUG_API_H__

/* The Bluetooth stack expects 2 functions for debugging:

 - bs_printf()
 - bs_warning()

Both have the same signature as a standard printf().

bs_printf() should only output something if the DEBUG environment variable
is defined. It is used to output detailed debugging informations.

bs_warning() is used to notify the environment of a serious incident. This
incident should never be ignored in debug builds, because it might indicate
that something is not working as expected.

Please note that these functions should be implemented as macros. The main
reason is to give the compiler and linker a chance to eliminate all calls to
these functions in a release build in order to save memory.

A minimal implementation of the functions could look like this:

#define func_printf(...) do { printf("%s(): ", __func__); printf(__VA_ARGS__); } while(0)
#define bs_warning func_printf
#ifdef DEBUG
#define bs_printf func_printf
#else
#define bs_printf(...)
#endif

The Bluetooth stack expects these functions to come "out of nowhere". Because
of this, we simply expect "debug.h" to exist here which must be implemented
by the environment and include the functions as mentioned above.
*/
#include "debug.h"

/*! format string for printing a bd address */
#define BD_ADDR_FORMAT "%02x:%02x:%02x:%02x:%02x:%02x"

/*! argument list for printing a bd_address */
#define BD_ADDR_ARG(x) x[0], x[1], x[2], x[3], x[4], x[5]

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
