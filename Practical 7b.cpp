#include <Windows.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gl/glext.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <string>

#include <cstdio>

using namespace std;

GLuint texture = 0;
BITMAP BMP;
HBITMAP hBMP = NULL;

// string textures[3] = {"Brick.bmp", "Wood.bmp", "Metal.bmp"};
int textureNo = 0;

string projectRoot = "D:\\TARUC\\Sem 5\\BACS2173 Graphics Programming\\Practical7b\\";
char temp[100];

bool iceCream = false;

float speed = 0;
float perspectiveX = 0, perspectiveY = 0, perspectiveZ = -8;
float orthoX = 0, orthoY = 0.5, orthoZ = 0;
float bridgeDegree = 0, bridgeLineUp = 0;

void drawCone();
void drawTower();
void drawPencil();

void drawCloud(float size);
void drawCirle(float x1, float y1, float radius);

void drawCuboid(float topLeftX, float topLeftY, float topLeftZ, float topRightX, float topRightY, float topRightZ, float botLeftX, float botLeftY, float botLeftZ, float botRightX, float botRightY, float botRightZ);
void drawBridgeCuboid(float size, float widthScale, float thinness, float longness);
void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks);
void drawBridge();
void drawBridgeLine();
void drawMoon();
void drawClouds();

float v = 0.51, v1 = 0, v2 = 0, v3 = 0;

bool rotate = false;
bool isOrtho = false;
bool isLift = false;

double w = 1920;
double h = 1080;
double ar = w / h; // aspect ratio

// use dedicated GPU to run
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
extern "C"
{
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// void initTexture(int textureNo)
// {
//     strcpy(temp, projectRoot.c_str());
//     strcat(temp, textures[textureNo].c_str());

//     glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
//     HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), temp, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
//     GetObject(hBMP, sizeof(BMP), &BMP);
// }

////////////////////////////////////
string textureNames[50] = {};
int textureCount = 0;
GLuint *textures = new GLuint[10];

void initTexture(string textureName)
{
    bool found = false;
    int texturePointer = -1;
    for (int i = 0; i < textureCount + 1; i++)
    {
        if (textureNames[i] == textureName)
        {
            found = true;
            texturePointer = i;
            break;
        }
    }

    glEnable(GL_TEXTURE_2D);
    if (found)
    {
        // glActiveTexture(GL_TEXTURE0 + texturePointer); // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, textures[texturePointer]);
    }
    else
    {
        // glDisable(GL_TEXTURE_2D);
        // glDeleteTextures(1, &texture);

        strcpy(temp, projectRoot.c_str());
        strcat(temp, textureName.c_str());
        strcat(temp, ".bmp");

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), temp, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
        GetObject(hBMP, sizeof(BMP), &BMP);

        // glActiveTexture(GL_TEXTURE0 + textureCount); // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, textures[textureCount]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth,
                     BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

        DeleteObject(hBMP);
        textureNames[textureCount] = textureName;
        textureCount++;
    }
}
////////////////////////////////////////////
void removeTexture()
{
    // glDisable(GL_TEXTURE_2D);
    // glDeleteTextures(1, &texture);
}

