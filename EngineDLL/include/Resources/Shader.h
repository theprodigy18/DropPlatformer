#pragma once

#include "Graphics/Graphics.h"

bool Shader_CreateVertexShader(
    const GfxHandle handle, const wchar_t* fileName,
    const D3D11_INPUT_ELEMENT_DESC* inputElementsDesc, u32 numInputElementDesc,
    ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppInputLayout);

bool Shader_CreatePixelShader(const GfxHandle handle, const wchar_t* fileName, ID3D11PixelShader** ppPixelShader);