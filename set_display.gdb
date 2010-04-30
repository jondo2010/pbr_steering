echo System Set\n
set *((uint8_t *)(0x1101)) = 0x59
set *((uint8_t *)(0x1100)) = 0x02 | (0x01 << 2) | (0x01 << 4) | (0x01 << 6)
echo Done\n
