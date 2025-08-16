#pragma once

#include "Platform/Window.h"

typedef struct _GfxHandle
{
    ID3D11Device*           pDevice;
    ID3D11DeviceContext*    pContext;
    IDXGISwapChain*         pSwapChain;
    ID3D11RenderTargetView* pBackBufferRTV;
} _GfxHandle;

typedef _GfxHandle* GfxHandle;

typedef struct _GfxInitProps
{
    WndHandle wndHandle;
} GfxInitProps;

typedef struct _GfxRenderTarget
{
    ID3D11RenderTargetView*   pRTV;
    ID3D11ShaderResourceView* pSRV;
    ID3D11Texture2D*          pTexture;

    u32 width, height;
} GfxRenderTarget;

bool Graphics_CreateGraphics(const GfxInitProps* pProps, GfxHandle* pHandle);
void Graphics_DestroyGraphics(GfxHandle* pHandle);

bool Graphics_CreateHDRRenderTarget(const GfxHandle handle, u32 width, u32 height, GfxRenderTarget* pRenderTarget);
void Graphics_DestroyRenderTarget(GfxRenderTarget* pRenderTarget);