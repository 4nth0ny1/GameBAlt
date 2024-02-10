#pragma once

// 6. Declare MainWindowProc function to fix undeclared identifier error 
// 9. Renamed paramters
// 19. Added SAL - Source annotation lang
LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam);

// 12. CreateMainGameWindow()
// 19. Added SAL - Source annotation lang
// 20. removed instance parameter and added void
DWORD CreateMainGameWindow(void);