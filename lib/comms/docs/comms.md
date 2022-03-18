# Communication protocol v1.0

## Example
SYN xx arbitrarydata yyyyyyyy
    ^                   ^
 pkg size            checksum

A packet is composed by four parts:
 - SYN character (marks begining of package)
 - Header -> a 16-bit long unsigned integer that establishes package size in bytes
 - Data -> Whatever data is transfered
 - Checksum -> a CRC-32 of 'Data' to validate the integrity