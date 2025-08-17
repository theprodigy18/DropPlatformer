#include "pch.h"
#include "Resources/Mesh.h"

bool Mesh_CreateVertexBuffer(const GfxHandle handle, const void* vertices, u32 verticesSize, ID3D11Buffer** ppVertexBuffer)
{
    DO_ASSERT_MSG(handle, "Graphics handle is null.");
    DO_ASSERT_MSG(vertices, "Pointer to vertices is null.");
    DO_ASSERT_MSG(ppVertexBuffer, "Pointer to vertex buffer is null.");

    ID3D11Device* pDevice = handle->pDevice;

    D3D11_BUFFER_DESC bufferDesc = {
        .ByteWidth           = verticesSize,
        .Usage               = D3D11_USAGE_DEFAULT,
        .BindFlags           = D3D11_BIND_VERTEX_BUFFER,
        .CPUAccessFlags      = 0,
        .MiscFlags           = 0,
        .StructureByteStride = 0};

    D3D11_SUBRESOURCE_DATA subresourceData = {
        .pSysMem          = vertices,
        .SysMemPitch      = 0,
        .SysMemSlicePitch = 0};

    ID3D11Buffer* pVertexBuffer = NULL;

    HRESULT hr = pDevice->lpVtbl->CreateBuffer(pDevice, &bufferDesc, &subresourceData, &pVertexBuffer);
    if (FAILED(hr) || !pVertexBuffer)
    {
        DO_ASSERT_MSG(false, "Failed to create vertex buffer.");
        return false;
    }

    *ppVertexBuffer = pVertexBuffer;

    return true;
}

bool Mesh_CreateIndexBuffer(const GfxHandle handle, const u32* indices, u32 numIndices, ID3D11Buffer** ppIndexBuffer)
{
    DO_ASSERT_MSG(handle, "Graphics handle is null.");
    DO_ASSERT_MSG(indices, "Pointer to indices is null.");
    DO_ASSERT_MSG(ppIndexBuffer, "Pointer to index buffer is null.");

    ID3D11Device* pDevice = handle->pDevice;

    D3D11_BUFFER_DESC bufferDesc = {
        .ByteWidth           = numIndices * sizeof(u32),
        .Usage               = D3D11_USAGE_DEFAULT,
        .BindFlags           = D3D11_BIND_INDEX_BUFFER,
        .CPUAccessFlags      = 0,
        .MiscFlags           = 0,
        .StructureByteStride = 0};

    D3D11_SUBRESOURCE_DATA subresourceData = {
        .pSysMem          = indices,
        .SysMemPitch      = 0,
        .SysMemSlicePitch = 0};

    ID3D11Buffer* pIndexBuffer = NULL;

    HRESULT hr = pDevice->lpVtbl->CreateBuffer(pDevice, &bufferDesc, &subresourceData, &pIndexBuffer);
    if (FAILED(hr) || !pIndexBuffer)
    {
        DO_ASSERT_MSG(false, "Failed to create index buffer.");
        return false;
    }

    *ppIndexBuffer = pIndexBuffer;

    return true;
}