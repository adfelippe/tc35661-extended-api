/***************************************************************************/
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

#ifndef __MESSAGE__
#define __MESSAGE__

int message_get_hci_timeout(uint8_t *buf, int length, int timeout_ms);
int message_get_extended_hci_timeout(uint8_t *buf, int length, int timeout_ms);
int message_put_unexpected_event(uint8_t *buf, int length);
int message_get_tcu_timeout(uint8_t *buf, int length, int timeout_ms);
#endif
