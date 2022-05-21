#include "dir.h"
#include <vector.h>

#ifndef PI
#define PI 3.14159265
#endif

int calculate_direction(float *dir, float des_lon, float des_lat, float des_hei, float can_lon, float can_lat, float can_hei){
    vec3_t can = {can_lon, can_lat, can_hei};
    vec3_t des = {des_lon, des_lat, des_hei};
    vec3_t diff = vec3_sub(&des, &can); // changed order
    *dir = vec3_deg(&diff)*180/PI;
    
    // TODO: Check if NaN
    return 0;
}