void controls(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        if (key == GLFW_KEY_ESCAPE)
        {
            delete textures; // clean up
            printf("v: %f\tv1: %f\tv2: %f\tv3: %f\n", v, v1, v2, v3);
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        else if (key == GLFW_KEY_SPACE)
        {
            if (rotate)
                rotate = false;
            else
                rotate = true;
            //     textureNo += 1;
            //     if (textureNo % 3 == 0)
            //     {
            //         textureNo = 0;
            //     }
            //     strcpy(temp, projectRoot.c_str());
            //     strcat(temp, textures[textureNo].c_str());
            //     printf("%d\n", textureNo);
            //     printf("%s\n", temp);

            //     initTexture(textureNo);
        }
        // else if (key == GLFW_KEY_UP)
        // {
        //     if (rotate == 0)
        //         rotate = 1;
        //     else
        //         rotate = 0;
        // }
        // else if (key == GLFW_KEY_DOWN)
        // {
        //     if (rotate == 0)
        //         rotate = 2;
        //     else
        //         rotate = 0;
        // }
        else if (key == GLFW_KEY_1)
        {
            if (iceCream)
                iceCream = false;
            else
                iceCream = true;
        }
        else if (key == GLFW_KEY_F1)
        {
            if (isOrtho)
                isOrtho = false;
            else
                isOrtho = true;
        }
        else if (key == GLFW_KEY_F2)
        {
            if (isLift)
                isLift = false;
            else
                isLift = true;
        }
        else if (key == GLFW_KEY_KP_8)
        {
            perspectiveZ += 0.1;
            orthoZ += 10;
        }
        else if (key == GLFW_KEY_KP_2)
        {
            perspectiveZ -= 0.1;
            orthoZ -= 10;
        }
        else if (key == GLFW_KEY_T)
        {
            v += 1;
        }
        else if (key == GLFW_KEY_G)
        {
            v -= 1;
        }
        else if (key == GLFW_KEY_T)
        {
            v += 1;
        }
        else if (key == GLFW_KEY_G)
        {
            v -= 1;
        }
        else if (key == GLFW_KEY_Y)
        {
            v1 += 1;
        }
        else if (key == GLFW_KEY_H)
        {
            v1 -= 1;
        }
        else if (key == GLFW_KEY_U)
        {
            v2 += 1;
        }
        else if (key == GLFW_KEY_J)
        {
            v2 -= 1;
        }
        else if (key == GLFW_KEY_I)
        {
            v3 += 1;
        }
        else if (key == GLFW_KEY_K)
        {
            v3 -= 1;
        }
}

GLFWwindow *initWindow(const int resX, const int resY)
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // Open a window and create its OpenGL context
    // GLFWwindow *window = glfwCreateWindow(resX, resY, "Jaeger", glfwGetPrimaryMonitor(), NULL);
    GLFWwindow *window = glfwCreateWindow(resX, resY, "Practical 7", NULL, NULL);

    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, controls);

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return window;
}

void drawPyramid()
{
    glPushMatrix();
    // glColor3ub(255, 255, 255);
    glTranslatef(0, -0.5f, 0);
    //glRotatef(rotateDeg, 1, 1, 1);
    glBegin(GL_POLYGON);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, 0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, -1.0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, -1.0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, -1.0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, -1.0);
    }
    glEnd();

    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(-0.5, 0, -1);

        glTexCoord2f(1, 1);
        glVertex3f(-0.5, 0, 0);

        glTexCoord2f(1, 0.0f);
        glVertex3f(0.5, 0, 0);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.5, 0, -1);
    }
    glEnd();

    glPopMatrix();
}

void drawCube(float size)
{
    glBegin(GL_QUADS);
    // front
    // glColor3ub(30, 136, 229);
    glTexCoord2f(0.0f, 1);
    glVertex3f(0, 0, size);

    glTexCoord2f(1, 1);
    glVertex3f(size, 0, size);

    glTexCoord2f(1, 0.0f);
    glVertex3f(size, 0, 0);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, 0);

    // left
    // glColor3ub(223, 120, 239);
    glTexCoord2f(0.0f, 1);
    glVertex3f(0, size, size);
    glTexCoord2f(1, 1);
    glVertex3f(0, 0, size);
    glTexCoord2f(1, 0.0f);
    glVertex3f(0, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, size, 0);

    // bottom
    // glColor3ub(128, 226, 126);
    glTexCoord2f(0.0f, 1);
    glVertex3f(0, size, 0);
    glTexCoord2f(1, 1);
    glVertex3f(size, size, 0);
    glTexCoord2f(1, 0.0f);
    glVertex3f(size, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, 0);

    // right
    // glColor3ub(255, 255, 114);
    glTexCoord2f(0.0f, 1);
    glVertex3f(size, 0, size);
    glTexCoord2f(1, 1);
    glVertex3f(size, size, size);
    glTexCoord2f(1, 0.0f);
    glVertex3f(size, size, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(size, 0, 0);

    // behind
    // glColor3ub(255, 201, 71);
    glTexCoord2f(0.0f, 1);
    glVertex3f(size, size, size);
    glTexCoord2f(1, 1);
    glVertex3f(0, size, size);
    glTexCoord2f(1, 0.0f);
    glVertex3f(0, size, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(size, size, 0);

    // top
    // glColor3ub(115, 232, 255);
    glTexCoord2f(0.0f, 1);
    glVertex3f(0, size, size);
    glTexCoord2f(1, 1);
    glVertex3f(size, size, size);
    glTexCoord2f(1, 0.0f);
    glVertex3f(size, 0, size);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, size);

    glEnd();
}

