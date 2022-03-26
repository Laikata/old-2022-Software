#include <vector.h>
#include <gps.h>
#include <nav.h>
#include <servos.h>
#include "MPU9250.h"

MPU9250 mpu;
vec3_t *g_destCord = NULL_VEC3;

void setup(){
    g_destCord = vec3_init(0, 0, 0);

    gps_init();
}

void loop(){
    vec3_t *can_position = gps_position();

    float north_dir = imu_north();

    float rotation = nav_angle(can_position, g_destCord, north_dir);
    
    // Programa de mover el servo empieza aqui :)
    servos_init(0, 3, 5);
    
    
    
    
    
    
    // Mover servos

    // Recibir datos sensores
    vec3_t magnetometer = {mpu.getMagX(), mpu.getMagY(), mpu.getMagZ()};
    vec3_t gyroscope = {mpu.getGyroX(), mpu.getGyroY(), mpu.getGyroZ()};
    vec3_t accelerometer = {mpu.getAccX(), mpu.getAccY(), mpu.getAccZ()};

    //   1. Voltaje bateria
    //   2. Temperatura
    //   3. Presión

    // Mandar datos por radio + posición GPS

    // Liberar memoria alocada
}