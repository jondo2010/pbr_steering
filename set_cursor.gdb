echo Display On\n
set *((uint8_t *)(0x1101)) = 0x46
set *((uint8_t *)(0x1100)) = 0x00
set *((uint8_t *)(0x1100)) = 0x05
echo Done\n