// ice cream
void drawIceCreamCone()
{
    glPushMatrix();
    GLUquadricObj *cylinder = NULL;
    cylinder = gluNewQuadric();
    glRotatef(270, 1, 0, 0);

    initTexture("cone");
    glTranslatef(0, 0, -1);
    glColor3ub(186, 129, 83);
    glPointSize(2);
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluQuadricTexture(cylinder, GL_TRUE);
    gluCylinder(cylinder, 0.01, 0.4, 1.2, 20, 20);
    gluDeleteQuadric(cylinder);
    glPopMatrix();
    removeTexture();
    // glPushMatrix();
    // cylinder = gluNewQuadric();
    // glRotatef(270, 1, 0, 0);

    // glTranslatef(0, 0, -1);
    // glColor3ub(98, 66, 40);
    // gluQuadricDrawStyle(cylinder, GLU_LINE);
    // gluCylinder(cylinder, 0.011, 0.41, 1.2, 15, 15);
    // gluDeleteQuadric(cylinder);
    // glPopMatrix();
}

void drawBall()
{
    glPushMatrix();
    GLUquadricObj *sphere = NULL;
    sphere = gluNewQuadric();

    initTexture("vanilla");
    glTranslatef(0, 0.6, 0);
    glColor3ub(243, 229, 171);
    gluQuadricTexture(sphere, GL_TRUE);
    gluSphere(sphere, 0.31, 50, 50);
    gluDeleteQuadric(sphere);
    glPopMatrix();

    initTexture("chocolate");
    glPushMatrix();
    sphere = gluNewQuadric();
    glTranslatef(0, 0.3, 0);
    glColor3ub(128, 90, 70);
    gluQuadricTexture(sphere, GL_TRUE);
    gluSphere(sphere, 0.39, 50, 50);
    gluDeleteQuadric(sphere);
    glPopMatrix();

    removeTexture();
}

void drawTopping()
{
    glPushMatrix();
    GLUquadricObj *cylinder = NULL;
    cylinder = gluNewQuadric();
    glTranslatef(0.2, 0.8, 0.15);
    glColor3ub(61, 36, 28);
    glRotatef(110, 1, 1, 0);
    gluCylinder(cylinder, 0.01, 0.01, 0.1, 15, 15);
    gluDeleteQuadric(cylinder);
    glPopMatrix();

    glPushMatrix();
    cylinder = gluNewQuadric();
    glTranslatef(-0.2, 0.8, 0.155);
    glColor3ub(102, 45, 28);
    glRotatef(50, 0, 1, 0);
    gluCylinder(cylinder, 0.01, 0.01, 0.1, 15, 15);
    gluDeleteQuadric(cylinder);
    glPopMatrix();

    glPushMatrix();
    cylinder = gluNewQuadric();
    glRotatef(50, 0, 1, 0);
    glTranslatef(0.2, 0.85, 0);
    glColor3ub(219, 104, 133);
    gluCylinder(cylinder, 0.01, 0.01, 0.1, 15, 15);
    gluDeleteQuadric(cylinder);
    glPopMatrix();

    glPushMatrix();
    initTexture("biscuit");
    cylinder = gluNewQuadric();
    glColor3ub(234, 171, 102);
    glRotatef(250, 1, 0, 0);
    glTranslatef(0, 0, 0.5);
    gluQuadricTexture(cylinder, GL_TRUE);
    gluCylinder(cylinder, 0.05, 0.05, 0.9, 15, 15);
    gluDeleteQuadric(cylinder);
    removeTexture();
    glPopMatrix();
}

