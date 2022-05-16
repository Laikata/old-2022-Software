#include <vector.h>
#include <gps.h>
#include <nav.h>
#include <servos.h>
#include <dir.h>
#include <Arduino.h>
#include <Servo.h>
#include <comms.h>
#include <Adafruit_BMP085.h>
#include <ErriezDHT22.h>
#include <imu.h>
#include "eeprom_utils.h"
#include <LowPass.h>

#define DHT22_PIN D4

// LAT, LON, HEI
#define DEST_COORDS 38.831541514133754, 0.10366977616384526, 0

Servos servo(5);
MPU9250 mpu;
Adafruit_BMP085 bmp;
DHT22 dht22(DHT22_PIN);
vec3_t g_destCord = {0, 0, 0};

void setup(){
    Serial.begin(9600);
    EEPROM.begin(0x80);
    Wire.begin();

    //TODO: Add calibrate branch to main repo
    mpu.setup(0x68);
    dht22.begin();
    bmp.begin();
    gps_init();
    servo.attach();

    //Serial.println("Starting Program");

    loadCalibration();
}

void moveServos(vec3_t *gps_pos, float mag_hoz);

void loop(){
    
    vec3_t can_position = gps_position();
    comms_gps(can_position.x, can_position.y, can_position.z);

    // Recibir datos sensores
    float north_dir = 0;
    if(mpu.update()) {
        north_dir = mpu.getMagHoz();
        Serial.printf("MAGN: (%g, %g, %g)\n", mpu.getMagX(), mpu.getMagY(), mpu.getMagZ());
        vec3_t magnetometer = {mpu.getMagX(), mpu.getMagY(), mpu.getMagZ()};
        vec3_t gyroscope = {mpu.getGyroX(), mpu.getGyroY(), mpu.getGyroZ()};
        vec3_t accelerometer = {mpu.getAccX(), mpu.getAccY(), mpu.getAccZ()};
        comms_imu(magnetometer, gyroscope, accelerometer, north_dir);
        moveServos(&can_position, north_dir);

    }
    float rotation = nav_angle(&can_position, &g_destCord, north_dir);

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
}


void moveServos(vec3_t *gps_pos, float mag_hoz){
    float direction;
    float realDirection = 0;
    static LowPassFilter lowPass;

    calculate_direction(&direction, DEST_COORDS, gps_pos->x, gps_pos->y, 0);
    
    realDirection = lowPass.low_pass(mag_hoz);
    direction = map(direction, 0, 2 * PI, 0, 360);

    direction = direction - realDirection;
    Serial.printf("DIR: %g\n", direction);
    Serial.printf("REALDIR: %g\n", realDirection);
    Serial.printf("MAGHOZ: %g\n", mag_hoz);

    float mappedDirection = map(direction, -180, 180, -50, 50);

    servo.angleRight(50 - mappedDirection);
    servo.angleLeft(50 + mappedDirection);

    //Serial.println(direction);
}
