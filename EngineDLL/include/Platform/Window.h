#pragma once

typedef struct _WndHandle
{
    HWND hwnd;

    u32 width, height;
} _WndHandle;

typedef _WndHandle* WndHandle;

typedef struct _WndCallback
{
    bool (*OnClose)();
} WndCallback;

typedef struct _WndInitProps
{
    const wchar_t* title;
    u32            width;
    u32            height;
    WndCallback    callback;
} WndInitProps;

// Create a window.
bool Window_CreateWindow(const WndInitProps* pProps, WndHandle* pHandle);
// Destroy a window.
void Window_DestroyWindow(WndHandle* pHandle);
// Handle message loop.
void Window_PollEvents();