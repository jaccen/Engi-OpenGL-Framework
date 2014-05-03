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

static std::vector<glm::vec3> triangle_vs = {glm::vec3(0.0, 0.5, 0.0), glm::vec3(-0.5, -0.5, 0.0), glm::vec3(0.5, -0.5, 0.0)};
static std::vector<unsigned> triangle_is = {0, 1, 2};

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
    3, 2, 6, 3, 4, 6, // Left face
    2, 1, 5, 2, 6, 5, // Bottom face
    3, 0, 4, 3, 7, 4  // Top face
};

static glm::vec4 e = glm::vec4(0, 5, -5, 1);
static glm::vec4 l = glm::vec4(.1, .1, .1, 0);

static std::vector<glm::vec3> vertexes;
static std::vector<glm::vec3> colors;

static float r = 0.0;
static float ep = 0.01f;

static bool bDraw = false;

#define v4(x)(glm::vec4(x, 0))

//////////////////////////////////////////////////////////////////////////

void Init(Graphics *_gfx, KeyboardServer *kbds, MouseServer *ms)
{
    // TODO: Initialization code here
    kbdc = new KeyboardClient(*kbds);
    mc = new MouseClient(*ms);
    gfx = _gfx;
    cam = new Camera();

    vertexes.reserve((2 * M_PI / ep) * (M_PI / ep));
    colors.reserve((2 * M_PI / ep) * (M_PI / ep));
    for (float phi = 0; phi < M_PI * 2; phi += ep)
    {
        for (float tht = -M_PI / 2; tht < M_PI / 2; tht += ep)
        {
            vertexes.push_back(MathFunctions::Garbage(1.0, phi, tht));
            colors.push_back(glm::vec3(phi / (M_PI * 2), (tht + (M_PI / 2)) / M_PI, 0.5));
        }
    }
}

static float sphere_rotation = 0.0;

void Loop()
{
    gfx->BeginFrame();

    float size = 10.0;
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex3f(-size, 0, 0); //x-axis
    glVertex3f(size, 0, 0); //x-axis
    glVertex3f(0, -size, 0); //y-axis
    glVertex3f(0, size, 0); //y-axis
    glVertex3f(0, 0, -size); //z-axis
    glVertex3f(0, 0, size); //z-axis
    glEnd();

    InputProcess();
    UpdateModel();

    if (bDraw) gfx->Demo();
    //gfx->DrawMesh(cube_vs, cube_is, Matrix::Identity());
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateX(r + M_PI / 2) * Matrix::Translate(2.0, 0.0, 0.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateZ(-r) * Matrix::Translate(0.0, 2.0, 0.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateX(-r) * Matrix::Translate(2.0, 2.0, 0.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateY(r) * Matrix::Translate(2.0, 0.0, 2.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateZ(-r - M_PI / 2) * Matrix::Translate(0.0, 0.0, 2.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateX(-r) * Matrix::Translate(-2.0, 2.0, 0.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateY(r) * Matrix::Translate(-2.0, 0.0, 2.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateZ(-r - M_PI / 2) * Matrix::Translate(-2.0, 2.0, -2.0));

    gfx->DrawSurface
        (
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::sin<float>(theta); },
        [](float phi, float theta) -> float { return glm::cos<float>(phi); },
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::cos<float>(theta); },
        Matrix::Scale(5) * Matrix::Translate(30,0,0) * Matrix::RotateY(r),
        20
        );

    gfx->DrawSurface // Draw sky
        (
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::sin<float>(theta); },
        [](float phi, float theta) -> float { return glm::cos<float>(phi); },
        [](float phi, float theta) -> float { return glm::sin<float>(phi) * glm::cos<float>(theta); },
        Matrix::Scale(50) * Matrix::RotateY(sphere_rotation),
        20
        );

    gfx->EndFrame();
}

void Exit()
{
    // TODO: Cleanup code here
    SafeDelete(kbdc);
    SafeDelete(mc);
    SafeDelete(cam);
}

static float move_speed = .1f;
static float angular_speed = 1.0f;
static float mouse_sensitivity = 1.0f;

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
    // Draw triangle
    if (kbdc->GetKeyState('1') == KEY_DOWN)
        bDraw = !bDraw;
    // Request to quit
    if (kbdc->IsPressed(27))
        bQuit = true;
    auto point = mc->GetDifference();
    cam->RotateX(mouse_sensitivity * point.y);
    cam->RotateY(mouse_sensitivity * point.x);
}

void UpdateModel()
{
    cam->Render();
    r += 0.01f;
    sphere_rotation += 0.001f;
}