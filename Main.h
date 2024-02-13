#pragma once

#define GAME_NAME						"Game_B_Alt"
#define GAME_RES_WIDTH					384
#define GAME_RES_HEIGHT					240
#define GAME_BPP						32
#define GAME_DRAWING_AREA_MEMORY_SIZE	(GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP / 8))
#define CALC_AVG_FPS_EVERY_X_FRAMES		100

// typedef creates an alias not a type
typedef struct GAMEBITMAP {
	BITMAPINFO BitmapInfo;
	void* Memory;
} GAMEBITMAP;

typedef struct PIXEL32 {
	uint8_t Blue;		// 4 8-bit values 
	uint8_t Green;
	uint8_t Red;
	uint8_t Alpha;
} PIXEL32;

typedef struct PERF_DATA {
	uint64_t TotalFramesRendered;
	uint32_t RawFramesPerSecondAverage;
	uint32_t CookedFramesPerSecondAverage;
	LARGE_INTEGER PerfFrequency;
	LARGE_INTEGER FrameStart;
	LARGE_INTEGER FrameEnd;
	LARGE_INTEGER ElapsedMicrosecondsPerFrame;
	MONITORINFO MonitorInfo;
	int32_t MonitorWidth;
	int32_t MonitorHeight;
} PERF_DATA;

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam);
DWORD CreateMainGameWindow(void);

BOOL GameIsAlreadyRunning(void);

void ProcessPlayerInput(void);
void RenderFrameGraphics(void);