#include "pch.h"
#include "Graphics/Graphics.h"

bool Graphics_CreateGraphics(const GfxInitProps* pProps, GfxHandle* pHandle)
{
    DO_ASSERT_MSG(pProps, "Pointer to properties is null.");
    DO_ASSERT_MSG(pHandle, "Pointer to handle is null.");

    *pHandle = NULL;

    WndHandle wndHandle = pProps->wndHandle;

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {
        .BufferCount                        = 2,
        .BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM,
        .BufferDesc.Width                   = wndHandle->width,
        .BufferDesc.Height                  = wndHandle->height,
        .BufferDesc.RefreshRate.Numerator   = 60,
        .BufferDesc.RefreshRate.Denominator = 1,
        .BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .SampleDesc.Count                   = 1,
        .SampleDesc.Quality                 = 0,
        .SwapEffect                         = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        .OutputWindow                       = wndHandle->hwnd,
        .Windowed                           = TRUE,
        .Flags                              = 0};

    u32 flags = 0;
    DO_DEBUG_OP(
        flags |= D3D11_CREATE_DEVICE_DEBUG;)

    D3D_FEATURE_LEVEL featureLevel;

    ID3D11Device*        pDevice    = NULL;
    ID3D11DeviceContext* pContext   = NULL;
    IDXGISwapChain*      pSwapChain = NULL;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
        flags, NULL, 0, D3D11_SDK_VERSION,
        &swapChainDesc, &pSwapChain,
        &pDevice, &featureLevel, &pContext);

    if (FAILED(hr) || !pDevice || !pContext || !pSwapChain)
    {
        DO_ASSERT_MSG(false, "Failed to create device and swap chain.");
        return false;
    }

    DO_DEBUG_OP(
        ID3D11InfoQueue* pInfoQueue = NULL;

        hr = pDevice->lpVtbl->QueryInterface(pDevice, &IID_ID3D11InfoQueue, (void**) &pInfoQueue);
        if (SUCCEEDED(hr) && pInfoQueue) {
            pInfoQueue->lpVtbl->SetBreakOnSeverity(pInfoQueue, D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
            pInfoQueue->lpVtbl->SetBreakOnSeverity(pInfoQueue, D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
            pInfoQueue->lpVtbl->SetBreakOnSeverity(pInfoQueue, D3D11_MESSAGE_SEVERITY_WARNING, TRUE);
            DO_RELEASE(pInfoQueue);
        })

    ID3D11Buffer* pBackBuffer = NULL;

    hr = pSwapChain->lpVtbl->GetBuffer(pSwapChain, 0, &IID_ID3D11Texture2D, (void**) &pBackBuffer);
    if (FAILED(hr) || !pBackBuffer)
    {
        DO_ASSERT_MSG(false, "Failed to get back buffer from swap chain.");
        DO_RELEASE(pSwapChain);
        DO_RELEASE(pContext);
        DO_RELEASE(pDevice);
        return false;
    }

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {
        .Format             = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
        .ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D,
        .Texture2D.MipSlice = 0};

    ID3D11RenderTargetView* pRTV = NULL;

    hr = pDevice->lpVtbl->CreateRenderTargetView(pDevice, (ID3D11Resource*) pBackBuffer, &rtvDesc, &pRTV);
    DO_RELEASE(pBackBuffer);
    if (FAILED(hr) || !pRTV)
    {
        DO_ASSERT_MSG(false, "Failed to create render target view.");
        DO_RELEASE(pSwapChain);
        DO_RELEASE(pContext);
        DO_RELEASE(pDevice);
        return false;
    }

    GfxHandle handle = (GfxHandle) ArenaAllocator_Allocate(DO_PERSISTENT, sizeof(_GfxHandle));
    if (!handle)
    {
        DO_ASSERT_MSG(false, "Failed to allocate memory for graphics handle.");
        DO_RELEASE(pRTV);
        DO_RELEASE(pSwapChain);
        DO_RELEASE(pContext);
        DO_RELEASE(pDevice);
        return false;
    }

    handle->pDevice        = pDevice;
    handle->pContext       = pContext;
    handle->pSwapChain     = pSwapChain;
    handle->pBackBufferRTV = pRTV;

    *pHandle = handle;

    return true;
}

void Graphics_DestroyGraphics(GfxHandle* pHandle)
{
    DO_ASSERT_MSG(pHandle && *pHandle, "Pointer to handle or handle itself is null.");
    GfxHandle handle = *pHandle;

    if (handle)
    {
        DO_SAFE_RELEASE(handle->pBackBufferRTV);
        DO_SAFE_RELEASE(handle->pSwapChain);
        DO_SAFE_RELEASE(handle->pContext);
        DO_SAFE_RELEASE(handle->pDevice);

        handle->pDevice        = NULL;
        handle->pContext       = NULL;
        handle->pSwapChain     = NULL;
        handle->pBackBufferRTV = NULL;
    }

    *pHandle = NULL;
}