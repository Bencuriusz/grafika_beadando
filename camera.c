#include "camera.h"

#include <GL/glut.h>

#include <math.h>
#include <stdio.h>

void init_camera(Camera *camera)
{
    camera->angleX = 0.0;
    camera->angleY = 1.0;
    camera->direction.x = 0.0;
    camera->direction.y = 0.0;
    camera->direction.z = -1.0;
    camera->position.x = 0.0;
    camera->position.y = 20.0;
    camera->position.z = 5.0;
    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;
}

void set_view(const Camera *camera)
{
    gluLookAt(
        camera->position.x, camera->position.y, camera->position.z,
        camera->position.x + camera->direction.x, camera->position.y + camera->direction.y, camera->position.z + camera->direction.z,
        0.0, 1.0, 0.0);
}

void update_camera(Camera *camera, double time)
{
    double decrease = 25;

    camera->position.y += camera->speed.y * time;
    if (camera->position.y <= 20.0)
    {
        camera->position.y = 1.0;
        camera->speed.y = 0.0;
    }
    else
    {
        camera->speed.y -= decrease;
    }
}
