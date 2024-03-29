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

#include "Math.hpp"
#include "GlobalVariables.h"

#include "GL\glew.h"
#include "GL\GL.h"
#include "GL\GLU.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <vector>

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
    void DrawMesh(const std::vector<glm::vec3> &vertexes, const glm::mat4 &transform, unsigned textureID, GLuint primitive = GL_TRIANGLES);
    void DrawMesh(const std::vector<glm::vec3> &vertexes, const std::vector<unsigned> &indexes, const glm::mat4 &transform, unsigned textureID, GLuint primitive = GL_TRIANGLES);
    //////////////////////////////////////////////////////////////////////////
    // Debugging methods
    void Demo();
    void Axis(float length);
    void IterateMesh(const std::vector<glm::vec3> &mesh, unsigned count, unsigned textureID);
    // Draws a surface, to do that pass the functions to evaluate each coordinate of the vector separetely (parametrized format)
    void DrawSurface(float(*para_x)(float, float), float(*para_y)(float, float), float(*para_z)(float, float), glm::mat4 &transform, unsigned textureID, glm::vec2 &start, glm::vec2 &end, unsigned quality = 20);
    void DrawSurface(float(*para_x)(float, float), float(*para_y)(float, float), float(*para_z)(float, float), glm::mat4 &transform, unsigned textureID, unsigned quality = 20);
    //////////////////////////////////////////////////////////////////////////
};