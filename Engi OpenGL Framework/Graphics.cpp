//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  Graphics.cpp                                                                //
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

#include "Graphics.hpp"
#include "Logger.hpp"
#include "glm\glm.hpp"

extern Logger *gpLogger;

Graphics::Graphics(HWND hwnd, unsigned width, unsigned height) : width(width), height(height), aspect((float) width / (float) height)
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),	// Size Of This Pixel Format Descriptor
        1,								// Version Number
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,					// Request An RGBA Format
        16,								// Select Our Color Depth
        0, 0, 0, 0, 0, 0,				// Color Bits Ignored
        0,								// No Alpha Buffer
        0,								// Shift Bit Ignored
        0,								// No Accumulation Buffer
        0, 0, 0, 0,						// Accumulation Bits Ignored
        16,								// 16Bit Z-Buffer (Depth Buffer)
        0,								// No Stencil Buffer
        0,								// No Auxiliary Buffer
        PFD_MAIN_PLANE,					// Main Drawing Layer
        0,								// Reserved
        0, 0, 0							// Layer Masks Ignored
    };

    GLuint PixelFormat;
    if (!(hDeviceContext=GetDC(hwnd)))
    {
        gpLogger->Log("[OpenGL]: Failed to get device context of window");
    }
    if (!(PixelFormat=ChoosePixelFormat(hDeviceContext, &pfd)))
    {
        gpLogger->Log("[OpenGL]: Failed to specify pixel format");
    }
    if (!SetPixelFormat(hDeviceContext, PixelFormat, &pfd))
    {
        gpLogger->Log("[OpenGL]: Failed to set pixel format");
    }
    if (!(hRenderingContext=wglCreateContext(hDeviceContext)))
    {
        gpLogger->Log("[OpenGL]: Failed to create rendering context");
    }
    if (!wglMakeCurrent(hDeviceContext, hRenderingContext))
    {
        gpLogger->Log("[OpenGL]: Failed to set current context");
    }

    glShadeModel(GL_SMOOTH);                            // Smooth shaders
    glClearDepth(1.0f);                                 // Clear depth
    glEnable(GL_DEPTH_TEST);                            // Enable Z buffer
    glDepthFunc(GL_LEQUAL);                             // Z buffer behavior
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Correction hint
    glEnable(GL_BLEND);                                 // Enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Alpha blending behavior
    glEnable(GL_TEXTURE_2D);                            // Enable textures
    glViewport(0, 0, width, height);                    // Viewport settings
}

Graphics::~Graphics()
{
    if (hRenderingContext)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hRenderingContext);
        hRenderingContext=NULL;
    }

    if (hDeviceContext && !ReleaseDC(hWindow, hDeviceContext))
    {
        hDeviceContext = nullptr;
    }
}

void Graphics::ChangeResolution(unsigned width, unsigned height)
{
    this->width = width;
    this->height = height;

    aspect = (float) width / (float) height;
    glViewport(0, 0, width, height);
}

void Graphics::BeginFrame()
{
    // Clears the buffer to black color
    glClearColor(0.2f, 0.1f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Sets projection matrix to render from vertexes
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Target view
    gluPerspective(45, aspect, 0.1, 15);
}

void Graphics::EndFrame()
{
    // Swaps frontbuffer & backbuffer, this will effectively present the resulting image
    SwapBuffers(hDeviceContext);
}

void Graphics::Demo()
{
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(+0.0f, +0.5f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(+0.5f, -0.5f);
    glEnd();
}

void Graphics::sphere()
{
    auto sphr = [](float phi, float theta) -> glm::vec3
    {
        float cosv = glm::cos(theta);
        return glm::vec3(glm::sin(phi) * cosv, glm::cos(phi) * cosv, glm::sin(theta));
    };

    float step = 0.01f;
    float pi = 3.14159265359f;

    glBegin(GL_QUADS);
    for (float phi = 0.0f; phi <= 2 * pi; phi += step)
    {
        for (float theta = -pi / 2; theta <= pi / 2; theta += step)
        {
            auto vec = sphr(phi, theta);
            glColor3f((vec.x + 1.0f) / 2.0f, (vec.y + 1.0f) / 2.0f, (vec.z + 1.0f) / 2.0f);
            glVertex3f(vec.x - 0.01, vec.y - 0.01, vec.z);
            glVertex3f(vec.x + 0.01, vec.y - 0.01, vec.z);
            glVertex3f(vec.x + 0.01, vec.y + 0.01, vec.z);
            glVertex3f(vec.x - 0.01, vec.y + 0.01, vec.z);
        }
    }
    glEnd();
}