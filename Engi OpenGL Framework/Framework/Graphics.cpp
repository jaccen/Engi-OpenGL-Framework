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
#include "Utility.h"

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
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Correction hint

    glDepthFunc(GL_LEQUAL);                             // Z buffer behavior
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
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(+0.0f, +0.5f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(+0.5f, -0.5f);
    glEnd();
}

void Graphics::DrawMesh(const std::vector<glm::vec3> &vertexes, const std::vector<unsigned> &indexes, const glm::mat4 &transform, unsigned textureID, GLuint primitive)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(primitive);
    int x = 0;
    for (auto itr = indexes.begin(); itr != indexes.end(); itr++)
    {
        glm::vec4 v = glm::vec4(vertexes.at(*itr), 1);
        glm::vec2 uv = glm::vec2(((glm::atan<float>(v.x, v.z) / M_PI) + 1.0f) * 0.5f, 0.5f - glm::asin<float>(v.y) / M_PI);
        v = v * transform;
        glTexCoord2f(uv.x, uv.y);
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();
}

void Graphics::DrawMesh(const std::vector<glm::vec3> &vertexes, const glm::mat4 &transform, unsigned textureID, GLuint primitive)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(primitive);
    glm::vec4 last;
    for (auto itr = vertexes.begin(); itr != vertexes.end(); itr++)
    {
        glm::vec2 uv = glm::vec2(((glm::atan<float>(itr->x, itr->z) / M_PI) + 1.0f) * 0.5f, 0.5f - glm::asin<float>(itr->y) / M_PI);
        glm::vec4 v = glm::vec4(*itr,1.0f) * transform;

        glTexCoord2f(uv.x, uv.y);
        if (v.z < 0 && last.x > 0 && v.x < 0)
            ;// glTexCoord2f(0, 0);

        glVertex3f(v.x, v.y, v.z);

        last = v;
    }
    glEnd();
}

void Graphics::DrawSurface(float(*para_x)(float, float), float(*para_y)(float, float), float(*para_z)(float, float),
                           glm::mat4 &transform, unsigned textureID, unsigned quality /*= 20*/)
{
    Graphics::DrawSurface(para_x, para_y, para_z, transform, textureID, glm::vec2(0.0f, 0.0f), glm::vec2(2.0f * M_PI, M_PI), quality);
}

void Graphics::DrawSurface(float(*para_x)(float, float), float(*para_y)(float, float), float(*para_z)(float, float), glm::mat4 &transform, unsigned textureID, glm::vec2 &start, glm::vec2 &end, unsigned quality /*= 20*/)
{
    // Untransformed vertexes, stored for UV mapping
    glm::vec4 v;
    // UV map
    glm::vec2 u;
    // Allow this vector to be translated
    v.w = 1.0f;
    // Step
    float dt = M_PI / quality;
    float dp = M_PI / quality;
    // Total number of iterations, used for UV mapping, stored as floats because they will be used for math
    float total_iterations = quality * quality * 2;
    float current = 0; // sin(x*pi/(2*k)), [0, k]
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    for (float theta = start.y; theta <= end.y; theta+=dt, current+=quality)
    {
        for (float phi = start.x; phi <= end.x; phi+=dp)
        {
            // Values of the next iteration
            float sum_p = phi + dp;
            float sum_t = theta + dt;
            // Vertex 01
            v.x = para_x(sum_p, sum_t);
            v.y = para_y(sum_p, sum_t);
            v.z = para_z(sum_p, sum_t);
            u.x = (current + quality) / total_iterations;
            u.y = (v.y + 1.0f) / 2.0f;
            v = v * transform;
            glTexCoord2f(u.x, u.y);
            glVertex3f(v.x, v.y, v.z);
            // Vertex 02
            v.x = para_x(phi, sum_t);
            v.y = para_y(phi, sum_t);
            v.z = para_z(phi, sum_t);
            u.x = (current + quality) / total_iterations;
            u.y = (v.y + 1.0f) / 2.0f;
            v = v * transform;
            glTexCoord2f(u.x, u.y);
            glVertex3f(v.x, v.y, v.z);
            // Vertex 03
            v.x = para_x(phi, theta);
            v.y = para_y(phi, theta);
            v.z = para_z(phi, theta);
            u.x = current / total_iterations;
            u.y = (v.y + 1.0f) / 2.0f;
            v = v * transform;
            glTexCoord2f(u.x, u.y);
            glVertex3f(v.x, v.y, v.z);
            // Vertex 04
            v.x = para_x(sum_p, theta);
            v.y = para_y(sum_p, theta);
            v.z = para_z(sum_p, theta);
            u.x = current / total_iterations;
            u.y = (v.y + 1.0f) / 2.0f;
            v = v * transform;
            glTexCoord2f(u.x, u.y);
            glVertex3f(v.x, v.y, v.z);

        }
    }
    glEnd();
}
// TODO: move to utilitary class
void Graphics::Axis(float length)
{
    glBegin(GL_LINES);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f); // X axis
    glVertex3f(length, 0.0f, 0.0f);
    glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
    glVertex3f(-length, 0.0f, 0.0f);
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f); // Y axis
    glVertex3f(0.0f, length, 0.0f);
    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, -length, 0.0f);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f); // Z axis
    glVertex3f(0.0f, 0.0f, length);
    glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -length);
    glColor3f(1, 1, 1);
    glEnd();
}
// TODO: move to utilitary class
void Graphics::IterateMesh(const std::vector<glm::vec3> &_mesh, unsigned count, unsigned textureID)
{
    auto midpoint = [](glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3) -> glm::vec3 {
        return glm::vec3((p1.x + p2.x + p3.x) / 3.0f, (p1.y + p2.y + p3.y) / 3.0f, (p1.z + p2.z + p3.z) / 3.0f);
    };

    std::vector<glm::vec3> *mesh = new std::vector<glm::vec3>(_mesh);

    while (count-- > 0)
    {
        std::vector<glm::vec3> *nmesh = new std::vector<glm::vec3>();
        for (auto i = 0; i < mesh->size(); i+=3)
        {
            glm::vec3 v1 = mesh->at(i);
            glm::vec3 v2 = mesh->at(i + 1);
            glm::vec3 v3 = mesh->at(i + 2);
            glm::vec3 m = midpoint(v1, v2, v3);
            //float length = glm::length(m);
            m = glm::normalize(m);// *(1.0f + length) * 0.5f;
            nmesh->push_back(v1);
            nmesh->push_back(m);
            nmesh->push_back(v2);
            nmesh->push_back(v1);
            nmesh->push_back(m);
            nmesh->push_back(v3);
            nmesh->push_back(v2);
            nmesh->push_back(m);
            nmesh->push_back(v3);
        }
        SafeDelete(mesh);
        mesh = std::move(nmesh);
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_TRIANGLES);
    int k = 0;
    for (auto itr = mesh->begin(); itr != mesh->end(); itr++)
    {
        if (!k)
        {
            glTexCoord2f(0.0f, 0.0f);
            k++;
        }
        else if (k % 2)
        {
            glTexCoord2f(1.0f, 0.0f);
            k++;
        }
        else
        {
            glTexCoord2f(.5f, 1.0f);
            k = 0;
        }
        glVertex3f(itr->x, itr->y, itr->z);
    }
    glEnd();

    SafeDelete(mesh);
}