#pragma once
#include <d3d11.h>
#include "Structures.h"

using namespace DirectX;

extern ConstantBuffer _cbData;
extern ID3D11Buffer* _constantBuffer;
extern ID3D11DeviceContext* _immediateContext;
extern ID3D11Device* _device;