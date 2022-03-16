#include <vector.h>

int calculate_direction(float *dir, float des_lat, float des_lon, float des_hei, float can_lat, float can_lon, float can_hei){
    vec3_t *can = vec3_init(can_lat, can_lon, can_hei);
    vec3_t *des = vec3_init(des_lat, des_lon, des_hei);
    vec3_t *diff = vec3_sub(des, can);
    diff->z = 0;
    *dir = vec3_deg(diff);
    // TODO: Check if NaN
    return 0;
}



int main(int argc, char **argv){
    float dir = 0;
    int status = calculate_direction(&dir, 0, 0, 0, 100, 100, 100);
    if(status != 0) return status;
    printf("Direction to head: %g\n", dir);
    return 0;
}
