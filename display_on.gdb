echo Display On\n
set *((uint8_t *)(0x1101)) = 0x59
set *((uint8_t *)(0x1100)) = 0x02 | (1 << 2) | (0 << 4)
echo Done\n
