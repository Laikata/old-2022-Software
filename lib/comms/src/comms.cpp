#include "comms.h"

void comms_init() {
	Serial.begin(9600);
}

void comms_send(uint8_t data[], uint16_t data_length){
    
	// SYN char + header + data + crc32 checksum
	uint16_t packet_size = 1 + 2 + data_length + 4;
	uint8_t packet[packet_size];

	packet[0] = 0x16;

	packet[1] = (data_length >> 8) & 0xff;
	packet[2] = data_length & 0xff;

	for (uint32_t i = 0; i < data_length; i++) {
		packet[i + 3] = data[i];
	}

	uint32_t checksum = crc32(data, data_length);

	packet[2 + data_length + 1] = (checksum >> 24) & 0xff;
	packet[2 + data_length + 2] = (checksum >> 16) & 0xff;
	packet[2 + data_length + 3] = (checksum >> 8) & 0xff;
	packet[2 + data_length + 4] = checksum & 0xff;

	Serial.write(packet, packet_size);
}

int comms_recv(char *data[]) {
	uint8_t header[2];
	Serial.readBytes(header, 2);
	uint16_t packet_size = (header[0] << 8) + 
				header[1];

	uint8_t packet_data[packet_size + 1];
	Serial.readBytes(packet_data, packet_size);
	packet_data[packet_size] = '\0';

	uint8_t crc32_buffer[4];
	Serial.readBytes(crc32_buffer, 4);
	uint32_t checksum = ( ((uint32_t) crc32_buffer[0]) << 24) +
						( ((uint32_t) crc32_buffer[1]) << 16) +
						(crc32_buffer[2] << 8) +
						crc32_buffer[3];


	if (checksum != crc32(packet_data, packet_size)) {
		// Checksums do not match!
		return 1;
	}

	data = (char **) packet_data;

	return 0;
}

static uint32_t crc32(const uint8_t data[], size_t data_length) {
	uint32_t crc32 = 0xFFFFFFFFu;
	
	for (size_t i = 0; i < data_length; i++) {
		const uint32_t lookupIndex = (crc32 ^ data[i]) & 0xff;
		crc32 = (crc32 >> 8) ^ crc_table[lookupIndex];  // CRCTable is an array of 256 32-bit constants
	}
	
	// Finalize the CRC-32 value by inverting all the bits
	crc32 ^= 0xFFFFFFFFu;
	return crc32;
}