// bridge

void drawCone()
{
    initTexture("rooftop");
    glPushMatrix();
    GLUquadricObj *cylinder = NULL;
    cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluQuadricTexture(cylinder, GL_TRUE);
    gluCylinder(cylinder, 0.01, 0.3, 1, 20, 20);
    gluDeleteQuadric(cylinder);
    glPopMatrix();

    glPushMatrix();
    cylinder = gluNewQuadric();
    // glColor3ub(98, 66, 40);
    gluQuadricDrawStyle(cylinder, GLU_LINE);
    gluCylinder(cylinder, 0.011, 0.31, 1, 5, 5);
    gluDeleteQuadric(cylinder);
    glPopMatrix();
    removeTexture();
}

void drawTower()
{
    float towerWidth = 0.4;
    glPushMatrix();
    {
        glScalef(2.5, 1.05, 2.5);
        glTranslatef(0, -0.15, 0);
        drawPencil();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-towerWidth, 0, towerWidth);
        drawPencil();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(towerWidth, 0, towerWidth);
        drawPencil();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(towerWidth, 0, -towerWidth);
        drawPencil();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-towerWidth, 0, -towerWidth);
        drawPencil();
    }
    glPopMatrix();

    glPushMatrix();
    {
        float towerCubeWidth = 0.51;
        // glColor3ub(131, 83, 65);
        initTexture("main");
        drawCuboid(-towerCubeWidth, towerCubeWidth * 5.85, towerCubeWidth,
                   -towerCubeWidth, towerCubeWidth * 5.85, -towerCubeWidth,
                   -towerCubeWidth, 0, towerCubeWidth, -towerCubeWidth,
                   0, -towerCubeWidth);
        removeTexture();
    }
    glPopMatrix();
}

