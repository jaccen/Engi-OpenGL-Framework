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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Alpha blending behavior
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
    gluPerspective(65.0, aspect, 0.01, 15000.0);
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
    glDisable(GL_TEXTURE_2D);
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
    glDisable(GL_TEXTURE_2D);
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

void Graphics::DrawSurface(float(*para_x)(float, float), float(*para_y)(float, float), float(*para_z)(float, float),
                           glm::mat4 &transform, glm::vec3 &color, unsigned quality /*= 20*/)
{
    glm::vec4 eval;
    eval.w = 1.0f; // Allow this vector to be translated
    float dtheta = M_PI / quality;
    float dphi = M_PI / quality;

    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    for (float theta = 0; theta <= 2.0*M_PI; theta+=dtheta)
    {
        for (float phi = 0; phi <= M_PI; phi+=dphi)
        {
            // Vertex 01
            eval.x = para_x(phi + dphi, theta + dtheta);
            eval.y = para_y(phi + dphi, theta + dtheta);
            eval.z = para_z(phi + dphi, theta + dtheta);

            glColor3f(color.x, color.y, color.z);
            eval = eval * transform;
            glVertex3f(eval.x, eval.y, eval.z);

            // Vertex 02
            eval.x = para_x(phi, theta + dtheta);
            eval.y = para_y(phi, theta + dtheta);
            eval.z = para_z(phi, theta + dtheta);

            glColor3f(0, 0, 0);
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

void Graphics::DrawSurface(float(*para_x)(float, float), float(*para_y)(float, float), float(*para_z)(float, float),
                           glm::mat4 &transform, unsigned textureID, unsigned quality /*= 20*/)
{
    // Untransformed vertexes, stored for UV mapping
    glm::vec4 v1;
    glm::vec4 v2;
    glm::vec4 v3;
    glm::vec4 v4;
    // Transformed vertexes
    glm::vec4 r1;
    glm::vec4 r2;
    glm::vec4 r3;
    glm::vec4 r4;
    // Allow this vector to be translated
    v1.w = v2.w = v3.w = v4.w = 1.0f;
    // Step
    float dt = M_PI / quality;
    float dp = M_PI / quality;
    // Total number of iterations, used for UV mapping, stored as floats because they will be used for math
    float total_iterations = quality * quality * 2;
    float current = 0; // sin(x*pi/(2*k)), [0, k]
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    for (float theta = 0; theta <= 2.0*M_PI; theta+=dt, current+=quality)
    {
        for (float phi = 0; phi <= M_PI; phi+=dp)
        {
            // Values of the next iteration
            float sum_p = phi + dp;
            float sum_t = theta + dt;
            // Vertex 01
            v1.x = para_x(sum_p, sum_t);
            v1.y = para_y(sum_p, sum_t);
            v1.z = para_z(sum_p, sum_t);
            r1 = v1 * transform;
            // Vertex 02
            v2.x = para_x(phi, sum_t);
            v2.y = para_y(phi, sum_t);
            v2.z = para_z(phi, sum_t);
            r2 = v2 * transform;
            // Vertex 03
            v3.x = para_x(phi, theta);
            v3.y = para_y(phi, theta);
            v3.z = para_z(phi, theta);
            r3 = v3 * transform;
            // Vertex 04
            v4.x = para_x(sum_p, theta);
            v4.y = para_y(sum_p, theta);
            v4.z = para_z(sum_p, theta);
            r4 = v4 * transform;

            glTexCoord2f((current + quality) / total_iterations, (v1.y + 1.0f) / 2.0f);
            glVertex3f(r1.x, r1.y, r1.z);

            glTexCoord2f((current + quality) / total_iterations, (v2.y + 1.0f) / 2.0f);
            glVertex3f(r2.x, r2.y, r2.z);

            glTexCoord2f(current / total_iterations, (v3.y + 1.0f) / 2.0f);
            glVertex3f(r3.x, r3.y, r3.z);

            glTexCoord2f(current / total_iterations, (v4.y + 1.0f) / 2.0f);
            glVertex3f(r4.x, r4.y, r4.z);
        }
    }
    glEnd();
}