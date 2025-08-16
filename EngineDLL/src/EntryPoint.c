#include "pch.h"
#include "EntryPoint.h"

#include "Platform/Window.h"
#include "Platform/Input.h"

static bool InitializeGlobalMemory(u64 persistentSize, u64 transientSize);
static void CleanupGlobalMemory();

static bool s_isRunning = true;

static bool OnClose()
{
    PostQuitMessage(0);
    s_isRunning = false;

    return true;
}

int EntryPoint()
{
    if (!InitializeGlobalMemory(KB(1), KB(1)))
    {
        DO_ASSERT_MSG(false, "Failed to initialize global memory.");
        return 1;
    }

    WndInitProps wndProps = {
        .title    = L"2D Drop Engine",
        .width    = 1280,
        .height   = 720,
        .callback = {
            .OnClose = OnClose}};

    WndHandle handle = NULL;

    DROP_CreateWindow(&wndProps, &handle);

    ShowWindow(handle->hwnd, SW_SHOW);

    while (s_isRunning)
    {
        DROP_UpdatePrevState();

        DROP_PollEvents();
    }

    ShowWindow(handle->hwnd, SW_HIDE);

    CleanupGlobalMemory();
    return 0;
}

static bool InitializeGlobalMemory(u64 persistentSize, u64 transientSize)
{
    g_memory = (Memory) DO_ALLOC(_Memory, 1);
    if (!g_memory)
    {
        DO_ERROR("Failed to allocate global memory.");
        return false;
    }

    if (!DROP_CreateArena(DO_PERSISTENT, persistentSize) ||
        !DROP_CreateArena(DO_TRANSIENT, transientSize))
    {
        DO_ERROR("Failed to create persistent or transient storage.");
        DO_FREE(g_memory);
        return false;
    }

    return true;
}
static void CleanupGlobalMemory()
{
    DO_FREE(g_memory);
    g_memory = NULL;
}