#pragma once
#include <inttypes.h>
#include <stddef.h>


void send(uint8_t data[], uint16_t data_length);
const uint32_t * make_crc_table();
uint32_t crc32(const uint8_t data[], size_t data_length, const uint32_t crctable[256]);