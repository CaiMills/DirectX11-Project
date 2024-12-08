#include "Pyramid.h"

Pyramid::Pyramid()
{

}

Pyramid::~Pyramid()
{

}

void Pyramid::IndexData(ID3D11Device* _device)
{
    WORD PyramidIndexData[] =
    {
        //Indices
        //front
        0, 1, 2,

        //left
        3, 1, 0,

        //right
        2, 1, 4,

        //back
        3, 4, 1,

        //bottom
        3, 0, 2,
        2, 4, 3,
    };

    D3D11_BUFFER_DESC pyramidIndexBufferDesc = {};
    //ByteWidth gets the length of the indices and reserves the correct amount of data for it to be loaded
    pyramidIndexBufferDesc.ByteWidth = sizeof(PyramidIndexData);
    //immutable means unchangable, meaning that this data cant be altered during runtime
    pyramidIndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    //this tells the GPU that its specifically an index buffer, and so it uses the correct specifications for indicies
    pyramidIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA pyramidIndexData = { PyramidIndexData };

    _device->CreateBuffer(&pyramidIndexBufferDesc, &pyramidIndexData, &_pyramidIndexBuffer);
}

void Pyramid::VertexData(ID3D11Device* _device)
{
    SimpleVertex PyramidVertexData[] =
    {
        //front
        //Position                           //Normal                           
        { XMFLOAT3(-1.00f, -1.00f, -1.00f), XMFLOAT3(-1.00f, -1.00f, -1.00f)},
        { XMFLOAT3(0.00f, 1.00f, 0.00f),    XMFLOAT3(0.00f, 1.00f, 0.00f)},
        { XMFLOAT3(1.00f, -1.00f, -1.00f),   XMFLOAT3(1.00f, -1.00f, -1.00f)},

        //back
        //Position                          //Normal                            
        { XMFLOAT3(-1.00f, -1.00f, 1.00f), XMFLOAT3(-1.00f, -1.00f, 1.00f)},
        { XMFLOAT3(1.00f, -1.00f, 1.00f),   XMFLOAT3(1.00f, -1.00f, 1.00f)},
    };

    //Pyramid Vertex Initialization
    D3D11_BUFFER_DESC pyramidVertexBufferDesc = {};
    pyramidVertexBufferDesc.ByteWidth = sizeof(PyramidVertexData);
    pyramidVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    pyramidVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA pyramidVertexData = { PyramidVertexData };

    _device->CreateBuffer(&pyramidVertexBufferDesc, &pyramidVertexData, &_pyramidVertexBuffer);
}

void Pyramid::Draw(ID3D11DeviceContext* _immediateContext, ID3D11Buffer* _constantBuffer)
{
	//Set object variables and draw
	UINT stride = { sizeof(SimpleVertex) };
	UINT offset = 0;

	_immediateContext->IASetVertexBuffers(0, 1, &_pyramidVertexBuffer, &stride, &offset);
	_immediateContext->IASetIndexBuffer(_pyramidIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	_immediateContext->DrawIndexed(18, 0, 0);
}
