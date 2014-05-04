#include "Camera.hpp"

#define sq(x) (x*x)
#define zeroVector (glm::vec3())
#define len(vec) (glm::sqrt(sq(vec.x) + sq(vec.y) + sq(vec.z)))

Camera::Camera()
{
    vecDPos = vecPos = zeroVector;
    vecViewDir = glm::vec3(0.0f, 0.0f, -1.0f);
    vecRight = glm::vec3(1.0f, 0.0f, -1.0f);
    vecUp = glm::vec3(0.0f, 1.0f, 0.0f);

    rotateX = rotateY = rotateZ = 0.0f;
}

Camera::Camera(float eye_x, float eye_y, float eye_z)
{
    vecDPos = vecPos = zeroVector;
    vecViewDir = glm::vec3(0.0f, 0.0f, -1.0f);
    vecRight = glm::vec3(1.0f, 0.0f, -1.0f);
    vecUp = glm::vec3(0.0f, 1.0f, 0.0f);

    rotateX = rotateY = rotateZ = 0.0f;
}

Camera::~Camera()
{}

void Camera::Move(glm::vec3 direction)
{
    vecPos = vecPos + direction;
}

void Camera::RotateX(float theta)
{
    rotateX += theta;

    //Rotate viewdir around the right vector:
    vecViewDir = glm::normalize(vecViewDir * glm::cos<float>(theta*M_PI180)
                                + vecUp * glm::sin<float>(theta*M_PI180));

    // Up vector will be the cross product of our adjusted vectors
    vecUp = glm::cross(vecViewDir, vecRight) * (-1.0f);
}

void Camera::RotateY(float theta)
{
    rotateY += theta;

    //Rotate viewdir around the up vector:
    vecViewDir = glm::normalize(vecViewDir*glm::cos<float>(theta*M_PI180)
                                - vecRight*glm::sin<float>(theta*M_PI180));

    //now compute the new RightVector (by cross product)
    vecRight = glm::cross(vecViewDir, vecUp);
}

void Camera::RotateZ(float theta)
{
    rotateZ += theta;

    //Rotate viewdir around the right vector:
    vecRight = glm::normalize(vecRight * glm::cos<float>(theta*M_PI180)
                                    + vecUp * glm::sin<float>(theta*M_PI180));

    //now compute the new UpVector (by cross product)
    vecUp = glm::cross(vecViewDir, vecRight) * (-1.0f);
}

void Camera::Render()
{
    // The point at which the camera looks:
    glm::vec3 vecViewPoint = vecPos + vecViewDir;

    // As we know the up vector, we can easily use gluLookAt:
    gluLookAt(vecPos.x, vecPos.y, vecPos.z,
              vecViewPoint.x, vecViewPoint.y, vecViewPoint.z,
              vecUp.x, vecUp.y, vecUp.z);
}

void Camera::Forward(float speed)
{
    vecPos += (vecViewDir*-speed);
}

void Camera::Strafe(float speed)
{
    vecPos += (vecRight*speed);
}

void Camera::Upward(float speed)
{
    vecPos += (vecUp*speed);
}