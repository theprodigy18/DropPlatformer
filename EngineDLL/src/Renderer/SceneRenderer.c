#include "pch.h"
#include "Renderer/SceneRenderer.h"

#include "Resources/Shader.h"
#include "Resources/Mesh.h"

#define WORLD_WIDTH 32.0f
#define WORLD_HEIGHT 18.0f

#pragma region INTERNAL
typedef struct
{
    f32 pos[2];
    f32 uv[2];
} Vertex;

typedef struct
{
    f32 m[4][4];
} Mat4x4;

typedef struct _SceneRenderer
{
    ID3D11VertexShader* pVertexShader;
    ID3D11PixelShader*  pPixelShader;
    ID3D11InputLayout*  pInputLayout;

    ID3D11Buffer* pRectVB;
    ID3D11Buffer* pRectIB;

    ID3D11Buffer* pProjectionCB;

    GfxRenderTarget* pHDRRenderTarget;
} _SceneRenderer;
#pragma endregion

bool SceneRenderer_Initialize(GfxHandle handle, u32 width, u32 height, ISceneRenderer** ppSceneRenderer)
{
    DO_ASSERT_MSG(handle, "Graphics handle is null.");
    DO_ASSERT_MSG(ppSceneRenderer, "Pointer to scene renderer is null.");

    const wchar_t* vsFileName = L"assets/shaders/basic_vs.cso";

    D3D11_INPUT_ELEMENT_DESC inputElementsDesc[] = {
        {.SemanticName         = "POSITION",
         .SemanticIndex        = 0,
         .Format               = DXGI_FORMAT_R32G32_FLOAT,
         .InputSlot            = 0,
         .AlignedByteOffset    = 0,
         .InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA,
         .InstanceDataStepRate = 0},
        {.SemanticName         = "TEXCOORD",
         .SemanticIndex        = 0,
         .Format               = DXGI_FORMAT_R32G32_FLOAT,
         .InputSlot            = 0,
         .AlignedByteOffset    = 8,
         .InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA,
         .InstanceDataStepRate = 0}};

    ID3D11VertexShader* pVertexShader = NULL;
    ID3D11InputLayout*  pInputLayout  = NULL;

    if (!Shader_CreateVertexShader(
            handle, vsFileName, inputElementsDesc, ARRAYSIZE(inputElementsDesc),
            &pVertexShader, &pInputLayout) ||
        !pVertexShader || !pInputLayout)
    {
        DO_ASSERT_MSG(false, "Failed to create vertex shader or input layout.");
        return false;
    }

    const wchar_t* psFileName = L"assets/shaders/basic_ps.cso";

    ID3D11PixelShader* pPixelShader = NULL;

    if (!Shader_CreatePixelShader(handle, psFileName, &pPixelShader) || !pPixelShader)
    {
        DO_ASSERT_MSG(false, "Failed to create pixel shader.");
        DO_RELEASE(pInputLayout);
        DO_RELEASE(pVertexShader);
        return false;
    }

    // Mesh.
    Vertex vertices[] = {
        {.pos = {-0.5f, 0.5f}, .uv = {0.0f, 0.0f}},
        {.pos = {0.5f, 0.5f}, .uv = {1.0f, 0.0f}},
        {.pos = {0.5f, -0.5f}, .uv = {1.0f, 1.0f}},
        {.pos = {-0.5f, -0.5f}, .uv = {0.0f, 1.0f}}};

    ID3D11Buffer* pRectVB = NULL;

    if (!Mesh_CreateVertexBuffer(
            handle, (void*) vertices,
            sizeof(vertices), &pRectVB) ||
        !pRectVB)
    {
        DO_ASSERT_MSG(false, "Failed to create vertex buffer.");
        DO_RELEASE(pPixelShader);
        DO_RELEASE(pInputLayout);
        DO_RELEASE(pVertexShader);
        return false;
    }

    u32 indices[] = {0, 1, 2, 2, 3, 0};

    ID3D11Buffer* pRectIB = NULL;

    if (!Mesh_CreateIndexBuffer(handle, indices, ARRAYSIZE(indices), &pRectIB) || !pRectIB)
    {
        DO_ASSERT_MSG(false, "Failed to create index buffer.");
        DO_RELEASE(pRectVB);
        DO_RELEASE(pPixelShader);
        DO_RELEASE(pInputLayout);
        DO_RELEASE(pVertexShader);
        return false;
    }

    // HDR render target.
    GfxRenderTarget* pHDRRenderTarget = (GfxRenderTarget*) ArenaAllocator_Allocate(
        DO_PERSISTENT, sizeof(GfxRenderTarget));

    if (!Graphics_CreateHDRRenderTarget(handle, width, height, pHDRRenderTarget) ||
        !pHDRRenderTarget)
    {
        DO_ASSERT_MSG(false, "Failed to create HDR render target.");
        DO_RELEASE(pRectIB);
        DO_RELEASE(pRectVB);
        DO_RELEASE(pPixelShader);
        DO_RELEASE(pInputLayout);
        DO_RELEASE(pVertexShader);
        return false;
    }

    // Projection cbuffer.
    Mat4x4 projection = {
        .m = {
            {1.0f / WORLD_WIDTH, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f / WORLD_HEIGHT, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}}};

    D3D11_BUFFER_DESC projectionBufferDesc = {
        .ByteWidth           = sizeof(Mat4x4),
        .Usage               = D3D11_USAGE_DYNAMIC,
        .BindFlags           = D3D11_USAGE_DEFAULT,
        .CPUAccessFlags      = 0,
        .MiscFlags           = 0,
        .StructureByteStride = 0};

    D3D11_SUBRESOURCE_DATA projectionSubresourceData = {
        .pSysMem          = (void*) &projection,
        .SysMemPitch      = 0,
        .SysMemSlicePitch = 0};

    ID3D11Buffer* pProjectionCB = NULL;

    HRESULT hr = handle->pDevice->lpVtbl->CreateBuffer(
        handle->pDevice, &projectionBufferDesc, &projectionSubresourceData, &pProjectionCB);
    if (FAILED(hr) || !pProjectionCB)
    {
        DO_ASSERT_MSG(false, "Failed to create projection cbuffer.");
        Graphics_DestroyRenderTarget(pHDRRenderTarget);
        DO_RELEASE(pRectIB);
        DO_RELEASE(pRectVB);
        DO_RELEASE(pPixelShader);
        DO_RELEASE(pInputLayout);
        DO_RELEASE(pVertexShader);
        return false;
    }

    ISceneRenderer* pSceneRenderer = (ISceneRenderer*) ArenaAllocator_Allocate(
        DO_PERSISTENT, sizeof(ISceneRenderer));
    if (!pSceneRenderer)
    {
        DO_ERROR("Failed to allocate memory for scene renderer.");
        Graphics_DestroyRenderTarget(pHDRRenderTarget);
        DO_RELEASE(pProjectionCB);
        DO_RELEASE(pRectIB);
        DO_RELEASE(pRectVB);
        DO_RELEASE(pPixelShader);
        DO_RELEASE(pInputLayout);
        DO_RELEASE(pVertexShader);
        return false;
    }

    handle->pContext->lpVtbl->VSSetConstantBuffers(handle->pContext, 0, 1, &pProjectionCB);

    pSceneRenderer->pVertexShader    = pVertexShader;
    pSceneRenderer->pPixelShader     = pPixelShader;
    pSceneRenderer->pInputLayout     = pInputLayout;
    pSceneRenderer->pRectVB          = pRectVB;
    pSceneRenderer->pRectIB          = pRectIB;
    pSceneRenderer->pProjectionCB    = pProjectionCB;
    pSceneRenderer->pHDRRenderTarget = pHDRRenderTarget;

    *ppSceneRenderer = pSceneRenderer;

    return true;
}

