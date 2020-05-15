#ifndef UTILS_H
#define UTILS_H

typedef struct vec2
{
    float x;
    float y;
} vec2;

/**
 * GLSL-like three dimensional vector
 */
typedef struct vec3
{
    float x;
    float y;
    float z;
} vec3;

/**
 * Calculates radian from degree.
 */
double degree_to_radian(double degree);

#endif /* UTILS_H */

