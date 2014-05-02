//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  WindowsMain.cpp                                                             //
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

//#define CONSOLE_MODE
#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <thread>

#ifdef _DEBUG
#include <consoleapi.h>
#include <fcntl.h>
#include <io.h>
#endif

// Utilities
#include "Timer.hpp"
#include "Logger.hpp"
#include "Utility.h"
// Input
#include "Keyboard.hpp"
#include "Mouse.hpp"
// Graphics
#include "Graphics.hpp"

using namespace Keyboard;
using namespace Mouse;

// Framework methods
extern void Init(Graphics*, KeyboardServer*, MouseServer*);
extern void Loop();
extern void Exit();

// TODO: configuration file
#define WIDTH 640                                               // Client area width in pixels
#define HEIGHT 480                                              // Client area height in pixels
#define FPS 60                                                  // Target FPS

// Windows functions
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);           // Defines how each message will be processed
HWND InitializeWindow();                                        // Creates a window for the application

static HWND hWindow;                                            // Handle to window
static HINSTANCE hInstance;                                     // Handle to instance

static Graphics *gfx = nullptr;
static KeyboardServer *kbds = new KeyboardServer(10);           // Keyboard server contains the state of the keys in a virtual keyboard
static MouseServer *ms = nullptr;                               // Mouse server contains the state of the buttons in a virtual mouse
static Timer *timer = new Timer();                              // High precision timer with nanosecond precision

static unsigned frame_duration_us = ((float) 1000000 / FPS);    // How long to wait until a new frame is rendered
extern Logger *gpLogger;

#ifdef CONSOLE_MODE

int main(int argc, char **argv)
{
    return 0;
}
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG // Console is allocated in debug configuration
    AllocConsole();
    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;
#endif

    hWindow = InitializeWindow();
    if (!hWindow) return EXIT_FAILURE;

    ShowWindow(hWindow, nCmdShow);
    SetForegroundWindow(hWindow);
    SetFocus(hWindow);

    // Moves the mouse cursor to the center of the window
    SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);

    // The current position of the mouse is used to make sure everything is initialized properly
    POINT p;
    GetCursorPos(&p);
    ms = new MouseServer(p.x, p.y);

    // Initialize OpenGL
    gfx = new Graphics(hWindow, WIDTH, HEIGHT);
    Init(gfx, kbds, ms);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    timer->Reset();
    while (msg.message != WM_QUIT)
    {
        // Handles window's messages, they contain keypresses, mouse movement and window updates
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Main loop
            Loop();
            // Unloads buffer and update key states
            kbds->UpdateState();
            // Updates button states
            ms->UpdateState();
        }
    }
    Exit();

    if (!DestroyWindow(hWindow))
        gpLogger->Log("Error " + std::to_string(GetLastError()) + ": failed to destroy window");
    if (!UnregisterClass("Engi OpenGL Framework", hInstance))
        gpLogger->Log("Error " + std::to_string(GetLastError()) + ": failed to unregister window class");

    SafeDelete(gpLogger);
    SafeDelete(kbds);
    SafeDelete(ms);
    SafeDelete(timer);

#ifdef _DEBUG
    fclose(hf_out);
#endif

    return msg.wParam;
}
#endif

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
#pragma region Window commands
    case WM_ACTIVATE: // Windows message for when the window is activated
    {
        return 0;
    }
    case WM_DESTROY: // Windows message to destroy the window
    case WM_CLOSE: // Windows message to close the window
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_SIZE: // Windows message to resize a window
    {
        if (gfx) // Resize is called before we initialize the graphics, this will prevent undefined behavior
        {
            gfx->ChangeResolution(lParam & 0x0000FFFF, lParam >> 16);
        }
        break;
    }
#pragma endregion
#pragma region Keyboard Input
    case WM_KEYDOWN: // Windows message for KEYDOWN events
    {
        if (wParam == 27) // Escape is pressed
            PostQuitMessage(0);

        // Disables auto-repeat
        if (!(lParam & 0x40000000))
        {
            kbds->OnKeyDown(wParam);
        }
        return 0;
    }
    case WM_KEYUP: // Windows message for KEYUP events
    {
        kbds->OnKeyUp(wParam);
        return 0;
    }
#pragma endregion
#pragma region Mouse Input
    case WM_MOUSEMOVE: // Mouse movement
    {
        ms->MoveTo(lParam & 0x0000FFFF, lParam >> 16);
        break;
    }
    case WM_RBUTTONDOWN: // Right button down
    {
        ms->RightButtonDown();
        break;
    }
    case WM_RBUTTONUP: // Right button up
    {
        ms->RightButtonUp();
        break;
    }
    case WM_LBUTTONDOWN: // Left button down
    {
        ms->LeftButtonDown();
        break;
    }
    case WM_LBUTTONUP: // Left button up
    {
        ms->LeftButtonUp();
        break;
    }
    case WM_MBUTTONDOWN: // Middle button down
    {
        ms->MiddleButtonDown();
        break;
    }
    case WM_MBUTTONUP: // Middle button up
    {
        ms->MiddleButtonUp();
        break;
    }
#pragma endregion
    }
    // Any other messages are handled by the default procedure
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND InitializeWindow()
{
    HWND hwnd           = nullptr;
    hInstance           = GetModuleHandle(NULL);

    WNDCLASS wc;
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;   // Redraws window when it moves
    wc.lpfnWndProc      = (WNDPROC) WndProc;                    // Pointer to our message handling function
    wc.cbClsExtra       = 0;                                    // Not used
    wc.cbWndExtra       = 0;                                    // Not used
    wc.hInstance        = hInstance;                            // Instance of the window
    wc.hIcon            = LoadIcon(NULL, IDI_SHIELD);           // Window Icon
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);          // Mouse Arrow
    wc.hbrBackground    = NULL;                                 // Using OpenGL, so we don't need this
    wc.lpszMenuName     = NULL;                                 // No flyout menus
    wc.lpszClassName    = "Engi OpenGL Framework";              // Class name to register

    if (!RegisterClass(&wc))
    {
        gpLogger->Log("Error " + std::to_string(GetLastError()) + ": failed to register window class");
        return nullptr;
    }

    RECT WindowRect;
    WindowRect.left     = 0;
    WindowRect.right    = WIDTH;
    WindowRect.top      = 0;
    WindowRect.bottom   = HEIGHT;

    // We expect the client area of the window to be the specified resolution, so we need to adjust it to ensure it's correct
    AdjustWindowRectEx(&WindowRect, WS_OVERLAPPEDWINDOW, false, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
    if (!(hwnd=CreateWindowEx(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        "Engi OpenGL Framework",
        "Engi OpenGL Framework",
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
        // Creates window on the center of the screen
        GetSystemMetrics(SM_CXSCREEN) / 2 - WIDTH / 2,
        GetSystemMetrics(SM_CYSCREEN) / 2 - HEIGHT / 2,
        // Actual dimensions of the window
        WindowRect.right - WindowRect.left,
        WindowRect.bottom - WindowRect.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr)))
    {
        gpLogger->Log("Error " + std::to_string(GetLastError()) + ": failed to create window");
        return nullptr;
    }

    return hwnd;
}
#else
#error Currently, only Microsoft Windows is supported
#endif