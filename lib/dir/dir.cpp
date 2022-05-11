#include "dir.h"
#include <vector.h>

int calculate_direction(float *dir, float des_lat, float des_lon, float des_hei, float can_lat, float can_lon, float can_hei){
    vec3_t can = {can_lat, can_lon, can_hei};
    vec3_t des = {des_lat, des_lon, des_hei};
    vec3_t diff = vec3_sub(&des, &can);
    *dir = vec3_deg(&diff);
    // TODO: Check if NaN
    return 0;
}
