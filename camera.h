#ifndef CAMERA_H
#include "utils/include/utils.h"
#define CAMERA_H

/**
 * Camera, as a moving point with direction
 */
typedef struct Camera
{
    float angleX, angleY;
    vec3 direction;
    vec3 position;
    vec3 speed;
} Camera;

/**
 * Initialize the camera to the start position.
 */
void init_camera(Camera* camera);

/**
 * Update the position of the camera.
 */
void update_camera(Camera* camera, double time);

/**
 * Apply the camera settings to the view transformation.
 */
void set_view(const Camera* camera);

#endif /* CAMERA_H */

