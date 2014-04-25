//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1												//
//  WindowsMain.cpp																//
//  																			//
//  This file is part of Engi OpenGL Framework									//
//  																			//
//  This program is free software: you can redistribute it and/or modify		//
//      it under the terms of the GNU General Public License as published by	//
//      the Free Software Foundation, either version 3 of the License, or		//
//      (at your option) any later version.										//
//  																			//
//      This program is distributed in the hope that it will be useful,			//
//      but WITHOUT ANY WARRANTY; without even the implied warranty of			//
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			//
//      GNU General Public License for more details.							//
//  																			//
//      You should have received a copy of the GNU General Public License		//
//      along with this program.  If not, see <http://www.gnu.org/licenses/>	//
//------------------------------------------------------------------------------//

#if defined(_WIN32) || defined(__WIN32__) || defined(_MSC_VER)
	int x;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <vector>
#include <string>

// TODO: configuration file
#define WIDTH 800
#define HEIGHT 600

// Windows functions
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND InitializeWindow();

extern void MainLoop();

static HWND hWindow;
static HINSTANCE hInstance;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// TODO: add console if --debug 

	hWindow = InitializeWindow();
	ShowWindow(hWindow, nCmdShow);
	SetForegroundWindow(hWindow);
	SetFocus(hWindow);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	int x = 0;
	while (msg.message != WM_QUIT)
	{
		// Handles window's messages, they contain keypresses, mouse movement and window updates
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			MainLoop();
		}
	}
	UnregisterClass("Engi OpenGL Framework", hInstance);

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE: // Windows message for when the window is activated
	{
		return 0;
	}
	case WM_KEYDOWN: // Windows message for KEYDOWN events
	{
		return 0;
	}
	case WM_KEYUP: // Windows message for KEYUP events
	{
		return 0;
	}
	case WM_SIZE: // Windows message to resize a window
	{
		break;
	}
	case WM_SYSCOMMAND: // Windows message for system calls
	{
		break;
	}
	case WM_DESTROY: // Windows message to destroy the window
	case WM_CLOSE: // Windows message to close the window
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	// Any other messages are handled by the default procedure
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND InitializeWindow()
{
	HWND hwnd			= nullptr;
	hInstance			= GetModuleHandle(NULL);

	WNDCLASS wc;
	DWORD dwExStyle;

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
		return nullptr;
	}

	return hwnd;
}

#endif