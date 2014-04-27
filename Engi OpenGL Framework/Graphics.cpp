#include "Graphics.h"
#include "Logger.h"

extern Logger *gpLogger;

Graphics::Graphics(HWND hwnd, unsigned width, unsigned height) : width(width), height(height)
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

void Graphics::BeginFrame()
{
    // Clears the buffer to black color
    glClearColor(0.2f, 0.2f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Sets projection matrix to render from vertexes
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Target view
    gluPerspective(45, 4.0f / 3.0f, 0.1, 5);
    gluLookAt(0,0,2,0,0,0,0,1,0);
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