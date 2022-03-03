#pragma once

typedef struct VEC3_STRUCT {
    float x;
    float y;
    float z;
} vec3_t;

vec3_t *vec3_init(float x, float y, float z);
void vec3_free(vec3_t *vec);

vec3_t *vec3_sub(vec3_t *a, vec3_t *b);
float vec3_deg(vec3_t *vec);
