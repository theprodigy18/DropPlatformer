#include "pch.h"
#include "Resources/Shader.h"

bool Shader_CreateVertexShader(
    const GfxHandle handle, const wchar_t* fileName,
    const D3D11_INPUT_ELEMENT_DESC* inputElementsDesc, u32 numInputElementDesc,
    ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppInputLayout)
{
    DO_ASSERT_MSG(handle, "Graphics handle is null.");
    DO_ASSERT_MSG(ppVertexShader, "Pointer to vertex shader is null.");
    DO_ASSERT_MSG(ppInputLayout, "Pointer to input layout is null.");

    ID3D11Device* pDevice = handle->pDevice;

    ID3DBlob* pByteCode = NULL;

    HRESULT hr = D3DReadFileToBlob(fileName, &pByteCode);
    if (FAILED(hr) || !pByteCode)
    {
        DO_ASSERT_MSG(false, "Failed to read file to blob.");
        return false;
    }

    ID3D11VertexShader* pVertexShader = NULL;

    hr = pDevice->lpVtbl->CreateVertexShader(pDevice, pByteCode->lpVtbl->GetBufferPointer(pByteCode), pByteCode->lpVtbl->GetBufferSize(pByteCode), NULL, &pVertexShader);
    if (FAILED(hr) || !pVertexShader)
    {
        DO_ASSERT_MSG(false, "Failed to create vertex shader.");
        DO_RELEASE(pByteCode);
        return false;
    }

    ID3D11InputLayout* pInputLayout = NULL;

    hr = pDevice->lpVtbl->CreateInputLayout(
        pDevice, inputElementsDesc, numInputElementDesc,
        pByteCode->lpVtbl->GetBufferPointer(pByteCode),
        pByteCode->lpVtbl->GetBufferSize(pByteCode), &pInputLayout);
    DO_RELEASE(pByteCode);
    if (FAILED(hr) || !pInputLayout)
    {
        DO_ASSERT_MSG(false, "Failed to create input layout.");
        DO_RELEASE(pVertexShader);
        return false;
    }

    *ppVertexShader = pVertexShader;
    *ppInputLayout  = pInputLayout;

    return true;
}

bool Shader_CreatePixelShader(const GfxHandle handle, const wchar_t* fileName, ID3D11PixelShader** ppPixelShader)
{
    DO_ASSERT_MSG(handle, "Graphics handle is null.");
    DO_ASSERT_MSG(ppPixelShader, "Pointer to pixel shader is null.");

    ID3D11Device* pDevice = handle->pDevice;

    ID3DBlob* pByteCode = NULL;

    HRESULT hr = D3DReadFileToBlob(fileName, &pByteCode);
    if (FAILED(hr) || !pByteCode)
    {
        DO_ASSERT_MSG(false, "Failed to read file to blob.");
        return false;
    }

    ID3D11PixelShader* pPixelShader = NULL;

    hr = pDevice->lpVtbl->CreatePixelShader(pDevice, pByteCode->lpVtbl->GetBufferPointer(pByteCode), pByteCode->lpVtbl->GetBufferSize(pByteCode), NULL, &pPixelShader);
    DO_RELEASE(pByteCode);
    if (FAILED(hr) || !pPixelShader)
    {
        DO_ASSERT_MSG(false, "Failed to create pixel shader.");
        return false;
    }

    *ppPixelShader = pPixelShader;

    return true;
}