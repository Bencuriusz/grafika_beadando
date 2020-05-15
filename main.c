#include <SOIL/SOIL.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#include "utils/include/load.h"
#include "utils/include/model.h"
#include "utils/include/draw.h"

#include "camera.h"

struct Model map;
float brigthness = 0.1f;
vec2 oldMousepos;
Camera camera;

typedef GLubyte Pixel[3]; /*represents red green blue*/

/**
 * Load texture from file and returns with the texture name.
 */
GLuint initialize_texture(char *filename)
{
    GLuint texture_name;
    glGenTextures(1, &texture_name);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int width;
    int height;

    unsigned char *image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 (Pixel *)image);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_TEXTURE_2D);

    return texture_name;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    set_view(&camera);

    draw_model(&map);

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, (GLdouble)width / (GLdouble)height, 0.5, 10000.0);
}

void mouseHandler(int button, int state, int x, int y)
{
    oldMousepos.x = x;
    oldMousepos.y = y;
}

void motionHandler(int x, int y)
{
    glutPostRedisplay();
}

void passivemotionHandler(int x, int y)
{
    float moveX = x - oldMousepos.x;
    float moveY = y - oldMousepos.y;

    float fraction = 0.007;

    camera.angleX += moveX * fraction;
    camera.angleY -= moveY * fraction;
    camera.angleY = camera.angleY > 0.99 ? 0.99 : camera.angleY < -0.99 ? -0.99 : camera.angleY;

    camera.direction.x = sin(camera.angleX) * (1 - fabs(camera.angleY));
    camera.direction.y = camera.angleY;
    camera.direction.z = -cos(camera.angleX) * (1 - fabs(camera.angleY));

    oldMousepos.x = x;
    oldMousepos.y = y;

    glutPostRedisplay();
}

void keyboardHandler(unsigned char key, int x, int y)
{
    float fraction = 0.5;

    switch (key)
    {
    case 'w':
        camera.position.x += camera.direction.x * fraction;
        camera.position.z += camera.direction.z * fraction;
        break;
    case 's':
        camera.position.x -= camera.direction.x * fraction;
        camera.position.z -= camera.direction.z * fraction;
        break;
    case 'a':
        camera.position.x += camera.direction.z * fraction;
        camera.position.z -= camera.direction.x * fraction;
        break;
    case 'd':
        camera.position.x -= camera.direction.z * fraction;
        camera.position.z += camera.direction.x * fraction;
        break;
    case '+':
        brigthness += 0.1f;
        printf("%.1f \n", brigthness);
        updateLights();
        break;
    case '-':
        if (brigthness <= 0.1f)
        {
            break;
        }
        brigthness -= 0.1f;
        printf("%.1f \n", brigthness);
        updateLights();
        break;
    default:
        printf("unknown key was pressed");
        break;
    }

    glutPostRedisplay();
}

void updateLights()
{
    float diffuse_light[] = {brigthness, brigthness, brigthness, brigthness};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
}

void specialHandler(int key, int x, int y)
{
    if (key == GLUT_KEY_F1)
    {
        printf("Help\n");
        printf("Movement: WASD\n");
        printf("Looking around: Mouse\n");
        printf("Lights: \"+\"/\"-\"\n");
    }
}

void idle()
{
    static int last_frame_time = 0;
    int current_time;
    double elapsed_time;

    current_time = glutGet(GLUT_ELAPSED_TIME);
    elapsed_time = (double)(current_time - last_frame_time) / 1000;
    last_frame_time = current_time;

    update_camera(&camera, elapsed_time);

    glutPostRedisplay();
}

void initialize()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.2, 0.2, 0.2, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        0.0, 0.0, -5000, // eye
        0.0, 0.0, 0.0,   // look at
        0.0, 1.0, 0.0    // up
    );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glClearDepth(1.0);

    initialize_texture("texture.png");

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float ambient_light[] = {0.2f, 0.2f, 0.2f, 1.0f};
    float diffuse_light[] = {0.1f, 0.1f, 0.1f, 0.1f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
}

/**
 * Main function
 */
int main(int argc, char *argv[])
{
    init_camera(&camera);

    load_model(&map, "map.obj");
    print_model_info(&map);

    print_bounding_box(&map);

    glutInit(&argc, argv);

    glutInitWindowSize(1600, 900);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    int window = glutCreateWindow("GLUT Window");
    glutSetWindow(window);

    initialize();

    //Callbacks binding
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    //Kattintás
    glutMouseFunc(mouseHandler);

    //Egér mozgatása
    glutMotionFunc(motionHandler);

    glutPassiveMotionFunc(passivemotionHandler);

    //Billentyűzet
    glutKeyboardFunc(keyboardHandler);

    //Spec
    glutSpecialFunc(specialHandler);

    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}
