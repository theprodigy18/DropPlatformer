#pragma once

typedef struct _Renderer IRenderer;

bool Renderer_Initialize(const wchar_t* title, u32 width, u32 height, IRenderer** ppRenderer);
void Renderer_Shutdown(IRenderer** ppRenderer);

bool Renderer_ProcessFrame(IRenderer* pRenderer);
void Renderer_BeginFrame(IRenderer* pRenderer);
void Renderer_DrawFrame(IRenderer* pRenderer);
void Renderer_EndFrame(IRenderer* pRenderer);