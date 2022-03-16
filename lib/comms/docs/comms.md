# Protocolo de comunicación v0.1

## Inicio de paquete
Todos los paquetes empiezan con el caracter de control SYN (0x16) dos veces

## Header
Un unsigned int 16 especifica como de largo va a ser el paquete

## Datos
Se transmiten todos los datos con la longitud establecida en el header

## Checksum
Se envía un CRC-32 de todos los datos para comprobación de errores

## Ejemplo
SYN xx todoslosdatosnecesarios yyyyyyyy

xx -> tamaño del paquete en bytes
yyyy... -> checksum CRC-32 (32 bits o 4 bytes)