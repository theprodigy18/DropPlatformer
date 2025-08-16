#include "pch.h"
#include "Platform/Window.h"

#include "Platform/Input.h"

#include <windowsx.h>

#pragma region INTERNAL
static LRESULT CALLBACK InternalWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    bool result = false;

    switch (msg)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        DROP_SetKeyState((u32) wParam, true);
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        DROP_SetKeyState((u32) wParam, false);
        break;
    case WM_LBUTTONDOWN:
        DROP_SetMouseButton(0, true);
        break;
    case WM_LBUTTONUP:
        DROP_SetMouseButton(0, false);
        break;
    case WM_RBUTTONDOWN:
        DROP_SetMouseButton(1, true);
        break;
    case WM_RBUTTONUP:
        DROP_SetMouseButton(1, false);
        break;
    case WM_MOUSEMOVE:
        DROP_SetMousePos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_CLOSE:
    {
        WndCallback* pCallback = (WndCallback*) GetWindowLongPtrW(hwnd, GWLP_USERDATA);
        if (pCallback->OnClose)
            result = pCallback->OnClose();
        break;
    }
    default:
        break;
    }

    return result ? 0 : DefWindowProcW(hwnd, msg, wParam, lParam);
}

static LRESULT CALLBACK TempWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_NCCREATE)
    {
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR) InternalWndProc);

        CREATESTRUCTW* pCS       = (CREATESTRUCTW*) lParam;
        WndCallback*   pCallback = pCS->lpCreateParams;
        if (!pCallback)
        {
            DO_ASSERT_MSG(false, "Failed to assign callback to the window.");
            return DefWindowProcW(hwnd, msg, wParam, lParam);
        }

        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR) pCallback);
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

static const wchar_t* WND_CLASS_NAME = L"DROP-2D-Engine";

static HINSTANCE s_hInstance = NULL;
#pragma endregion

bool DROP_CreateWindow(const WndInitProps* pProps, WndHandle* pHandle)
{
    DO_ASSERT_MSG(pProps, "Pointer to the properties is null.");
    DO_ASSERT_MSG(pHandle, "Pointer to the handle is null.");

    *pHandle = NULL;

    SetProcessDPIAware();

    s_hInstance = GetModuleHandleW(NULL);
    if (!s_hInstance)
    {
        DO_ASSERT_MSG(false, "Failed to get module handle.");
        return false;
    }

    WNDCLASSEXW wcex = {
        .cbSize        = sizeof(WNDCLASSEXW),
        .style         = 0,
        .hInstance     = s_hInstance,
        .cbClsExtra    = 0,
        .cbWndExtra    = 0,
        .hCursor       = LoadCursorW(s_hInstance, IDC_ARROW),
        .hIcon         = NULL,
        .hIconSm       = NULL,
        .lpszClassName = WND_CLASS_NAME,
        .lpszMenuName  = NULL,
        .lpfnWndProc   = TempWndProc};
    if (!RegisterClassExW(&wcex))
    {
        DO_ASSERT_MSG(false, "Failed to register window class.");
        return false;
    }

    DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT  rc      = {0, 0, pProps->width, pProps->height};
    AdjustWindowRectEx(&rc, dwStyle, FALSE, 0);
    u32 width  = rc.right - rc.left;
    u32 height = rc.bottom - rc.top;

    WndCallback* pCallback = (WndCallback*) DROP_Allocate(DO_PERSISTENT, sizeof(WndCallback));
    if (!pCallback)
    {
        DO_ASSERT_MSG(false, "Failed to allocate memory for window callback struct.");
        return false;
    }
    *pCallback = pProps->callback;

    HWND hwnd = CreateWindowExW(
        0, WND_CLASS_NAME, pProps->title, dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, s_hInstance, (LPVOID) pCallback);
    if (!hwnd)
    {
        DO_ASSERT_MSG(false, "Failed to create window.");
        return false;
    }

    WndHandle handle = (WndHandle) DROP_Allocate(DO_PERSISTENT, sizeof(_WndHandle));
    if (!handle)
    {
        DO_ASSERT_MSG(false, "Failed to allocate memory for window handle.");
        DestroyWindow(hwnd);
        return false;
    }

    handle->hwnd = hwnd;

    *pHandle = handle;

    return true;
}

void DROP_DestroyWindow(WndHandle* pHandle)
{
    DO_ASSERT_MSG(pHandle && *pHandle, "Pointer to the handle or the handle itself is null.");
    WndHandle handle = *pHandle;

    if (handle)
    {
        DestroyWindow(handle->hwnd);

        handle->hwnd = NULL;
    }

    *pHandle = NULL;

    UnregisterClassW(WND_CLASS_NAME, s_hInstance);

    s_hInstance = NULL;
}

void DROP_PollEvents()
{
    MSG msg;
    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            break;

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}
