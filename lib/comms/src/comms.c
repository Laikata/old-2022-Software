#include "comms.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

void send(uint8_t data[], uint16_t data_length){
    
	// SYN char + header + data + crc32 checksum
	size_t packet_size = 1 + 2 + data_length + 4;
	uint8_t packet[packet_size];

	packet[0] = 0x16;

	packet[1] = data_length & 0x00ff;
	packet[2] = data_length & 0xff00 >> 8;

	for (int i = 0; i < data_length; i++) {
		packet[i + 3] = data[i];
	}

	// TODO: Make the table a const in the header
	const uint32_t* crc_table = make_crc_table();

	//Maybe add the whole packet?
	uint32_t checksum = crc32(data, data_length, crc_table);

	packet[2 + data_length + 1] = checksum & 0x000000ff;
	packet[2 + data_length + 2] = checksum & 0x0000ff00 >> 8;
	packet[2 + data_length + 3] = checksum & 0x00ff0000 >> 16;
	packet[2 + data_length + 4] = checksum & 0xff000000 >> 24;

	Serial.write(packet, packet_size);
}

int read(char *data[]) {
	uint8_t header[2];
	Serial.readBytes(header, 2);

	//TODO: This should work
	uint16_t packet_size = (uint16_t) header[0] + 
						   ( (uint16_t) header[1] << 8);
	uint8_t packet_data[packet_size];
	Serial.readBytes(packet_data, packet_size);

	uint8_t crc32_buffer[4];
	Serial.readBytes(crc32_buffer, 4);
	uint32_t checksum = (uint32_t) crc32_buffer[] +
						( (uint32_t) crc32_buffer[] << 8) +
						( (uint32_t) crc32_buffer[] << 16) +
						( (uint32_t) crc32_buffer[] << 24)

	// TODO: Make the table a const in the header
	const uint32_t* crc_table = make_crc_table();

	if (checksum != crc32(packet_data, packet_size, crc_table)) {
		// Checksums do not match!
		return 1;
	}

	char* result = malloc((packet_size + 1) * sizeof(uint8_t));
	memcpy(result, packet_data, packet_size * sizeof(uint8_t));
	memcpy(result + packet_size, '\0', sizeof(uint8_t));

	data = result;
	return 0;
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