void drawPencil()
{
    glPushMatrix();
    {
        glScalef(0.5, 1, 0.5);

        glPushMatrix();
        {
            glRotatef(270, 1, 0, 0);
            glTranslatef(0, 0, -1);
            // glColor3ub(186, 129, 83);
            glPointSize(2);
            drawCone();
        }
        glPopMatrix();

        glPushMatrix();
        {
            double baseRadius = 0.3;
            double topRadius = 0.3;
            double cylinderHeight = 3;
            int slices = 30;
            int stacks = 30;
            glRotatef(-90, 1, 0, 0);
            initTexture("main");
            drawCylinder(baseRadius, topRadius, cylinderHeight, slices, stacks);
            removeTexture();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawCuboid(float topLeftX, float topLeftY, float topLeftZ,
                float topRightX, float topRightY, float topRightZ,
                float botLeftX, float botLeftY, float botLeftZ,
                float botRightX, float botRightY, float botRightZ)
{
    glPushMatrix();
    glBegin(GL_QUADS);
    //Face 1
    glTexCoord2f(0, 1);
    glVertex3f(topLeftX, topLeftY, topLeftZ);
    glTexCoord2f(0, 0);
    glVertex3f(botLeftX, botLeftY, botLeftZ);
    glTexCoord2f(1, 0);
    glVertex3f(botRightX, botRightY, botRightZ);
    glTexCoord2f(1, 1);
    glVertex3f(topRightX, topRightY, topRightZ);
    //Face 2
    glTexCoord2f(0, 1);
    glVertex3f(topLeftX, topLeftY, topLeftZ);
    glTexCoord2f(0, 0);
    glVertex3f(botLeftX, botLeftY, botLeftZ);
    glTexCoord2f(1, 0);
    glVertex3f(-botLeftX, botLeftY, botLeftZ);
    glTexCoord2f(1, 1);
    glVertex3f(-topLeftX, topLeftY, topLeftZ);
    //Face 3
    glTexCoord2f(0, 1);
    glVertex3f(-topLeftX, topLeftY, topLeftZ);
    glTexCoord2f(0, 0);
    glVertex3f(-botLeftX, botLeftY, botLeftZ);
    glTexCoord2f(1, 0);
    glVertex3f(-botRightX, botRightY, botRightZ);
    glTexCoord2f(1, 1);
    glVertex3f(-topRightX, topRightY, topRightZ);
    //Face 4
    glTexCoord2f(0, 1);
    glVertex3f(-topRightX, topRightY, topRightZ);
    glTexCoord2f(0, 0);
    glVertex3f(-botRightX, botRightY, botRightZ);
    glTexCoord2f(1, 0);
    glVertex3f(botRightX, botRightY, botRightZ);
    glTexCoord2f(1, 1);
    glVertex3f(topRightX, topRightY, topRightZ);
    //Face 5
    glTexCoord2f(0, 1);
    glVertex3f(topRightX, topRightY, topRightZ);
    glTexCoord2f(0, 0);
    glVertex3f(topLeftX, topLeftY, topLeftZ);
    glTexCoord2f(1, 0);
    glVertex3f(-topLeftX, topLeftY, topLeftZ);
    glTexCoord2f(1, 1);
    glVertex3f(-topRightX, topRightY, topRightZ);

    glEnd();
    glPopMatrix();
}

void drawBridgeCuboid(float size, float widthScale, float thinness, float longness)
{
    glBegin(GL_QUADS);
    // front
    glVertex3f(0, 0, size / widthScale);
    glVertex3f(size / thinness, 0, size / widthScale);
    glVertex3f(size / thinness, 0, 0);
    glVertex3f(0, 0, 0);

    // left
    glVertex3f(0, size * longness, size / widthScale);
    glVertex3f(0, 0, size / widthScale);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size * longness, 0);

    // bottom
    glVertex3f(0, size * longness, 0);
    glVertex3f(size / thinness, size * longness, 0);
    glVertex3f(size / thinness, 0, 0);
    glVertex3f(0, 0, 0);

    // right
    glVertex3f(size / thinness, 0, size / widthScale);
    glVertex3f(size / thinness, size * longness, size / widthScale);
    glVertex3f(size / thinness, size * longness, 0);
    glVertex3f(size / thinness, 0, 0);

    // behind
    glVertex3f(size / thinness, size * longness, size / widthScale);
    glVertex3f(0, size * longness, size / widthScale);
    glVertex3f(0, size * longness, 0);
    glVertex3f(size / thinness, size * longness, 0);

    // top
    glVertex3f(0, size * longness, size / widthScale);
    glVertex3f(size / thinness, size * longness, size / widthScale);
    glVertex3f(size / thinness, 0, size / widthScale);
    glVertex3f(0, 0, size / widthScale);

    glEnd();
}

void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks)
{
    GLUquadricObj *cylinder = NULL;
    cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);

    gluQuadricNormals(cylinder, GLU_SMOOTH);
    gluQuadricTexture(cylinder, GLU_TRUE);

    gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
    gluDeleteQuadric(cylinder);
}

