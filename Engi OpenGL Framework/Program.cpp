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
#include "glm\detail\func_trigonometric.hpp"

using namespace std;
using namespace Keyboard;
using namespace Mouse;

static KeyboardClient   *kbdc   = nullptr;
static MouseClient      *mc     = nullptr;
static Graphics         *gfx    = nullptr;

static Camera           *cam    = nullptr;

static unsigned frames = 0;
extern Logger *gpLogger;

static float scale = 1.0f;

glm::vec3 sphere(float radius, float phi, float theta) {
    float cosv = glm::cos(theta);
    return glm::vec3(glm::sin(phi) * cosv, glm::cos(phi) * cosv, glm::sin(theta)) * radius;
}

static float phi   = 0.0f;
static float theta = 0.0f;
static float zoom  = 5.0f;

void Init(Graphics *_gfx, KeyboardServer *kbds, MouseServer *ms)
{
    // TODO: Initialization code here
    kbdc = new KeyboardClient(*kbds);
    mc = new MouseClient(*ms);
    gfx = _gfx;

    //cam = new Camera(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void Loop()
{
    float epsilon = 0.05f;
    // TODO: Main loop code here
    gfx->BeginFrame();
    if (kbdc->GetKeyState('A') == KEY_HELDDOWN)
        phi += epsilon;
    if (kbdc->GetKeyState('D') == KEY_HELDDOWN)
        phi -= epsilon;
    if (kbdc->GetKeyState('W') == KEY_HELDDOWN)
        theta += epsilon;
    if (kbdc->GetKeyState('S') == KEY_HELDDOWN)
        theta -= epsilon;
    if (kbdc->GetKeyState('Q') == KEY_HELDDOWN)
        zoom += epsilon;
    if (kbdc->GetKeyState('E') == KEY_HELDDOWN)
        zoom -= epsilon;
    auto blauto = sphere(zoom, phi, theta);
    gluLookAt(blauto.x, blauto.y, blauto.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    //cam->Update();
    gfx->sphere();
    gfx->Demo();
    gfx->EndFrame();
}

void Exit()
{
    // TODO: Cleanup code here
    SafeDelete(kbdc);
    SafeDelete(mc);
    SafeDelete(cam);
}