The iNemo STEVAL-MKI121V1 board does not work with openocd out of the box. The included firmware uses DFU and prevents the SWD connector from working.

You have to unsolder R39 and R40. These are 0 Ohm resistors which connect INemo's pins 16 and 18 to the push button and USB EN. 
Then you have to upload any firmare (choose any example) with the USART1 port. You can use an USB to serial cable for that, and the stm32flash software.
After uploading this software, openocd should be able to connect to the board, with the configuration file in this directory.
You can solder back R39 and R40 at this stage.

