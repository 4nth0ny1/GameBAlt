#include <stdio.h>

#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)

#include "Main.h"


int WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PSTR CommandLine, int CmdShow)
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

    MSG Message = { 0 };

    while (GetMessageA(&Message, NULL, 0, 0) > 0) {
        TranslateMessage(&Message);
        DispatchMessageA(&Message);
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
    HWND WindowHandle = 0;

    WindowClass.cbSize = sizeof(WNDCLASSEXA);
    WindowClass.style = 0;
    WindowClass.lpfnWndProc = MainWindowProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = GetModuleHandleA(NULL);
    WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WindowClass.lpszMenuName = NULL;
    WindowClass.lpszClassName = GAME_NAME "_WindowClass";
    WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);

    if (!RegisterClassExA(&WindowClass))
    {
        Result = GetLastError();
        MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    WindowHandle = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        WindowClass.lpszClassName,
        "Window Title",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, WindowClass.hInstance, NULL);

    if (WindowHandle == NULL)
    {
        Result = GetLastError();
        MessageBoxA(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

Exit:
    return 0;
}


// 24. Create global mutex to stop the user from opening more than one game at a time.
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