#include "comms.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <Arduino.h>

void send(uint8_t data[], uint16_t data_length){
    
	// SYN char + header + data + crc32 checksum
	size_t packet_size = 1 + 2 + data_length + 4;
	uint8_t packet[packet_size];

	packet[0] = 0x16;

	packet[1] = (data_length >> 8) & 0xff;
	packet[2] = data_length & 0xff;

	for (int i = 0; i < data_length; i++) {
		packet[i + 3] = data[i];
	}

	uint32_t checksum = crc32(data, data_length);

	packet[2 + data_length + 1] = (checksum >> 24) & 0xff;
	packet[2 + data_length + 2] = (checksum >> 16) & 0xff;
	packet[2 + data_length + 3] = (checksum >> 8) & 0xff;
	packet[2 + data_length + 4] = checksum & 0xff;

	//Serial.write(packet, packet_size);
}

int recv(char *data[]) {
	uint8_t header[2];
	//Serial.readBytes(header, 2);
	uint16_t packet_size = (uint16_t) header[0] + 
						   ( (uint16_t) header[1] << 8);

	uint8_t packet_data[packet_size + 1];
	//Serial.readBytes(packet_data, packet_size);
	packet_data[packet_size] = '\0';

	uint8_t crc32_buffer[4];
	//Serial.readBytes(crc32_buffer, 4);
	uint32_t checksum = (crc32_buffer[0] << 24) +
						(crc32_buffer[1] << 16) +
						(crc32_buffer[2] << 8) +
						crc32_buffer[3];


	if (checksum != crc32(packet_data, packet_size)) {
		// Checksums do not match!
		return 1;
	}

	data = (char **) packet_data;

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
