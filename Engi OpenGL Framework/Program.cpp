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

using namespace std;
using namespace Keyboard;
using namespace Mouse;

static KeyboardClient *kbdc = nullptr;
static MouseClient *mc = nullptr;
static Graphics *gfx = nullptr;

static unsigned frames = 0;
extern Logger *gpLogger;

void Init(Graphics *_gfx, KeyboardServer *kbds, MouseServer *ms)
{
    // TODO: Initialization code here
    kbdc = new KeyboardClient(*kbds);
    mc = new MouseClient(*ms);
    gfx = _gfx;
}

void Loop()
{
    // TODO: Main loop code here
    gfx->BeginFrame();
    gfx->Demo();
    gfx->EndFrame();
}

void Exit()
{
    // TODO: Cleanup code here
    SafeDelete(kbdc);
    SafeDelete(mc);
}