#include "pch.h"
#include "Renderer/SceneRenderer.h"

void SceneRenderer_DrawScene(GfxHandle handle)
{
    DO_ASSERT_MSG(handle, "Graphics handle is null.");

    f32 clearColor[] = {0.1f, 0.2f, 0.5f, 1.0f};

    handle->pContext->lpVtbl->OMSetRenderTargets(handle->pContext, 0, &handle->pBackBufferRTV, NULL);
    handle->pContext->lpVtbl->ClearRenderTargetView(handle->pContext, handle->pBackBufferRTV, clearColor);
}
