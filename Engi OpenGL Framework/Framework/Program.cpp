//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  Program.cpp                                                                 //
//                                                                              //
//  This file is part of Engi OpenGL Framework                                  //
//                                                                              //
//  This program is free software: you can redistribute it and/or modify        //
//      it under the terms of the GNU General Public License as published by    //
//      the Free Software Foundation, either version 3 of the License, or       //
//      (at your option) any later version.                                     //
//                                                                              //
//      This program is distributed in the hope that it will be useful,         //
//      but WITHOUT ANY WARRANTY; without even the implied warranty of          //
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           //
//      GNU General Public License for more details.                            //
//                                                                              //
//      You should have received a copy of the GNU General Public License       //
//      along with this program.  If not, see <http://www.gnu.org/licenses/>    //
//------------------------------------------------------------------------------//

#include "Logger.hpp"
#include "Utility.h"

#include "Keyboard.hpp"
#include "Mouse.hpp"

#include "Graphics.hpp"
#include "Camera.hpp"

#include "Math.hpp"
#include "GlobalVariables.h"

#include "SOIL.h"

void InputProcess();
void UpdateModel();

using namespace std;
using namespace Keyboard;
using namespace Mouse;

static KeyboardClient   *kbdc   = nullptr;
static MouseClient      *mc     = nullptr;
static Graphics         *gfx    = nullptr;
static Camera           *cam    = nullptr;

static unsigned frames = 0;

static std::vector<glm::vec3> cube_vs =
{
    glm::vec3(+0.5, +0.5, +0.5),
    glm::vec3(+0.5, -0.5, +0.5),
    glm::vec3(-0.5, -0.5, +0.5),
    glm::vec3(-0.5, +0.5, +0.5),
    glm::vec3(+0.5, +0.5, -0.5),
    glm::vec3(+0.5, -0.5, -0.5),
    glm::vec3(-0.5, -0.5, -0.5),
    glm::vec3(-0.5, +0.5, -0.5)
};

static std::vector<unsigned> cube_is =
{
    0, 1, 2, 0, 3, 2, // Front face
    4, 5, 6, 4, 7, 6, // Back face
    0, 1, 5, 0, 4, 5, // Right face
    3, 2, 6, 3, 7, 6, // Left face
    2, 1, 5, 2, 6, 5, // Bottom face
    3, 0, 4, 3, 7, 4  // Top face
};
// Camera controls
static float move_speed = 1.0f;
static float angular_speed = 1.0f;
static float mouse_sensitivity = 1.0f;
// Movement controls
static float r = 0.0;
static float sphere_rotation = 0.0;
// Textures
static unsigned spaceID = 0;
static unsigned sunID = 0;
static unsigned mercuryID = 0;
static unsigned venusID = 0;
static unsigned earthID = 0;
static unsigned marsID = 0;
//////////////////////////////////////////////////////////////////////////
// Planets
//////////////////////////////////////////////////////////////////////////
// Sun -- not really a planet.. haha
static float sun_speed = 2.5f;
static float sun_distance = 5.0f;
// Mercury
static float mercury_speed = 5.0f;
static float mercury_distance = 100.0f;
// Venus
static float venus_speed = 10.0f;
static float venus_distance = 200.0f;
// Earth
static float earth_speed = 3.0f;
static float earth_distance = 300.0f;
// Mars
static float mars_speed = 2.0f;
static float mars_distance = 400.0f;

void Init(Graphics *_gfx, KeyboardServer *kbds, MouseServer *ms)
{
    kbdc = new KeyboardClient(*kbds);
    mc = new MouseClient(*ms);
    gfx = _gfx;
    cam = new Camera();
    // TODO: Initialization code here

    spaceID   = glLoadTexture(".\\Assets\\Images\\Space.jpg");
    sunID     = glLoadTexture(".\\Assets\\Images\\Sun.jpg");
    mercuryID = glLoadTexture(".\\Assets\\Images\\Mercury.jpg");
    venusID   = glLoadTexture(".\\Assets\\Images\\Venus.jpg");
    earthID   = glLoadTexture(".\\Assets\\Images\\Earth.jpg");
    marsID    = glLoadTexture(".\\Assets\\Images\\Mars.jpg");
}

