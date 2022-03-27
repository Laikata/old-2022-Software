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

void comms_imu(vec3_t mag, vec3_t accel, vec3_t gyro, float hoz) {
	static const int data_size = sizeof(uint8_t) + (sizeof(vec3_t) * 3) + sizeof(float);
	uint8_t data[data_size];

	data[0] = 0x02;
	vec3_t allvecs[3] = {mag, accel, gyro};
	// NOTE: This depends on endianness!
	for(int i = 0; i < 3; i++) {
		memcpy(&data[(i*sizeof(vec3_t))+1], &allvecs[i].x, sizeof(float));
		memcpy(&data[(i*sizeof(vec3_t))+5], &allvecs[i].y, sizeof(float));
		memcpy(&data[(i*sizeof(vec3_t))+9], &allvecs[i].z, sizeof(float));
	}
	memcpy(&data[36], &hoz, 4);

	comms_send(data, data_size);
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
