#include <stdio.h>

#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)

#include <stdint.h>
#include "Main.h"

HWND gGameWindow;
BOOL gGameIsRunning;

GAMEBITMAP gBackBuffer;

MONITORINFO gMonitorInfo = { sizeof(MONITORINFO) };


int __stdcall WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PSTR CommandLine, int CmdShow)
{

    UNREFERENCED_PARAMETER(PreviousInstance);
    UNREFERENCED_PARAMETER(CommandLine);
    UNREFERENCED_PARAMETER(CmdShow);

    if (GameIsAlreadyRunning() == TRUE) {
        MessageBoxA(NULL, "Another Instance of this Program is Already Running!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (CreateMainGameWindow() != ERROR_SUCCESS) {
        goto Exit;
    }

    gBackBuffer.BitmapInfo.bmiHeader.biSize = sizeof(gBackBuffer.BitmapInfo.bmiHeader);
    gBackBuffer.BitmapInfo.bmiHeader.biWidth = GAME_RES_WIDTH;      // 384, 16:9 ratio and also divisible by 8, for many modern screens
    gBackBuffer.BitmapInfo.bmiHeader.biHeight = GAME_RES_HEIGHT;    // 216
    gBackBuffer.BitmapInfo.bmiHeader.biBitCount = GAME_BPP;         // 32, 32 is chosen because it's way easier to work with and more performant than 24
    gBackBuffer.BitmapInfo.bmiHeader.biCompression = BI_RGB;         
    gBackBuffer.BitmapInfo.bmiHeader.biPlanes = 1;    
    
    gBackBuffer.Memory = VirtualAlloc(NULL, GAME_DRAWING_AREA_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (gBackBuffer.Memory == NULL) {
        MessageBoxA(NULL, "Failed to Allocate Memory for Drawing Surface!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    } 



    MSG Message = { 0 };
   
    // MAIN GAME LOOP - for games that need infinite loop use gGameIsRunning
    gGameIsRunning = TRUE;
    while (gGameIsRunning == TRUE) {
        // for non games use GetMessage in previous commit, 
        // for non-blocking use PeakMessageA
        while (PeekMessageA(&Message, gGameWindow, 0, 0, PM_REMOVE)) {
            DispatchMessageA(&Message);
        }

        ProcessPlayerInput();
        RenderFrameGraphics();

        Sleep(1); // TODO: temp solution to high cpu usage, but will be changed later
    }


Exit:
    return 0;
}



LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
        case WM_CLOSE: {
            gGameIsRunning = FALSE;

            PostQuitMessage(0);

            break;
        }
        default: {
            Result = DefWindowProcA(WindowHandle, Message, WParam, LParam);
        }
    }
    return Result;
}


DWORD CreateMainGameWindow(void) {
    DWORD Result = ERROR_SUCCESS;

    WNDCLASSEXA WindowClass = { 0 };
 
    WindowClass.cbSize = sizeof(WNDCLASSEXA);
    WindowClass.style = 0;
    WindowClass.lpfnWndProc = MainWindowProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = GetModuleHandleA(NULL);
    WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    WindowClass.hbrBackground = CreateSolidBrush(RGB(255, 0, 255)); // Debug color. Hot Pink.
    WindowClass.lpszMenuName = NULL;
    WindowClass.lpszClassName = GAME_NAME "_WindowClass";
    
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

     if (!RegisterClassExA(&WindowClass))
    {
        Result = GetLastError();
        MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    gGameWindow = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        WindowClass.lpszClassName,
        "Window Title",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
        NULL, NULL, WindowClass.hInstance, NULL);

    if (gGameWindow == NULL)
    {
        Result = GetLastError();
        MessageBoxA(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (GetMonitorInfoA(MonitorFromWindow(gGameWindow, MONITOR_DEFAULTTOPRIMARY), &gMonitorInfo) == 0) {
        Result = ERROR_MONITOR_NO_DESCRIPTOR;
        goto Exit;
    }

    int MonitorWidth = gMonitorInfo.rcMonitor.right - gMonitorInfo.rcMonitor.left;
    int MonitorHeight = gMonitorInfo.rcMonitor.bottom - gMonitorInfo.rcMonitor.top;

    if (SetWindowLongPtrA(gGameWindow, GWL_STYLE, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_OVERLAPPEDWINDOW) == 0) {
        Result = GetLastError();
        goto Exit;
    };

    if (SetWindowPos(gGameWindow, HWND_TOPMOST, gMonitorInfo.rcMonitor.left, gMonitorInfo.rcMonitor.top, MonitorWidth, MonitorHeight, SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0) {
        Result = GetLastError();
        goto Exit;
    };

Exit:
    return 0;
}


BOOL GameIsAlreadyRunning(void) {
    HANDLE Mutex = NULL;
    Mutex = CreateMutexA(NULL, FALSE, GAME_NAME "_GameMutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return (TRUE);
    }
    else {
        return(FALSE);
    }
}

void ProcessPlayerInput(void) {
    short EscapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE); // returns true if pressed and false if not.

    if (EscapeKeyIsDown) {
        SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
    }
}

void RenderFrameGraphics(void) {
    HDC DeviceContext = GetDC(gGameWindow);
    
    StretchDIBits(DeviceContext, 0, 0, 100, 100, 0, 0, 100, 100, gBackBuffer.Memory, &gBackBuffer.BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    
    
    ReleaseDC(gGameWindow, DeviceContext);
}