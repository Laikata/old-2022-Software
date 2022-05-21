#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector.h>

vec3_t vec3_sub(vec3_t *a, vec3_t *b){
    vec3_t res = {0,0,0};
    res.x = a->x - b->x;
    res.y = a->y - b->y;
    res.z = a->z - b->z;
    return res;
}

float vec3_deg(vec3_t *vec){

    return atan2(vec->y, vec->x);
}
