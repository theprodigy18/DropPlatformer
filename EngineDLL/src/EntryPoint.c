#include "pch.h"
#include "EntryPoint.h"

#include "Renderer/Renderer.h"

static bool InitializeGlobalMemory(u64 persistentSize, u64 transientSize);
static void CleanupGlobalMemory();

int EntryPoint()
{
    if (!InitializeGlobalMemory(KB(1), KB(1)))
    {
        DO_ASSERT_MSG(false, "Failed to initialize global memory.");
        return 1;
    }

    IRenderer* pRenderer = NULL;
    if (!Renderer_Initialize(L"Drop Platformer", 1280, 720, &pRenderer))
    {
        DO_ASSERT_MSG(false, "Failed to initialize renderer.");
        CleanupGlobalMemory();
        return 1;
    }

    while (Renderer_ProcessFrame(pRenderer))
    {
        Renderer_BeginFrame(pRenderer);
        Renderer_DrawFrame(pRenderer);
        Renderer_EndFrame(pRenderer);
    }

    Renderer_Shutdown(&pRenderer);
    CleanupGlobalMemory();

    DO_PRINT_LEAKS();
    DO_CLEANUP();
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

    if (!ArenaAllocator_CreateArena(DO_PERSISTENT, persistentSize) ||
        !ArenaAllocator_CreateArena(DO_TRANSIENT, transientSize))
    {
        DO_ERROR("Failed to create persistent or transient storage.");
        DO_FREE(g_memory);
        return false;
    }

    return true;
}
static void CleanupGlobalMemory()
{
    DO_FREE(g_memory->transientStorage.memory);
    DO_FREE(g_memory->persistentStorage.memory);
    DO_FREE(g_memory);
    g_memory = NULL;
}