#include <stdio.h>

#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)

#include "Main.h"



// 1. Main 
int WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PSTR CommandLine, int CmdShow)
{
    UNREFERENCED_PARAMETER(PreviousInstance);
    UNREFERENCED_PARAMETER(CommandLine);
    UNREFERENCED_PARAMETER(CmdShow);

    // 18. call Create MainWindowFunction
    if (CreateMainGameWindow(Instance) != ERROR_SUCCESS) {
        goto Exit;
    }

    // 3. Message Loop 
    MSG Message = { 0 };

    while (GetMessageA(&Message, NULL, 0, 0) > 0) {
        TranslateMessage(&Message);
        DispatchMessageA(&Message);
    }

Exit:
    return 0;
}


// 2. Window
// 10. cleaned up this function, renamed parameters, created result, made everything default response
// 19. Added SAL - Source annotation lang
LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
        // 22. adding ability to shutdown program on exit.
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

// 13. DWORD CreateMainGameWindow() 
// 19. Added SAL - Source annotation lang
// 20. removed instance parameter and added void
DWORD CreateMainGameWindow(void) {
    DWORD Result = ERROR_SUCCESS;

    // 14. copy and paste all window stuff from the winmain function and moved it to this function
    
    // 4. Register a window class
    // 11. GetModuleHandleA
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
    WindowClass.lpszClassName = "Game_B_ALT_WindowClass";
    WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);

    // 5. Register class comes from winprog website, but docs is #5 in bookmarks
    if (!RegisterClassExA(&WindowClass))
    {
        // 16. 
        Result = GetLastError();
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    // 5. Create the window
    // 21. Replaced instance with WindowClass.hInstance which gets from GetModule ...
    WindowHandle = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        WindowClass.lpszClassName,
        "Window Title",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, WindowClass.hInstance, NULL);

    if (WindowHandle == NULL)
    {
        // 17.
        Result = GetLastError();
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    // 15. Exit Message 
Exit:
    return 0;
}