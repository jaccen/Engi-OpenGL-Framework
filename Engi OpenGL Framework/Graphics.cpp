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
#include "Math.hpp"

Graphics::Graphics(HWND hwnd, unsigned width, unsigned height) : width(width), height(height), aspect((float) width / (float) height)
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  // Size Of This Pixel Format Descriptor
        1,                              // Version Number
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,                  // Request An RGBA Format
        16,                             // Select Our Color Depth
        0, 0, 0, 0, 0, 0,               // Color Bits Ignored
        0,                              // No Alpha Buffer
        0,                              // Shift Bit Ignored
        0,                              // No Accumulation Buffer
        0, 0, 0, 0,                     // Accumulation Bits Ignored
        16,                             // 16Bit Z-Buffer (Depth Buffer)
        0,                              // No Stencil Buffer
        0,                              // No Auxiliary Buffer
        PFD_MAIN_PLANE,                 // Main Drawing Layer
        0,                              // Reserved
        0, 0, 0                         // Layer Masks Ignored
    };

    GLuint PixelFormat;
    if (!(hDeviceContext=GetDC(hwnd)))
    {
        logger.Log("[OpenGL]: Failed to get device context of window");
    }
    if (!(PixelFormat=ChoosePixelFormat(hDeviceContext, &pfd)))
    {
        logger.Log("[OpenGL]: Failed to specify pixel format");
    }
    if (!SetPixelFormat(hDeviceContext, PixelFormat, &pfd))
    {
        logger.Log("[OpenGL]: Failed to set pixel format");
    }
    if (!(hRenderingContext=wglCreateContext(hDeviceContext)))
    {
        logger.Log("[OpenGL]: Failed to create rendering context");
    }
    if (!wglMakeCurrent(hDeviceContext, hRenderingContext))
    {
        logger.Log("[OpenGL]: Failed to set current context");
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

unsigned Graphics::getWidth() const
{
    return width;
}

unsigned Graphics::getHeight() const
{
    return height;
}

void Graphics::BeginFrame()
{
    // Clears the buffer to black color
    glClearColor(0.2f, 0.1f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Sets projection matrix to render from vertexes
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, aspect, 0.01, 1000.0);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
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

void Graphics::DrawMesh(const std::vector<glm::vec3> &vertexes, const std::vector<unsigned> &indexes, const glm::mat4 &transform)
{
    glBegin(GL_TRIANGLES);
    int x = 0;
    for (auto itr = indexes.begin(); itr != indexes.end(); itr++)
    {
        if (x++ % 3 == 0) glColor3f(1.0f, 0.0f, 0.0f);
        else glColor3f(1.0f, 1.0f, 1.0f);
        glm::vec4 v = glm::vec4(vertexes.at(*itr), 1) * transform;
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();
}

void Graphics::DrawSurface(float(*para_x)(float, float), float(*para_y)(float, float), float(*para_z)(float, float), glm::mat4 &transform, unsigned quality)
{
    glm::vec4 eval;
    eval.w = 1.0f; // Allow this vector to be translated
    float dtheta = M_PI / quality;
    float dphi = M_PI / quality;

    glBegin(GL_QUADS);
    for (float theta = 0; theta <= 2.0*M_PI; theta+=dtheta)
    {
        for (float phi = 0; phi <= M_PI; phi+=dphi)
        {
            // Vertex 01
            eval.x = para_x(phi + dphi, theta + dtheta);
            eval.y = para_y(phi + dphi, theta + dtheta);
            eval.z = para_z(phi + dphi, theta + dtheta);

            glColor3f(0, 0, 1); // TODO: remove
            eval = eval * transform;
            glVertex3f(eval.x, eval.y, eval.z);

            // Vertex 02
            eval.x = para_x(phi, theta + dtheta);
            eval.y = para_y(phi, theta + dtheta);
            eval.z = para_z(phi, theta + dtheta);

            glColor3f(0, 0, 0); // TODO: remove
            eval = eval * transform;
            glVertex3f(eval.x, eval.y, eval.z);

            // Vertex 03
            eval.x = para_x(phi, theta);
            eval.y = para_y(phi, theta);
            eval.z = para_z(phi, theta);

            eval = eval * transform;
            glVertex3f(eval.x, eval.y, eval.z);

            // Vertex 04
            eval.x = para_x(phi + dphi, theta);
            eval.y = para_y(phi + dphi, theta);
            eval.z = para_z(phi + dphi, theta);

            eval = eval * transform;
            glVertex3f(eval.x, eval.y, eval.z);
        }
    }
    glEnd();
}