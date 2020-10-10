
Refer to Twiki: http://twiki.na.mindray.gb/bin/view/Main/UsbStreaming

To build Tcp Server for ARM::
----------------------------
<your toolchain GCC for ARM> streamserver.cpp  -o streamserver
for e.g. :
/remote/xtools/timesys/i.MX6QZonareG3Plus/main_trunk_latest/toolchain/bin/armv7l-timesys-linux-gnueabi-gcc streamserver.cpp  -o streamserver

To build Client for Ubuntu:
---------------------------
gcc streamclient.cpp  -o streamclienta


Please refer to the above Twiki page for additional setup and running instructions.

