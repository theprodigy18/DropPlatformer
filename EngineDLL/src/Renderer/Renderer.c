#include "pch.h"
#include "Renderer/Renderer.h"

#include "Platform/Window.h"
#include "Graphics/Graphics.h"

#include "Platform/Input.h"

#include "Renderer/SceneRenderer.h"

#pragma region INTERNAL
static bool s_exit = false;

static bool OnClose()
{
    PostQuitMessage(0);
    s_exit = true;
    return true;
}

typedef struct _Renderer
{
    WndHandle wndHandle;
    GfxHandle gfxHandle;

    bool isRunning;
} _Renderer;

static bool InitializeCore(const wchar_t* title, u32 width, u32 height, IRenderer* pRenderer)
{
    DO_ASSERT_MSG(pRenderer, "Pointer to renderer is null.");

    WndInitProps wndProps = {
        .title    = title,
        .width    = width,
        .height   = height,
        .callback = {
            .OnClose = OnClose}};

    if (!Window_CreateWindow(&wndProps, &pRenderer->wndHandle))
    {
        DO_ERROR("Failed to create a window.");
        return false;
    }

    GfxInitProps gfxProps = {
        .wndHandle = pRenderer->wndHandle};

    if (!Graphics_CreateGraphics(&gfxProps, &pRenderer->gfxHandle))
    {
        DO_ERROR("Failed to create graphics.");
        Window_DestroyWindow(&pRenderer->wndHandle);
        return false;
    }

    return true;
}
static void ShutdownCore(IRenderer* pRenderer)
{
    DO_ASSERT_MSG(pRenderer, "Pointer to renderer is null.");

    Graphics_DestroyGraphics(&pRenderer->gfxHandle);
    Window_DestroyWindow(&pRenderer->wndHandle);
}
#pragma endregion

bool Renderer_Initialize(const wchar_t* title, u32 width, u32 height, IRenderer** ppRenderer)
{
    DO_ASSERT_MSG(ppRenderer, "Pointer to renderer is null.");

    IRenderer* pRenderer = (IRenderer*) ArenaAllocator_Allocate(DO_PERSISTENT, sizeof(_Renderer));
    if (!pRenderer)
    {
        DO_ERROR("Failed to allocate memory for renderer pointer.");
        return false;
    }

    if (!InitializeCore(title, width, height, pRenderer))
    {
        DO_ERROR("Failed to initialize core system.");
        return false;
    }

    pRenderer->isRunning = true;

    ShowWindow(pRenderer->wndHandle->hwnd, SW_SHOW);

    *ppRenderer = pRenderer;

    return true;
}
void Renderer_Shutdown(IRenderer** ppRenderer)
{
    DO_ASSERT_MSG(ppRenderer && *ppRenderer, "Pointer to renderer is null.");
    IRenderer* pRenderer = *ppRenderer;

    ShowWindow(pRenderer->wndHandle->hwnd, SW_HIDE);

    ShutdownCore(pRenderer);

    *ppRenderer = NULL;
}

bool Renderer_ProcessFrame(IRenderer* pRenderer)
{
    DO_ASSERT_MSG(pRenderer, "Pointer to renderer is null");

    Input_UpdatePrevState();

    return pRenderer->isRunning;
}

void Renderer_BeginFrame(IRenderer* pRenderer)
{
    DO_ASSERT_MSG(pRenderer, "Pointer to renderer is null");

    Window_PollEvents();
}

void Renderer_DrawFrame(IRenderer* pRenderer)
{
    DO_ASSERT_MSG(pRenderer, "Pointer to renderer is null");

    SceneRenderer_DrawScene(pRenderer->gfxHandle);
}

void Renderer_EndFrame(IRenderer* pRenderer)
{
    DO_ASSERT_MSG(pRenderer, "Pointer to renderer is null");
    IDXGISwapChain* pSwapChain = pRenderer->gfxHandle->pSwapChain;

    pSwapChain->lpVtbl->Present(pSwapChain, 1, 0);
    ArenaAllocator_ClearArena(DO_TRANSIENT);
    if (s_exit) pRenderer->isRunning = false;
}