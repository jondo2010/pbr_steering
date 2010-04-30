echo System Set\n
set *((uint8_t *)(0x1101)) = 0x46
set *((uint8_t *)(0x1100)) = 0x00
set *((uint8_t *)(0x1100)) = 0x00

set *((uint8_t *)(0x1101)) = 0x42
set *((uint8_t *)(0x1100)) = 'a'
set *((uint8_t *)(0x1100)) = 'b'
set *((uint8_t *)(0x1100)) = 'c'
set *((uint8_t *)(0x1100)) = 'd'

echo Done\n
