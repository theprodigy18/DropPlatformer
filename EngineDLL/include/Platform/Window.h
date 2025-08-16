#pragma once

typedef struct _WndHandle
{
    HWND hwnd;
} _WndHandle;

typedef _WndHandle* WndHandle;

typedef struct _WndCallback
{
    bool (*OnClose)();
} WndCallback;

typedef struct _WndInitProps
{
    wchar_t*    title;
    u32         width;
    u32         height;
    WndCallback callback;
} WndInitProps;

// Create a window.
bool DROP_CreateWindow(const WndInitProps* pProps, WndHandle* pHandle);
// Destroy a window.
void DROP_DestroyWindow(WndHandle* pHandle);
// Handle message loop.
void DROP_PollEvents();