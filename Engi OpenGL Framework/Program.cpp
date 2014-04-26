//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1												//
//  MainLoop.cpp																//
//  																			//
//  This file is part of Engi OpenGL Framework									//
//  																			//
//  This program is free software: you can redistribute it and/or modify		//
//      it under the terms of the GNU General Public License as published by	//
//      the Free Software Foundation, either version 3 of the License, or		//
//      (at your option) any later version.										//
//  																			//
//      This program is distributed in the hope that it will be useful,			//
//      but WITHOUT ANY WARRANTY; without even the implied warranty of			//
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			//
//      GNU General Public License for more details.							//
//  																			//
//      You should have received a copy of the GNU General Public License		//
//      along with this program.  If not, see <http://www.gnu.org/licenses/>	//
//------------------------------------------------------------------------------//

#include "Logger.h"
#include "Keyboard.h"
#include "Utility.h"

using namespace std;
using namespace Keyboard;
static KeyboardClient *kbdc = nullptr;
static Logger *logger = nullptr;

void Init(KeyboardServer *kbds, Logger* _logger)
{
	// TODO: Initialization code here
	kbdc = new KeyboardClient(*kbds);
	logger = _logger;
}

void Loop()
{
	// TODO: Main loop code here
	switch (kbdc->GetState('A'))
	{
	case NOT_PRESSED:
		break;
	case KEY_DOWN:
		printf("'A' is KEY_DOWN!\n");
		break;
	case PRESSED:
		printf("'A' is PRESSED!\n");
		break;
	case KEY_UP:
		printf("'A' is KEY_UP!\n");
		break;
	}
}

void Exit()
{
	// TODO: Cleanup code here
	SafeDelete(kbdc);
}