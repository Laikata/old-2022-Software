#include <vector.h>
#include <gps.h>
#include <imu.h>
#include <nav.h>
#include <servos.h>

vec3_t *g_destCord = NULL_VEC3;

/*void setup(){
    g_destCord = vec3_init(0, 0, 0);

    gps_init();
}*/

void loop__(){
    vec3_t *can_position = gps_position();

    float north_dir = imu_north();

    float rotation = nav_angle(can_position, g_destCord, north_dir);
    
    // Programa de mover el servo empieza aqui :)
    servos_init(0, 3, 5);   /*Poner esto cuando antes de activar los servos. 
                            No lo pongais nada mas arrancar el programa porque 
                            o si no los servos se pasaran todo el rato consumiendo de la bateria*/
    
    //angleServoRight();
    
    
    
    
    // Mover servos

    // Recibir datos sensores

    //   1. Voltaje bateria
    //   2. Temperatura
    //   3. Presión

    // Mandar datos por radio + posición GPS

    // Liberar memoria alocada
}