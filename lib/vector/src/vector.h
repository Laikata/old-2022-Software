#pragma once

#define NULL_VEC3 (vec3_t *)0

typedef struct VEC3_STRUCT {
    float x;
    float y;
    float z;
} vec3_t;

typedef struct {
    double x;
    double y;
    double z;
} dvec3_t;

vec3_t vec3_sub(vec3_t *a, vec3_t *b);
float vec3_deg(vec3_t *vec);