void drawBridge()
{
    // top bridge
    float bridgeWidth = 1.287;
    float bridgeHeight = 0.15;
    float bridgeUp = -0.15;
    float bridgeDown = -1;

    initTexture("ice");
    glPushMatrix();
    {
        // glColor3ub(140, 255, 255);
        glTranslatef(0, bridgeUp, -0.21);
        glRotatef(90, 0, 0, 1);
        drawBridgeCuboid(1.5, 4, 30, 1);
    }
    glPopMatrix();

    glPushMatrix();
    {
        // glColor3ub(140, 255, 255);
        glTranslatef(0, bridgeUp + 0.05, -0.21);
        glRotatef(-90, 0, 0, 1);
        drawBridgeCuboid(1.5, 4, 30, 1);
    }
    glPopMatrix();

    initTexture("road");
    glPushMatrix();
    {
        glTranslatef(-1.2, bridgeDown + 0.05, -0.21);
        glRotatef(bridgeDegree, 0, 0, 1);
        // glColor3ub(183, 141, 69);
        glRotatef(270, 0, 0, 1);
        drawBridgeCuboid(1.5, 4, 30, 0.8);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(1.2, bridgeDown, -0.21);
        glRotatef(-bridgeDegree, 0, 0, 1);
        // glColor3ub(183, 141, 69);
        glRotatef(-270, 0, 0, 1);
        drawBridgeCuboid(1.5, 4, 30, 0.8);
    }
    glPopMatrix();

    glPushMatrix();
    {
        // glColor3ub(140, 255, 255);
        glTranslatef(-1.5, bridgeDown, -0.21);
        glRotatef(90, 0, 0, 1);
        drawBridgeCuboid(1.5, 4, 30, 1);
    }
    glPopMatrix();

    glPushMatrix();
    {
        // glColor3ub(140, 255, 255);
        glTranslatef(1.5, bridgeDown + 0.05, -0.21);
        glRotatef(-90, 0, 0, 1);
        drawBridgeCuboid(1.5, 4, 30, 1);
    }
    glPopMatrix();
    removeTexture();
}

void drawBridgeLine()
{
    glPushMatrix();
    {
        glLineWidth(10);
        glBegin(GL_LINES);
        {
            float x = 0.5 * sin(42);
            float y = 0.5 * cos(42);
            glVertex3f(x, y, 0); // First endpoint of line

            x = 0.5 * sin(42 + 3.142);
            y = 0.5 * cos(42 + 3.142);
            glVertex3f(x, y, 0); // Second endpoint of line
        }
        glEnd();
        glLineWidth(1);
    }
    glPopMatrix();
}

void drawMoon()
{
    float x = 0, y = 0, z = -15, GL_PI = 3.142, radius = 0.4;
    glPushMatrix();
    {
        // Call only once for all remaining points
        glBegin(GL_LINES);

        // All lines lie in the xy plane.
        for (float angle = 0.0f; angle <= GL_PI; angle += (GL_PI / 50.0f))
        {
            // Top half of the circle
            x = radius * sin(angle);
            y = radius * cos(angle);
            glVertex3f(x + 1, y + 2, z); // First endpoint of line

            // Bottom half of the circle
            x = radius * sin(angle + GL_PI);
            y = radius * cos(angle + GL_PI);
            glVertex3f(x + 1, y + 2, z); // Second endpoint of line
        }
        // Done drawing points
        glEnd();
    }
    glPopMatrix();
}

void drawCloud(float size)
{
    drawCirle(0, 0, size);
    drawCirle(0.2, 0, size);

    drawCirle(0.35, 0.1, size);

    drawCirle(0.2, 0.2, size);
    drawCirle(0, 0.2, size);

    drawCirle(-0.15, 0.1, size);
}

