// This example is for exemplification purposes and has no real usage

// A program to subtract two vectors

// Include the library
#include <vector.h>

int main(){
    // Create a (1, 2, 3) tridimensional vector
    // Note that vec3_init returns a pointer to a vector
    vec3_t *vec = vec3_init(1, 2, 3);

    // Another one
    vec3_t *vec2 = vec3_init(3, 2, 1);

    // Subtract both vectors and save the result as a new vector
    vec3_t *result = vec3_sub(vec, vec2);

    // It's always nice to free allocated memory
    vec3_free(vec);
    vec3_free(vec2);
    vec3_free(result);

    return 0;
}
