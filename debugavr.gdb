file Debug/pbr_steering.elf
dir ../libusart
dir ../libxbee
dir ../libspi
dir ../libadc
dir ../libsed1335
dir ../libcan

set remoteaddresssize 32
set remote Z-packet enable
set remote hardware-breakpoint-limit 3
set remote hardware-watchpoint-limit 2

target extended-remote localhost:4242
set radix 16
