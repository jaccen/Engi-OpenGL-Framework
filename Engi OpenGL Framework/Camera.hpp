#pragma once

// OpemGL
#include "GL\glew.h"
#include "GL\GL.h"
#include "GL\GLU.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "Math.hpp"
#include "Utility.h"

class Camera
{
private:
    glm::vec3 vecPos;
    glm::vec3 vecRight;
    glm::vec3 vecUp;
    glm::vec3 vecViewDir;
    glm::vec3 vecDPos;
    float rotateX;
    float rotateY;
    float rotateZ;
public:
    explicit Camera();
    explicit Camera(float eye_x, float eye_y, float eye_z);
    ~Camera();

    void Move(glm::vec3 direction);
    void RotateX(float theta);
    void RotateY(float theta);
    void RotateZ(float theta);

    void Render();

    void Forward(float speed);
    void Strafe(float speed);
    void Upward(float speed);
};