#include "Line.h"

Line::Line()
{
}

Line::~Line()
{
}

void Line::VertexData(ID3D11Device* _device)
{
    //Commented out due to color not existing anymore, meaning that line will always give errors

    /*SimpleVertex LineList[] =
    {
        //position         //Color
        { XMFLOAT3(0,3,0), XMFLOAT4(1, 1, 1, 1) },
        { XMFLOAT3(0,4,0), XMFLOAT4(1, 1, 1, 1) },
    };

    //Line Vertex Initialization
    D3D11_BUFFER_DESC lineVertexBufferDesc = {};
    lineVertexBufferDesc.ByteWidth = sizeof(LineList);
    lineVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    lineVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA lineVertexData = { LineList };

    hr = _device->CreateBuffer(&lineVertexBufferDesc, &lineVertexData, &_lineVertexBuffer);
    if (FAILED(hr))
    {
        return hr;
    }*/
}

void Line::Draw(ID3D11DeviceContext* _immediateContext, ID3D11Buffer* _constantBuffer)
{
    //Set object variables and draw
    UINT stride = { sizeof(SimpleVertex) };
    UINT offset = 0;

    //Input Assembler (Sets it to be line list topology)
    _immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    _immediateContext->IASetVertexBuffers(0, 1, &_lineVertexBuffer, &stride, &offset);

    _immediateContext->Draw(2, 0);

    //Input Assembler (Resets it back to being triangle list topology)
    _immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
