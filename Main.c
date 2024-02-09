#include <stdio.h>

#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)


// 6. Declare MainWindowProc function to fix undeclared identifier error 
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 1. Main 
int WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PSTR CommandLine, int CmdShow)
{
    UNREFERENCED_PARAMETER(PreviousInstance);
    UNREFERENCED_PARAMETER(CommandLine);
    UNREFERENCED_PARAMETER(CmdShow);

    // 4. Register a window class

    WNDCLASSEXA WindowClass = { 0 };
    HWND WindowHandle = 0;

    WindowClass.cbSize = sizeof(WNDCLASSEXA);
    WindowClass.style = 0;
    WindowClass.lpfnWndProc = MainWindowProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WindowClass.lpszMenuName = NULL;
    WindowClass.lpszClassName = "Game_B_ALT_WindowClass";
    WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);

    // 5. Register class comes from winprog website, but docs is #5 in bookmarks
    if (!RegisterClassExA(&WindowClass))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // 5. Create the window
    WindowHandle = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        WindowClass.lpszClassName,
        "Window Title",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, Instance, NULL);

    if (WindowHandle == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // 7. Show the window
    ShowWindow(WindowHandle, TRUE);

    // 3. Message Loop 
    MSG Message = { 0 };

    while (GetMessageA(&Message, NULL, 0, 0) > 0) {
        TranslateMessage(&Message);
        DispatchMessageA(&Message);
    }

    return 0;
}


// 2. Window
LRESULT CALLBACK MainWindowProc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{

    switch (uMsg)
    {
        case WM_CREATE:
            // Initialize the window. 
            return 0;

        case WM_PAINT:
            // Paint the window's client area. 
            return 0;

        case WM_SIZE:
            // Set the size and position of the window. 
            return 0;

        case WM_DESTROY:
            // Clean up window-specific data objects. 
            return 0;

            // 
            // Process other messages. 
            // 

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}