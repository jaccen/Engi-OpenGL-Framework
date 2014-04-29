#include "Camera.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"

Camera::Camera(float ex, float ey, float ez,
               float lx, float ly, float lz,
               float nx, float ny, float nz)
{
    eye    = glm::vec4(ex, ey, ez, 1);                  // Can translate
    lookAt = glm::vec4(lx, ly, lz, 0);                  // Can not translate
    normal = glm::vec4(nx, ny, nz, 0);                  // Can not translate

    rot_eyeX = rot_eyeY = rot_eyeZ = glm::mat4();       // Identity
    scaler = 1.0f;
}

Camera::~Camera()
{}

void Camera::rotateEye(float x, float y, float z)
{
    float sinx = glm::sin(x);
    float cosx = glm::cos(x);
    float siny = glm::sin(y);
    float cosy = glm::cos(y);
    float sinz = glm::sin(z);
    float cosz = glm::cos(z);

    rot_eyeX = glm::mat4(1, 0, 0, 0,
                         0, cosx, sinx, 0,
                         0, -sinx, cosx, 0,
                         0, 0, 0, 1);

    rot_eyeY = glm::mat4(cosy, 0, -siny, 0,
                         0, 1, 0, 0,
                         siny, 0, cosy, 0,
                         0, 0, 0, 1);

    rot_eyeZ = glm::mat4(cosz, sinz, 0, 0,
                         -sinz, cosz, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);
}

void Camera::rotate_eyeX(float angle)
{
    float sin = glm::sin(angle);
    float cos = glm::cos(angle);

    rot_eyeX = glm::mat4(1, 0, 0, 0,
                         0, cos, sin, 0,
                         0, -sin, cos, 0,
                         0, 0, 0, 1);
}

void Camera::rotate_eyeY(float angle)
{
    float sin = glm::sin(angle);
    float cos = glm::cos(angle);

    rot_eyeY = glm::mat4(cos, 0, -sin, 0,
                         0, 1, 0, 0,
                         sin, 0, cos, 0,
                         0, 0, 0, 1);
}

void Camera::rotate_eyeZ(float angle)
{
    float sin = glm::sin(angle);
    float cos = glm::cos(angle);

    rot_eyeZ = glm::mat4(cos, sin, 0, 0,
                         -sin, cos, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);
}

void Camera::scale(float scalar)
{
    scaler = scalar > 0 ? scalar : 1.0f;
}

void Camera::Update()
{

    glm::vec4 lt = lookAt * (((rot_eyeX) * (rot_eyeY) * (rot_eyeZ)) * scaler);

    gluLookAt(eye.x, eye.y, eye.z,
              lt.x, lt.y, lt.z,
              normal.y, normal.z, normal.x);
    
    scaler = 1.0f;
}