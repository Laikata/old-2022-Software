# Communication protocol v2.0

## Base packet
A packet is composed by five parts:
 - SYN(`0x16`) character: marks begining of a packet
 - Header: An 8-bit long unsigned integer that establishes the packet size in bytes
 - Counter: An ascending 8-bit long unsigned integer used to check for packet loss
 - Data: Whatever data needs to be transfered
 - Checksum: 4-byte long CRC-32 checksum of 'Data' to validate integrity

### Example
`SYN 14 1 somerandomdata crc-32_cksum`

## Subprotocols
All subprotocols are wrapped in the base packet and its first byte defines which kind of packet it is.
### GPS
- Identifier: `0x01`
- Size: `13 bytes`
- Structure: `0x01 Latitude(float) Longitude(float) Altitude(float)`

### IMU
- Identifier: `0x02`
- Size: `41 bytes`
- Structure: `0x02 Magnetometer(vec3) Accelerometer(vec3) Gyroscope(vec3) Horizontal(float)`

### ENV
- Identifier: `0x03`
- Size: `13 bytes`
- Structure: `0x03 Temperature(float) Humidity(float) Pressure(float)`

### BAT
- Identifier: `0x04`
- Size: `5 bytes`
- Structure: `0x04 Voltage(float)`

### DEBUG
- Identifier: `0x05`
- Size: `arbitrary` (any string with length up to 256)
- Strucutre: `0x05 Message(not null terminated string)`
