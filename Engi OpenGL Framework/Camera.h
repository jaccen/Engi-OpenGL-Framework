#pragma once

// OpemGL
#include "GL\glew.h"
#include "GL\GL.h"
#include "GL\GLU.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

// OpenGL Math
#include "glm\vec4.hpp"
#include "glm\vec3.hpp"
#include "glm\mat4x4.hpp"

class Camera
{
private:
    float scaler;
    glm::mat4 rot_eyeX;
    glm::mat4 rot_eyeY;
    glm::mat4 rot_eyeZ;
    glm::vec4 eye;
    glm::vec3 lookAt;
    glm::vec3 normal;
public:
    Camera(float ex, float ey, float ez,    // Eye vector
           float lx, float ly, float lz,    // Look at vector
           float nx, float ny, float nz);   // Normal vector
    ~Camera();

    void rotateEye(float x, float y, float z);
    void rotate_eyeX(float angle);          // Roll
    void rotate_eyeY(float angle);          // Pitch
    void rotate_eyeZ(float angle);          // Yaw

    void scale(float scalar);               // Move closer or away

    void Update();                          // Updates camera parameters
};

//rotx =
//[1,       0,       0,       0]
//[0,       cos(x),  sin(x),  0]
//[0,       -sin(x), cos(x),  0]
//[0,       0,       0,       1]
//
//roty =
//[cos(y),  0,       -sin(y), 0]
//[0,       1,       0,       0]
//[sin(y),  0,       cos(y),  0]
//[0,       0,       0,       1]
//
//rotz =
//[cos(z),  sin(z),  0,       0]
//[-sin(z), cos(z),  0,       0]
//[0,       0,       1,       0]
//[0,       0,       0,       1]