#pragma once

#include "Graphics/Graphics.h"

typedef struct _SceneRenderer ISceneRenderer;

bool SceneRenderer_Initialize(GfxHandle handle, u32 width, u32 height, ISceneRenderer** ppSceneRenderer);
void SceneRenderer_Shutdown(ISceneRenderer** ppSceneRenderer);
void SceneRenderer_DrawScene(GfxHandle handle, ISceneRenderer* pSceneRenderer, D3D11_VIEWPORT* pViewport);