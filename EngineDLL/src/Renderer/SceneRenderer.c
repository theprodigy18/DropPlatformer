#include "pch.h"
#include "Renderer/SceneRenderer.h"

#pragma region INTERNAL
typedef struct _SceneRenderer
{
    GfxRenderTarget* pHDRRenderTarget;
} _SceneRenderer;
#pragma endregion

bool SceneRenderer_Initialize(GfxHandle handle, ISceneRenderer** ppSceneRenderer);
void SceneRenderer_Shutdown(ISceneRenderer** ppSceneRenderer);

void SceneRenderer_DrawScene(GfxHandle handle)
{
    DO_ASSERT_MSG(handle, "Graphics handle is null.");

    f32 clearColor[] = {0.1f, 0.2f, 0.5f, 1.0f};

    handle->pContext->lpVtbl->OMSetRenderTargets(handle->pContext, 0, &handle->pBackBufferRTV, NULL);
    handle->pContext->lpVtbl->ClearRenderTargetView(handle->pContext, handle->pBackBufferRTV, clearColor);
}
