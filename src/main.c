#include <vector.h>
#include <gps.h>
#include <imu.h>
#include <nav.h>
#include <servos.h>


vec3_t *g_destCord = NULL_VEC3;

void setup(){
    g_destCord = vec3_init();

    gps_init();

}

void loop(){
    vec3_t *can_position = gps_position();

    float north_dir = imu_north();

    float rotation = nav_angle(can_position, g_destCord, north_dir);


    // Mover servos

    // Recibir datos sensores

    //   1. Voltaje bateria
    //   2. Temperatura
    //   3. Presión

    // Mandar datos por radio + posición GPS

    // Liberar memoria alocada
}