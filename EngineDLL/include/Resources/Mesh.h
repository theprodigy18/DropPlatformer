#pragma once

#include "Graphics/Graphics.h"

bool Mesh_CreateVertexBuffer(const GfxHandle handle, const void* vertices, u32 verticesSize, ID3D11Buffer** ppVertexBuffer);
bool Mesh_CreateIndexBuffer(const GfxHandle handle, const u32* indices, u32 numIndices, ID3D11Buffer** ppIndexBuffer);