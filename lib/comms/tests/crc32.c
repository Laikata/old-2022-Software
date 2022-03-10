#include "../src/comms.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

int main() {
	const uint32_t * crc_table = make_crc_table();

	uint8_t data[3] = {'h', 'e', 'y'};
	uint32_t result = crc32(data, sizeof(data), crc_table);

	printf("%u", result);
}