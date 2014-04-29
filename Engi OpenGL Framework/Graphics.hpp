//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  Graphics.h                                                                  //
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

#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#error Currently, only Windows is supported
#endif

#include "GL\glew.h"
#include "GL\GL.h"
#include "GL\GLU.h"

#include "glm\vec3.hpp"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

class Graphics
{
private:
    HWND hWindow = nullptr;             // Handle to window
    HDC  hDeviceContext = nullptr;      // Device context
    HGLRC hRenderingContext = nullptr;  // Rendering context

    unsigned width;
    unsigned height;
    float aspect;

public:
    // Constructor & destructor
    #ifdef _WIN32
    Graphics(HWND hwnd, unsigned width, unsigned height);
    #endif
    ~Graphics();

    void ChangeResolution(unsigned width, unsigned height);

    void BeginFrame();
    void EndFrame();
    void Demo();

    void sphere();
};