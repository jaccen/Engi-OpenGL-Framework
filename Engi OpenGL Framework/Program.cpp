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
extern Logger *gpLogger;

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

    //gfx->Demo();
    glColor3f(0.2, 0.1, 0.4);
    //gfx->DrawMesh(cube_vs, cube_is, Matrix::Identity());
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateX(r + M_PI / 2) * Matrix::Translate(2.0, 0.0, 0.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateZ(-r) * Matrix::Translate(0.0, 2.0, 0.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateX(-r) * Matrix::Translate(2.0, 2.0, 0.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateY(r) * Matrix::Translate(2.0, 0.0, 2.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateZ(-r - M_PI / 2) * Matrix::Translate(0.0, 0.0, 2.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateX(-r) * Matrix::Translate(-2.0, 2.0, 0.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateY(r) * Matrix::Translate(-2.0, 0.0, 2.0));
    gfx->DrawMesh(cube_vs, cube_is, Matrix::RotateZ(-r - M_PI / 2) * Matrix::Translate(-2.0, 2.0, -2.0));

    { // Draw 'sky'
        float radius = 50;
        float z1, x1, y1, z2, x2, y2, z3, x3, y3, z4, x4, y4;

        float dtheta = M_PI / 20;
        float dphi = M_PI / 20;

        glBegin(GL_QUADS);
        for (float theta = 0; theta <= 2.0*M_PI; theta+=dtheta)
        {
            for (float phi = 0; phi <= M_PI; phi+=dphi)
            {
                z1 = radius * sin(phi + dphi) * cos(theta + dtheta);
                x1 = radius * sin(phi + dphi) * sin(theta + dtheta);
                y1 = radius * cos(phi + dphi);

                z2 = radius * sin(phi) * cos(theta + dtheta);
                x2 = radius * sin(phi) * sin(theta + dtheta);
                y2 = radius * cos(phi);

                z3 = radius * sin(phi) * cos(theta);
                x3 = radius * sin(phi) * sin(theta);
                y3 = radius * cos(phi);

                z4 = radius * sin(phi + dphi) * cos(theta);
                x4 = radius * sin(phi + dphi) * sin(theta);
                y4 = radius * cos(phi + dphi);

                glColor3f(z4 / radius*2, y4 / radius, x4 / radius);
                glVertex3f(x4, y4, z4);
                glColor3f(z1 / radius, y1 / radius, x1 / radius);
                glVertex3f(x3, y3, z3);
                glColor3f(z2 / radius, y2 / radius, x2 / radius);
                glVertex3f(x2, y2, z2);
                glColor3f(x3 / radius, z3 / radius, y3 / radius);
                glVertex3f(x1, y1, z1);
            }
        }
        glEnd();
    }

    gfx->EndFrame();
}

void Exit()
{
    // TODO: Cleanup code here
    SafeDelete(kbdc);
    SafeDelete(mc);
    SafeDelete(cam);
}

static float move_speed = 0.2f;
static float angular_speed = 1.0f;
static float mouse_sensitivity = 1.0f;

void InputProcess()
{
    // Keyboard commands
    if (kbdc->IsPressed('W'))
        cam->Forward(-move_speed);
    if (kbdc->IsPressed('S'))
        cam->Forward(move_speed);;
    if (kbdc->IsPressed('A'))
        cam->Strafe(-move_speed);
    if (kbdc->IsPressed('D'))
        cam->Strafe(move_speed);
    if (kbdc->IsPressed('Q'))
        cam->RotateZ(angular_speed);
    if (kbdc->IsPressed('E'))
        cam->RotateZ(-angular_speed);

    auto point = mc->GetDifference();
    cam->RotateX(mouse_sensitivity * point.y);
    cam->RotateY(mouse_sensitivity * point.x);
}

void UpdateModel()
{
    cam->Render();
    r += 0.01;
}