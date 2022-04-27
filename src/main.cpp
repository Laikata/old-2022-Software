#include <vector.h>
#include <gps.h>
#include <nav.h>
#include <servos.h>
#include <comms.h>
#include <Adafruit_BMP085.h>
#include <ErriezDHT22.h>
#include <imu.h>
#include "eeprom_utils.h"

#define DHT22_PIN D4

MPU9250 mpu;
Adafruit_BMP085 bmp;
DHT22 dht22(DHT22_PIN);
vec3_t *g_destCord = NULL_VEC3;

void setup(){
    EEPROM.begin(0x80);
    Wire.begin();

    g_destCord = vec3_init(0, 0, 0);

    //TODO: Add calibrate branch to main repo
    mpu.setup(0x68);
    dht22.begin();
    bmp.begin();
    gps_init();

    loadCalibration();
}

void loop(){
    vec3_t *can_position = gps_position();

    mpu.update();
    float north_dir = mpu.getMagHoz();

    float rotation = nav_angle(can_position, g_destCord, north_dir);
    
    // Programa de mover el servo empieza aqui :)
    servos_init(0, 3, 5);
    
    // Mover servos

    // Recibir datos sensores
    float north_dir = 0;
    if(mpu.update()) {
        north_dir = mpu.getMagHoz();
        vec3_t magnetometer = {mpu.getMagX(), mpu.getMagY(), mpu.getMagZ()};
        vec3_t gyroscope = {mpu.getGyroX(), mpu.getGyroY(), mpu.getGyroZ()};
        vec3_t accelerometer = {mpu.getAccX(), mpu.getAccY(), mpu.getAccZ()};
        comms_imu(magnetometer, gyroscope, accelerometer, north_dir);
    }


    //   1. Voltaje bateria
    //   2. Temperatura
    //   3. Presión
    // TODO: Read battery voltage

    if(dht22.available()) {
        float temperature = dht22.readTemperature(); // Do we read temp from the BMP085 or the DHT22?
        float pressure = bmp.readPressure();
        float humidity = dht22.readHumidity();
        comms_env(temperature, humidity, pressure);
    }

    // Liberar memoria alocada
}