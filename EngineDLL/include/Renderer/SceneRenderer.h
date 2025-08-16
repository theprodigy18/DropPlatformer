#pragma once

#include "Graphics/Graphics.h"

typedef struct _SceneRenderer ISceneRenderer;

bool SceneRenderer_Initialize(GfxHandle handle, ISceneRenderer** ppSceneRenderer);
void SceneRenderer_Shutdown(ISceneRenderer** ppSceneRenderer);
void SceneRenderer_DrawScene(GfxHandle handle);