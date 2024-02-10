#include <stdio.h>

#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)


// 6. Declare MainWindowProc function to fix undeclared identifier error 
// 9. Renamed paramters
LRESULT CALLBACK MainWindowProc(HWND WindowHandle, UINT Message, WPARAM WParam, LPARAM LParam);

// 12. CreateMainGameWindow()
DWORD CreateMainGameWindow(HANDLE Instance);

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
LRESULT CALLBACK MainWindowProc(HWND WindowHandle, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
        default: {
            Result = DefWindowProcA(WindowHandle, Message, WParam, LParam);
        }
    }
    return Result;
}

// 13. DWORD CreateMainGameWindow() 
DWORD CreateMainGameWindow(HANDLE Instance) {
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
    WindowHandle = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        WindowClass.lpszClassName,
        "Window Title",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, Instance, NULL);

    if (WindowHandle == NULL)
    {
        // 17.
        Result = GetLastError();
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    // 7. Show the window, 
    // 8. NOT NECESSARY, add WS_VISIBLE to the CreateWindowExA()
    // ShowWindow(WindowHandle, TRUE);

    // 15. Exit Message 
Exit:
    return 0;
}