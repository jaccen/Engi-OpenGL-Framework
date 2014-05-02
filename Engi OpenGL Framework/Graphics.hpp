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

#include "Math.hpp"

#include <vector>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

class Graphics
{
private:
    //////////////////////////////////////////////////////////////////////////
    // Context variables
    HWND hWindow = nullptr;             // Handle to window
    HDC  hDeviceContext = nullptr;      // Device context
    HGLRC hRenderingContext = nullptr;  // Rendering context
    //////////////////////////////////////////////////////////////////////////
    // Dimensions of the client area of the window
    unsigned width;
    unsigned height;
    // Aspect ratio
    float aspect;
    //////////////////////////////////////////////////////////////////////////
public:
    //////////////////////////////////////////////////////////////////////////
    // Constructor & destructor
    #ifdef _WIN32
    Graphics(HWND hwnd, unsigned width, unsigned height);
    #endif
    ~Graphics();
    //////////////////////////////////////////////////////////////////////////
    // Display related methods
    void ChangeResolution(unsigned width, unsigned height);
    unsigned getWidth() const;
    unsigned getHeight() const;
    //////////////////////////////////////////////////////////////////////////
    // Frame control methods
    void BeginFrame();
    void EndFrame();
    void DrawMesh(const std::vector<glm::vec3> &vertexes, const std::vector<unsigned> &indexes, const glm::mat4 &transform);
    //////////////////////////////////////////////////////////////////////////
    // Draws some shapes for testing purposes
    void Demo();
    void sphere();
    //////////////////////////////////////////////////////////////////////////
};