void drawClouds()
{
    glPushMatrix();
    {
        glTranslatef(-5, 1.5, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-7, 1.2, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-2, 1.5, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(4, 1.2, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(8, 1.5, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(13, 1.2, -15);
        drawCloud(0.15);
    }
    glPopMatrix();
}

void drawCirle(float x1, float y1, float radius)
{
    float x2 = x1, y2 = y1;

    glColor3ub(246, 246, 246);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1);
    for (float angle = 0; angle <= 360; angle += 10)
    {
        x2 = x1 + cos(angle) * radius;
        y2 = y1 + sin(angle) * radius;
        glVertex2f(x2, y2);
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 0);

    glRotatef(speed, 0, 1, 0);

    // if (rotate == 1)
    // {
    //     glRotatef(-1, 1, 1, 1);
    // }
    // else if (rotate == 2)
    // {
    //     glRotatef(1, 1, 1, 1);
    // }

    // glEnable(GL_TEXTURE_2D);
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
    //                 GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    //                 GL_LINEAR);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth,
    //              BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

    if (!iceCream)
    {
        // bridge
        glPushMatrix();
        {
            float towerDistance = 1.25;
            glPushMatrix();
            {
                glTranslatef(towerDistance, 0, 0);
                glScalef(0.6, 0.6, 0.6);
                glRotatef(180, 0, 0, 1);
                drawTower();
            }
            glPopMatrix();

            glPushMatrix();
            {
                glTranslatef(-towerDistance, 0, 0);
                glScalef(0.6, 0.6, 0.6);
                glRotatef(180, 0, 0, 1);
                drawTower();
            }
            glPopMatrix();

            drawBridge();

            glPushMatrix();
            {
                glTranslatef(-0.8, -1.2 + bridgeLineUp, 0);
                glRotatef(bridgeDegree, 0, 0, 1);
                // glColor3ub(146, 113, 55);
                drawBridgeLine();
            }
            glPopMatrix();

            glPushMatrix();
            {
                glRotatef(180, 0, 1, 0);
                glTranslatef(-0.8, -1.2 + bridgeLineUp, 0);
                glRotatef(bridgeDegree, 0, 0, 1);
                // glColor3ub(146, 113, 55);
                drawBridgeLine();
            }
            glPopMatrix();

            //  sky
            glPushMatrix();
            {
                initTexture("sky2");
                glRotatef(180, 1, 0, 0);
                GLUquadricObj *sphere = NULL;
                sphere = gluNewQuadric();

                gluQuadricTexture(sphere, GL_TRUE);
                gluSphere(sphere, 7.5, 50, 50);
                gluDeleteQuadric(sphere);

                removeTexture();
            }
            glPopMatrix();

            //  ground
            glPushMatrix();
            {
                initTexture("water");
                glTranslatef(2.0, -9.5, 0);
                float towerCubeWidth = 7.5;
                // glColor3ub(131, 83, 65);
                drawCuboid(-towerCubeWidth, towerCubeWidth, towerCubeWidth,
                           -towerCubeWidth, towerCubeWidth, -towerCubeWidth,
                           -towerCubeWidth, 0, towerCubeWidth, -towerCubeWidth,
                           0, -towerCubeWidth);
                removeTexture();
            }
            glPopMatrix();
            // glPushMatrix();
            // {
            //     glColor3ub(252, 212, 64);
            //     glTranslatef(0, 5, 0);
            //     drawMoon();
            // }
            // glPopMatrix();

            // drawClouds();
        }
        glPopMatrix();
    }
    else
    {
        drawBall();
        drawIceCreamCone();
        drawTopping();
    }

    glDisable(GL_TEXTURE_2D);
    DeleteObject(hBMP);
    glDeleteTextures(1, &texture);
}

int main(int argc, char **argv)
{
    GLFWwindow *window = initWindow(1920, 1080);

    if (NULL != window)
    {
        // handle resolution problem
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        double w = 1920;
        double h = 1080;
        double ar = w / h;
        glOrtho(-2 * ar, 2 * ar, -2, 2, -1, 10);

        glMatrixMode(GL_MODELVIEW);

        // initTexture(textureNo);
        glGenTextures(10, textures);

        while (!glfwWindowShouldClose(window))
        {
            // Scale to window size
            GLint windowWidth, windowHeight;
            glfwGetWindowSize(window, &windowWidth, &windowHeight);
            glViewport(0, 0, windowWidth, windowHeight);

            // setup the camera and
            // toggle different view mode
            if (isOrtho)
            {
                glLoadIdentity();
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(-2 * ar, 2 * ar, -2, 2, -2, 2);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glTranslatef(orthoX, orthoY, orthoZ);
            }
            else
            {
                glLoadIdentity();
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(45, ar, 1, 2000);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glTranslatef(perspectiveX, perspectiveY, perspectiveZ);
            }

            display();

            if (rotate)
                speed += 1;
            else
                speed = speed;

            // Update Screen
            glfwSwapBuffers(window);

            // Check for any input, or window movement
            glfwPollEvents();
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
