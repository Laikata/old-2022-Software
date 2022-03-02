#include <stdio.h>
#include <stdlib.h>
#include <math.h>



typedef struct VEC3_STRUCT {
    float x;
    float y;
    float z;
} vec3_t;



vec3_t *vec3_init(float x, float y, float z){
    vec3_t *vec = (vec3_t *)malloc(sizeof(vec3_t));
    vec->x = x;
    vec->y = y;
    vec->z = z;
    return vec;
}

void vec3_free(vec3_t *vec){
    free(vec);
}



vec3_t *vec3_sub(vec3_t *a, vec3_t *b){
    vec3_t *res = (vec3_t *)malloc(sizeof(vec3_t));
    res->x = a->x - b->x;
    res->y = a->y - b->y;
    res->z = a->z - b->z;
    return res;
}

float vec3_deg(vec3_t *vec){
    if(vec->x == 0){
        printf("ERROR: vector x component is 0\n");
        exit(1);
        return (float)0;
    }
    return atan(vec->y/vec->x);
}

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
