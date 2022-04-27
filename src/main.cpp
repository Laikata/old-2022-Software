#include <vector.h>
#include <gps.h>
#include <nav.h>
#include <servos.h>
#include <Arduino.h>
#include <Servo.h>
#include <comms.h>
#include <Adafruit_BMP085.h>
#include <ErriezDHT22.h>
#include <imu.h>

#define DHT22_PIN D4

Servos servo(5);
MPU9250 mpu;
Adafruit_BMP085 bmp;
DHT22 dht22(DHT22_PIN);
vec3_t *g_destCord = NULL_VEC3;

void setup(){
    g_destCord = vec3_init(0, 0, 0);

    //TODO: Calibrate sensors
    dht22.begin();
    bmp.begin();
    gps_init();
}

void loop(){
    vec3_t *can_position = gps_position();

    mpu.update();
    float north_dir = mpu.getMagHoz();

    float rotation = nav_angle(can_position, g_destCord, north_dir);
    
    // Programa de mover el servo empieza aqui :)
    
    // Mover servos

    // Recibir datos sensores
    vec3_t magnetometer = {mpu.getMagX(), mpu.getMagY(), mpu.getMagZ()};
    vec3_t gyroscope = {mpu.getGyroX(), mpu.getGyroY(), mpu.getGyroZ()};
    vec3_t accelerometer = {mpu.getAccX(), mpu.getAccY(), mpu.getAccZ()};

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


void moveServos(){
    float direction;
    calculate_direction(&direction, 0 /*latitud de donde quieres ir */, 0 /*longitud de donde quieres ir */,
    0 /*altura de donde quieres ir */, gps_position()->x, gps_position()->y, gps_position()->z);

    float mappedDirection = map(direction, 0, 2 * PI, -50, 50) ;

    servo.angleRight(50 - mappedDirection);
    servo.angleLeft(50 + mappedDirection);
}
