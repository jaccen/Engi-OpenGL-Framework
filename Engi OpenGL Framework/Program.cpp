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
#include "Keyboard.h"
#include "Mouse.h"
#include "Utility.h"

using namespace std;
using namespace Keyboard;
using namespace Mouse;
static KeyboardClient *kbdc = nullptr;
static MouseClient *mc = nullptr;
static Logger *logger = nullptr;

static unsigned frames;

void Init(KeyboardServer *kbds, MouseServer *ms, Logger* _logger)
{
    // TODO: Initialization code here
    kbdc = new KeyboardClient(*kbds);
    mc = new MouseClient(*ms);
    logger = _logger;
}

void Loop()
{
    // TODO: Main loop code here
    printf("%u\n", frames++);
}

void Exit()
{
    // TODO: Cleanup code here
    SafeDelete(kbdc);
    SafeDelete(mc);
}