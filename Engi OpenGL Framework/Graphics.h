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

public:
    // Constructor & destructor
    #ifdef _WIN32
    Graphics(HWND hwnd, unsigned width, unsigned height);
    #endif
    ~Graphics();

    void BeginFrame();
    void EndFrame();
    void Demo();
};