void Loop()
{
    gfx->BeginFrame();

    InputProcess();
    UpdateModel();
    // x(u, v) = a sqrt(u ^ 2 + 1) cos(v)
    // y(u, v) = a sqrt(u ^ 2 + 1) sin(v)
    // z(u, v) = c u

    gfx->DrawSurface // Draws the space
        (
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::sin<float>(theta); },
        [](float phi, float theta) -> float { return glm::cos<float>(phi); },
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::cos<float>(theta); },
        Matrix::Scale(5000),
        spaceID, 20
        );

    gfx->DrawSurface // Draws the sun
        (
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::sin<float>(theta); },
        [](float phi, float theta) -> float { return glm::cos<float>(phi); },
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::cos<float>(theta); },
        Matrix::RotateY(sphere_rotation * 5.0f) * Matrix::Scale(40) * Matrix::Translate(sun_distance * glm::cos<float>(sun_speed * sphere_rotation), 0, sun_distance / 1.25f * glm::sin<float>(sun_speed * sphere_rotation)),
        sunID, 20
        );

    gfx->DrawSurface // Draws mercury
        (
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::sin<float>(theta); },
        [](float phi, float theta) -> float { return glm::cos<float>(phi); },
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::cos<float>(theta); },
        Matrix::RotateY(sphere_rotation * 30.0f) * Matrix::Scale(8.0f) * Matrix::Translate(mercury_distance * glm::cos<float>(mercury_speed * sphere_rotation), 0, mercury_distance / 1.25f * glm::sin<float>(mercury_speed * sphere_rotation)),
        mercuryID, 20
        );

    gfx->DrawSurface // Draws venus
        (
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::sin<float>(theta); },
        [](float phi, float theta) -> float { return glm::cos<float>(phi); },
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::cos<float>(theta); },
        Matrix::RotateY(sphere_rotation * 80.0f) * Matrix::Scale(12.0f) * Matrix::Translate(venus_distance / 1.25f * glm::cos<float>(venus_speed * sphere_rotation + M_PI / 2.0f), 0, venus_distance * glm::sin<float>(venus_speed * sphere_rotation + M_PI / 2.0f)),
        venusID, 20
        );

    gfx->DrawSurface // Draws earth
        (
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::sin<float>(theta); },
        [](float phi, float theta) -> float { return glm::cos<float>(phi); },
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::cos<float>(theta); },
        Matrix::RotateY(sphere_rotation * 60.0f) * Matrix::Scale(30.0f) * Matrix::Translate(earth_distance * glm::cos<float>(earth_speed * sphere_rotation + M_PI), 0, earth_distance / 1.25f * glm::sin<float>(earth_speed * sphere_rotation + M_PI)),
        earthID, 20
        );

    gfx->DrawSurface // Draws mars
        (
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::sin<float>(theta); },
        [](float phi, float theta) -> float { return glm::cos<float>(phi); },
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::cos<float>(theta); },
        Matrix::RotateY(sphere_rotation * 50.0f) * Matrix::Scale(24.0f) * Matrix::Translate(mars_distance / 1.25f * glm::cos<float>(mars_speed * sphere_rotation - M_PI / 4.0f), 0, mars_distance * glm::sin<float>(mars_speed * sphere_rotation - M_PI / 4.0f)),
        marsID, 20
        );

    gfx->EndFrame();
}

void Exit()
{
    // TODO: Cleanup code here
    glDeleteTextures(1, &spaceID);
    glDeleteTextures(1, &sunID);
    glDeleteTextures(1, &mercuryID);
    glDeleteTextures(1, &venusID);
    glDeleteTextures(1, &earthID);
    glDeleteTextures(1, &marsID);
    SafeDelete(kbdc);
    SafeDelete(mc);
    SafeDelete(cam);
}

void InputProcess()
{
    ///////////////////////
    // Keyboard commands //
    ///////////////////////
    // Movement controls
    if (kbdc->IsPressed('W'))
        cam->Forward(-move_speed);
    if (kbdc->IsPressed('S'))
        cam->Forward(move_speed);;
    if (kbdc->IsPressed('A'))
        cam->Strafe(-move_speed);
    if (kbdc->IsPressed('D'))
        cam->Strafe(move_speed);
    // Roll camera
    if (kbdc->IsPressed('Q'))
        cam->RotateZ(angular_speed);
    if (kbdc->IsPressed('E'))
        cam->RotateZ(-angular_speed);
    // Acceleration
    if (kbdc->IsPressed('1'))
        move_speed += .1f;
    if (kbdc->IsPressed('2'))
        move_speed -= .1f;

    // Request to quit
    if (kbdc->IsPressed(27))
        bQuit = true;
    auto point = mc->GetDifference();
    cam->RotateX(mouse_sensitivity * point.y);
    cam->RotateY(mouse_sensitivity * point.x);
}

void UpdateModel()
{
    r += 0.01f;
    // 1 Hour to orbit around the sun
    sphere_rotation += (M_PI * 2) / (FPS * 360);
    cam->Render();
}