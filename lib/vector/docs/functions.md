# Functions

## `vec3_init`
Initialize and allocate a tridimensional vector and return a pointer to it

Definition:
```c
vec3_t *vec3_init(float x, float y, float z);
```

## `vec3_free`
Free memory used by a tridimensional vector

Definition:
```c
void vec3_free(vec3_t *vec);
```

## `vec3_sub`
Subtract two tridimensional vectors and return the result

Definition:
```c
vec3_t *vec3_sub(vec3_t *a, vec3_t *b);
```

## `vec3_deg`
Get angle in radians of a tridimensional vector relative to *x* coordinates on the *x/y* plane and ignoring *z*

Definition:
```c
float vec3_deg(vec3_t *vec);
```



