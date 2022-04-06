#include "comms.h"

void comms_init() {
	Serial.begin(9600);
}

void comms_send(uint8_t data[], uint8_t data_length){
    
	// SYN char + header + counter + data + crc32 checksum
	uint16_t packet_size = 1 + 1 + 1 + data_length + 4;
	static uint8_t counter = 0;
	uint8_t packet[packet_size];

	packet[0] = 0x16;
	packet[1] = data_length;
	packet[2] = counter;

	for (uint32_t i = 0; i < data_length; i++) {
		packet[i + 3] = data[i];
	}

	uint32_t checksum = crc32(data, data_length);

	//TODO: Test whether this is sent in big endian or not
	packet[2 + data_length + 1] = (checksum >> 24) & 0xff;
	packet[2 + data_length + 2] = (checksum >> 16) & 0xff;
	packet[2 + data_length + 3] = (checksum >> 8) & 0xff;
	packet[2 + data_length + 4] = checksum & 0xff;

	counter++; // This will overflow and that is okay
	Serial.write(packet, packet_size);
}

// This should work but I'm not really sure...
float ReverseFloat(float value) {
    union {
		float f;
		uint32_t i;
	} f32_u8 = {.f = value};

    f32_u8.i = __builtin_bswap32(f32_u8.i);

	return f32_u8.f;
}

inline float BigEndianFloat(float value) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	return ReverseFloat(value);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	return value;
#else
#	error Unsupported endianness
#endif
}

void comms_imu(vec3_t mag, vec3_t accel, vec3_t gyro, float hoz) {
	static const int data_size = sizeof(uint8_t) + (sizeof(vec3_t) * 3) + sizeof(float);
	uint8_t data[data_size];

	data[0] = 0x02;
	vec3_t * allvecs[3] = {&mag, &accel, &gyro};
	for(int i = 0; i < 3; i++) {
		float x = BigEndianFloat(allvecs[i]->x);
		float y = BigEndianFloat(allvecs[i]->y);
		float z = BigEndianFloat(allvecs[i]->z);

		memcpy(&data[(i*sizeof(vec3_t))+1], &x, sizeof(float));
		memcpy(&data[(i*sizeof(vec3_t))+5], &y, sizeof(float));
		memcpy(&data[(i*sizeof(vec3_t))+9], &z, sizeof(float));
	}
	memcpy(&data[36], &hoz, 4);

	comms_send(data, data_size);
}

void comms_env(float temp, float humidity, float pressure) {
	static const int data_size = sizeof(uint8_t) + sizeof(float) * 3;
	uint8_t data[data_size];

	float bige_temp = BigEndianFloat(temp);
	float bige_hum = BigEndianFloat(humidity);
	float bige_pres = BigEndianFloat(pressure);

	data[0] = 0x03;
	memcpy(&data[1], &bige_temp, sizeof(float));
	memcpy(&data[5], &bige_hum, sizeof(float));
	memcpy(&data[9], &bige_pres, sizeof(float));

	comms_send(data, data_size);
}

void comms_gps(float longitude, float latitude, float altitude) {
	static const int data_size = sizeof(uint8_t) + sizeof(float) * 3;
	uint8_t data[data_size];

	float bige_lon = BigEndianFloat(longitude);
	float bige_lat = BigEndianFloat(latitude);
	float bige_alt = BigEndianFloat(altitude);

	data[0] = 0x01;
	memcpy(&data[1], &bige_lon, sizeof(float));
	memcpy(&data[5], &bige_lat, sizeof(float));
	memcpy(&data[9], &bige_alt, sizeof(float));

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
