#include "comms.h"
#include <inttypes.h>
#include <stddef.h>

void send(uint8_t data[]){
    
    uint8_t header = sizeof(*data);

}

uint32_t crc32(const uint8_t data[], size_t data_length, const uint32_t crctable[256]) {
	uint32_t crc32 = 0xFFFFFFFFu;
	
	for (size_t i = 0; i < data_length; i++) {
		const uint32_t lookupIndex = (crc32 ^ data[i]) & 0xff;
		crc32 = (crc32 >> 8) ^ crctable[lookupIndex];  // CRCTable is an array of 256 32-bit constants
	}
	
	// Finalize the CRC-32 value by inverting all the bits
	crc32 ^= 0xFFFFFFFFu;
	return crc32;
}

const uint32_t * make_crc_table()
{
	uint32_t crc_table[256];
	
	uint32_t c;
	int16_t n, k;

	for (n = 0; n < 256; n++) {
	c = (uint32_t) n;
	for (k = 0; k < 8; k++) {
		if (c & 1)
		c = 0xedb88320L ^ (c >> 1);
		else
		c = c >> 1;
	}
	crc_table[n] = c;
	}
	
	return crc_table;
}
