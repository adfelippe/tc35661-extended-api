# tc35661-extended-api
## Extended API for the Toshiba TC35661 Bluetooth radio

TC35661 Extended API for vendor-specific radio settings HCI commands.
These additions are useful for anyone wishing to control PAN1026/TC35661 radio settings in order to go under any certification test.
The original Toshiba API has also been modified in order to run on Linux.

### EXTENDED HCI COMMANDS
* hci_api_set_afh_host_channel_classification
* hci_api_loc_operation_mode_set
* hci_api_set_hopping_mode
* hci_api_loc_set_whitening_mode
* hci_api_non_modulated_carrier
* hci_api_loc_tx_mode_set
* hci_api_loc_sender_receiver
* hci_api_loc_tx_slot_length
* hci_api_write_page_scan_activity
* hci_api_write_scan_enable
* hci_api_set_event_filter_for_certif_test
* hci_enable_device_under_test_mode

### HOW TO IMPLEMENT THIS TOSHIBA API
Two modules have to be modified for your platform:

* board.c and .h
* bluetooth_serial.c and .h

They are the abstraction layer for your hardware settings and the serial port interface connected to the PAN1026/TC35661 module.
This version of mine is tweaked to run on any Linux embedded system with only a few modifications.

## _TO-DO_
* Improve abstractions to ease hardware portability.
* Completely review code style since Toshiba code style stinks. (I mixed it up with mine, so it's a mess)
* Maybe create a modular port of board and serial modules for several architectures and easily set them up with a config/Makefile?
