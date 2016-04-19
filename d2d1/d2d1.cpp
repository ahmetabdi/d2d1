// d2d1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "d2d1.h"
#include "overlay.h"
#include "Process.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance

Graphics * graphics;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
// My declarations
BOOL OverwriteValues();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D2D1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    //WNDCLASSEXW wcex;
	WNDCLASSEX wcex = { NULL };

	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = OVERLAY_NAME;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.cbClsExtra = NULL;
	wcex.cbWndExtra = NULL;
	wcex.hInstance = NULL;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	return RegisterClassEx(&wcex);
}

HRESULT ExtendIntoClientAll(HWND hwnd)
{
	// Negative margins have special meaning to DwmExtendFrameIntoClientArea.
	// Negative margins create the "sheet of glass" effect, where the client area
	// is rendered as a solid surface with no window border.
	MARGINS margins = { -1 };
	HRESULT hr = S_OK;

	// Extend the frame across the entire window.
	hr = DwmExtendFrameIntoClientArea(hwnd, &margins);
	if (SUCCEEDED(hr))
	{
		// ...
	}
	return hr;
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   RECT rect;// = { 0, 0, 1920, 1080 };

   //HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   //   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   HWND gameWindow = FindWindow(NULL, "Star Wars™: The Old Republic™");
   //HWND gameWindow = FindWindow(NULL, "AssaultCube");
   if (!gameWindow) {
	   ExitProcess(0);
   }

   // Set rect to window position, better than manually setting..
   GetWindowRect(gameWindow, &rect);

   AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

   HWND hWnd = CreateWindowEx(WS_EX_LAYERED, OVERLAY_NAME, "Teste", WS_EX_TOPMOST | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
	   rect.right, rect.bottom, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   graphics = new Graphics();

   if (!graphics->Init(hWnd)) {
	   delete graphics; return -1;
   };

   if (!process->Attach("swtor.exe")) {
	   ExitProcess(0);
   }

   client = process->GetModule("swtor.exe");

   //DWORD_PTR MainPtr = client->GetImage();

   //wchar_t buffer[256];
   //wsprintfW(buffer, L"0x%I64X\n", MainPtr);
   //OutputDebugStringW(buffer);

   //DWORD_PTR LogPtr = process->Read<DWORD_PTR>(0x73877448);
   //DWORD_PTR LogPtr = process->Read<DWORD_PTR>(0x73877450);

   //wchar_t buffer[256];
   //wsprintfW(buffer, L"0x%I64X\n", LogPtr);
   //OutputDebugStringW(buffer);

   // Sets the opacity and transparency color key of a layered window.
   SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, ULW_COLORKEY | LWA_ALPHA);
   // Extends the window frame into the client area.
   ExtendIntoClientAll(hWnd);
   // Sets the specified window's show state.
   ShowWindow(hWnd, nCmdShow);
   // Changes the size, position, and Z order of a child, pop-up, or top-level window.
   SetWindowPos(gameWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
   SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

   // Move window to correct place
   RECT game;

   GetWindowRect(gameWindow, &game);

   int w = game.right - game.left;
   int h = game.bottom - game.top;

   LONG_PTR dwStyle = GetWindowLongPtr(gameWindow, GWL_STYLE);

   if (dwStyle & WS_BORDER) {
	   int x = GetSystemMetrics(SM_CXBORDER);
	   int y = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER);

	   game.left += x;
	   game.top += y;

	   w -= x;
	   h -= y;
   }

   MoveWindow(hWnd, game.left, game.top, w, h, TRUE);

   // Perform background thread initalization:
   // This is used for stuff called outside of the drawing
   if (!CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OverwriteValues, NULL, NULL, NULL)) {
	   return false;
   }

   // UpdateWindow function updates the client area of the specified window by sending a WM_PAINT message
   UpdateWindow(hWnd);

   while (true) {
	   graphics->Render();
   }

   return TRUE;
}

BOOL OverwriteValues() {
	DWORD_PTR MainPtr = client->GetImage();
	DWORD_PTR MainPtrSize = client->GetSize();

	DWORD_PTR PlayerDropAddress;
	bool CtrlKeyToggle = false;

	/*Loop forever, with a millisecond interval per loop*/
	for (;; Sleep(100))
	{
		// Stop player from falling
		/*
		if (GetAsyncKeyState(VK_CONTROL)) {
			CtrlKeyToggle = !CtrlKeyToggle;

			if (CtrlKeyToggle) {
				PlayerDropAddress = process->Scan(MainPtr, MainPtrSize, "\xF3\x0F\x11\x46\x0c\xF3\x0F\x10\x40", "xxxxxxxxx");
				BYTE NopDrop[] = { 0x90, 0x90, 0x90, 0x90, 0x90 }; // SIZE (5) This REMOVES dropping Y AXIS
				process->Write(PlayerDropAddress, NopDrop);
			}
			else {
				PlayerDropAddress = process->Scan(MainPtr, MainPtrSize, "\x90\x90\x90\x90\x90\xF3\x0F\x10\x40", "xxxxxxxxx");
				BYTE RestoreDrop[] = { 0xF3, 0x0F, 0x11, 0x46, 0x0C }; // SIZE (5) This RESTORES dropping Y AXIS
				process->Write(PlayerDropAddress, RestoreDrop);
			}
		}
		*/

	}
	return true;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        break;
    case WM_PAINT:
        {
            //PAINTSTRUCT ps;
            //HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...
			//EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}