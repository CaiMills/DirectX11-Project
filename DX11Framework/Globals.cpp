#include "Globals.h"

ConstantBuffer _cbData = {};
ID3D11Buffer* _constantBuffer = nullptr;
ID3D11DeviceContext* _immediateContext = nullptr;
ID3D11Device* _device = nullptr;