void SceneRenderer_Shutdown(ISceneRenderer** ppSceneRenderer)
{
    DO_ASSERT_MSG(ppSceneRenderer && *ppSceneRenderer, "Pointer to scene renderer is null.");
    ISceneRenderer* pSceneRenderer = *ppSceneRenderer;

    if (pSceneRenderer)
    {
        DO_RELEASE(pSceneRenderer->pProjectionCB);
        DO_SAFE_RELEASE(pSceneRenderer->pRectIB);
        DO_SAFE_RELEASE(pSceneRenderer->pRectVB);
        DO_SAFE_RELEASE(pSceneRenderer->pPixelShader);
        DO_SAFE_RELEASE(pSceneRenderer->pInputLayout);
        DO_SAFE_RELEASE(pSceneRenderer->pVertexShader);
        Graphics_DestroyRenderTarget(pSceneRenderer->pHDRRenderTarget);

        pSceneRenderer->pHDRRenderTarget = NULL;
        pSceneRenderer->pProjectionCB    = NULL;
        pSceneRenderer->pRectIB          = NULL;
        pSceneRenderer->pRectVB          = NULL;
        pSceneRenderer->pPixelShader     = NULL;
        pSceneRenderer->pInputLayout     = NULL;
        pSceneRenderer->pVertexShader    = NULL;
    }

    *ppSceneRenderer = NULL;
}

void SceneRenderer_DrawScene(GfxHandle handle, ISceneRenderer* pSceneRenderer, D3D11_VIEWPORT* pViewport)
{
    DO_ASSERT_MSG(handle, "Graphics handle is null.");

    handle->pContext->lpVtbl->RSSetViewports(handle->pContext, 1, pViewport);
    // Clearing screen.
    f32 clearColor[] = {0.1f, 0.2f, 0.5f, 1.0f};
    handle->pContext->lpVtbl->OMSetRenderTargets(handle->pContext, 1, &handle->pBackBufferRTV, NULL);
    handle->pContext->lpVtbl->ClearRenderTargetView(handle->pContext, handle->pBackBufferRTV, clearColor);

    // Set shaders.
    handle->pContext->lpVtbl->VSSetShader(handle->pContext, pSceneRenderer->pVertexShader, NULL, 0);
    handle->pContext->lpVtbl->PSSetShader(handle->pContext, pSceneRenderer->pPixelShader, NULL, 0);

    // Set input layout.
    handle->pContext->lpVtbl->IASetInputLayout(handle->pContext, pSceneRenderer->pInputLayout);

    // Set vertex and index buffer.
    u32 vertexSize = sizeof(Vertex);
    u32 offset     = 0;
    handle->pContext->lpVtbl->IASetVertexBuffers(handle->pContext, 0, 1, &pSceneRenderer->pRectVB, &vertexSize, &offset);
    handle->pContext->lpVtbl->IASetIndexBuffer(handle->pContext, pSceneRenderer->pRectIB, DXGI_FORMAT_R32_UINT, 0);

    // Draw.
    handle->pContext->lpVtbl->IASetPrimitiveTopology(handle->pContext, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    handle->pContext->lpVtbl->DrawIndexed(handle->pContext, 6, 0, 0);
}
