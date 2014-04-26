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

#if defined(_WIN32) || defined(__WIN32__) || defined(_MSC_VER)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <thread>

#ifdef _DEBUG
#include <consoleapi.h>
#include <fcntl.h>
#include <io.h>
#endif

#include "Timer.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Logger.h"
#include "Utility.h"

using namespace Keyboard;
using namespace Mouse;

// Framework methods
extern void Init(KeyboardServer *kbds, MouseServer *ms, Logger *logger);
extern void Loop();
extern void Exit();

// TODO: configuration file
#define WIDTH 800
#define HEIGHT 600
#define FPS 60

// Windows functions
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND InitializeWindow();

static HWND hWindow;
static HINSTANCE hInstance;

static KeyboardServer *kbds = new KeyboardServer(10);
static MouseServer *ms = new MouseServer();
static Logger *logger = new Logger(".\\out.log");
static Timer *timer = new Timer();

static unsigned frame_duration_us = (1000000 / FPS);

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

    Init(kbds, ms, logger);
    ShowWindow(hWindow, nCmdShow);
    SetForegroundWindow(hWindow);
    SetFocus(hWindow);

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
            auto duration = frame_duration_us - timer->Elapsed_us();
            if (duration > 0) std::this_thread::sleep_for(std::chrono::microseconds(duration));
            Loop();
            kbds->UpdateState();
            ms->UpdateState();
            timer->Reset();
        }
    }
    Exit();

    if (!DestroyWindow(hWindow))
        logger->Log("Error " + std::to_string(GetLastError()) + ": failed to destroy window");
    if (!UnregisterClass("Engi OpenGL Framework", hInstance))
        logger->Log("Error " + std::to_string(GetLastError()) + ": failed to unregister window class");

    SafeDelete(logger);
    SafeDelete(kbds);
    SafeDelete(ms);
    SafeDelete(timer);

    #ifdef _DEBUG
    fclose(hf_out);
    #endif

    return msg.wParam;
}

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
        break;
    }
    #pragma endregion
    #pragma region Keyboard Input
    case WM_KEYDOWN: // Windows message for KEYDOWN events
    {
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
    HWND hwnd			= nullptr;
    hInstance			= GetModuleHandle(NULL);

    WNDCLASS wc;
    wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws window when it moves
    wc.lpfnWndProc		= (WNDPROC) WndProc;					// Pointer to our message handling function
    wc.cbClsExtra		= 0;									// Not used
    wc.cbWndExtra		= 0;									// Not used
    wc.hInstance		= hInstance;							// Instance of the window
    wc.hIcon			= LoadIcon(NULL, IDI_SHIELD);			// Window Icon
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Mouse Arrow
    wc.hbrBackground	= NULL;									// Using OpenGL, so we don't need this
    wc.lpszMenuName		= NULL;									// No flyout menus
    wc.lpszClassName	= "Engi OpenGL Framework";				// Class name to register

    if (!RegisterClass(&wc))
    {
        logger->Log("Error " + std::to_string(GetLastError()) + ": failed to register window class");
        return nullptr;
    }

    RECT WindowRect;
    WindowRect.left		= 0;
    WindowRect.right	= WIDTH;
    WindowRect.top		= 0;
    WindowRect.bottom	= HEIGHT;

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
        logger->Log("Error " + std::to_string(GetLastError()) + ": failed to create window");
        return nullptr;
    }

    return hwnd;
}
#endif