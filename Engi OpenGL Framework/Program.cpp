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

#include "Logger.h"
#include "Utility.h"

#include "Keyboard.h"
#include "Mouse.h"

#include "Graphics.h"
#include "Camera.h"

using namespace std;
using namespace Keyboard;
using namespace Mouse;

static KeyboardClient   *kbdc   = nullptr;
static MouseClient      *mc     = nullptr;
static Graphics         *gfx    = nullptr;

static Camera           *cam    = nullptr;

static unsigned frames = 0;
extern Logger *gpLogger;

static float x = 0.0f;
static float y = 0.0f;
static float z = 2.0f;

static float scale = 1.0f;

void Init(Graphics *_gfx, KeyboardServer *kbds, MouseServer *ms)
{
    // TODO: Initialization code here
    kbdc = new KeyboardClient(*kbds);
    mc = new MouseClient(*ms);
    gfx = _gfx;

    cam = new Camera(x, y, z, 0, 0, 0, 0, 0.1, 0);
}

void Loop()
{
    // TODO: Main loop code here
    gfx->BeginFrame();

    if (kbdc->GetKeyState('A') == KEY_HELDDOWN)
        x -= 0.05f;
    if (kbdc->GetKeyState('D') == KEY_HELDDOWN)
        x += 0.05f;
    if (kbdc->GetKeyState('W') == KEY_HELDDOWN)
        y -= 0.05f;
    if (kbdc->GetKeyState('S') == KEY_HELDDOWN)
        y += 0.05f;
    if (kbdc->GetKeyState('Q') == KEY_HELDDOWN)
        z -= 0.05f;
    if (kbdc->GetKeyState('E') == KEY_HELDDOWN)
        z += 0.05f;
    if (kbdc->GetKeyState('1') == KEY_HELDDOWN)
        scale -= 0.01f;
    if (kbdc->GetKeyState('2') == KEY_HELDDOWN)
        scale += 0.01f;

    //cam->rotateEye(x, y, z);
    cam->scale(scale);
    cam->Update();

    glBegin(GL_TRIANGLES);
    // Front face
    glColor3f(1,0,0);
    glVertex3f(-0.0f * cos(x), +0.5f* sin(y), -0.0f * sin(z));
    glColor3f(0,1,0);                 
    glVertex3f(-0.5f * cos(x), -0.5f* sin(y), +0.5f * sin(z));
    glColor3f(0,0,1);                 
    glVertex3f(+0.5f * cos(x), -0.5f * sin(y), +0.5f * sin(z));

    glEnd();

    gfx->EndFrame();
}

void Exit()
{
    // TODO: Cleanup code here
    SafeDelete(kbdc);
    SafeDelete(mc);
    SafeDelete(cam);
}