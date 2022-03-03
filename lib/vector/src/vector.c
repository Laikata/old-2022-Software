#include <vector.h>

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
