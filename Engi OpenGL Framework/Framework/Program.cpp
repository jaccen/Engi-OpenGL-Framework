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

static float move_speed = 0.05f;
static float angular_speed = 1.0f;
static float mouse_sensitivity = 1.0f;
static float r = 0.0;

static unsigned textureID = 0;

void Init(Graphics *_gfx, KeyboardServer *kbds, MouseServer *ms)
{
    kbdc = new KeyboardClient(*kbds);
    mc = new MouseClient(*ms);
    gfx = _gfx;
    cam = new Camera();
    // TODO: Initialization code here

    textureID = SOIL_load_OGL_texture(
        ".\\Assets\\Images\\Earth.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
}

static float sphere_rotation = 0.0;

void Loop()
{
    gfx->BeginFrame();

    InputProcess();
    UpdateModel();
    // x(u, v) = a sqrt(u ^ 2 + 1) cos(v)
    // y(u, v) = a sqrt(u ^ 2 + 1) sin(v)
    // z(u, v) = c u

    gfx->DrawSurface // Draws a 'sky'
    (
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::sin<float>(theta); },
        [](float phi, float theta) -> float { return glm::cos<float>(phi); },
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::cos<float>(theta); },
        Matrix::RotateY(sphere_rotation),
        textureID, 200
    );

    //gfx->DrawSurface // Hyperboloid
    //    (
    //    [](float phi, float theta) -> float { return 1 * glm::sqrt<float>(phi*phi + 1) * glm::cos<float>(theta); },
    //    [](float phi, float theta) -> float { return 1 * glm::sqrt<float>(phi*phi + 1) * glm::sin<float>(theta); },
    //    [](float phi, float theta) -> float { return 1 * phi; },
    //    Matrix::RotateY(r) * Matrix::Scale(5) * Matrix::Translate(0, 40, 0) * Matrix::RotateX(r),
    //    textureID, 20
    //    );

    //gfx->DrawSurface // Ellipsoid
    //    (
    //    [](float phi, float theta) -> float { return 2 * glm::cos<float>(phi) * glm::sin<float>(theta); },
    //    [](float phi, float theta) -> float { return 3 * glm::sin<float>(phi) * glm::sin<float>(theta); },
    //    [](float phi, float theta) -> float { return 4 * glm::cos<float>(theta); },
    //    Matrix::RotateY(r) * Matrix::Scale(5) * Matrix::Translate(30, 0, 0) * Matrix::RotateY(r),
    //    textureID, 20
    //    );

    //gfx->DrawSurface // Draws a 'sky'
    //    (
    //    [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::sin<float>(theta); },
    //    [](float phi, float theta) -> float { return glm::cos<float>(phi); },
    //    [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::cos<float>(theta); },
    //    Matrix::Scale(100) * Matrix::RotateY(sphere_rotation),
    //    glm::vec3(1, 0, 0), 20
    //    );

    //gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateX(r + M_PI / 2) * Matrix::Translate(2.0, 0.0, 0.0));
    //gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateZ(-r) * Matrix::Translate(0.0, 2.0, 0.0));
    //gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateX(-r) * Matrix::Translate(2.0, 2.0, 0.0));
    //gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateY(r) * Matrix::Translate(2.0, 0.0, 2.0));
    //gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateZ(-r - M_PI / 2) * Matrix::Translate(0.0, 0.0, 2.0));
    //gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateX(-r) * Matrix::Translate(-2.0, 2.0, 0.0));
    //gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateY(r) * Matrix::Translate(-2.0, 0.0, 2.0));
    //gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateZ(-r - M_PI / 2) * Matrix::Translate(-2.0, 2.0, -2.0));

    gfx->EndFrame();
}

void Exit()
{
    // TODO: Cleanup code here
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
    sphere_rotation += 0.001f;
    cam->Render();
}