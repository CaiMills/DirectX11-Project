#include "Cube.h"

Cube::Cube()
{

}

Cube::~Cube()
{

}

void Cube::IndexData(ID3D11Device* _device, bool inverted)
{
    if (inverted)
    {
        WORD CubeIndexData[] =
        {
            //top face
            22, 21, 20,
            20, 23, 22,

            //front face
            6, 5, 4,
            4, 7, 6,

            //left face
            10, 9, 8,
            8, 11, 10,

            //right face
            14, 13, 12,
            12, 15, 14,

            //back face
            2, 1, 0,
            0, 3, 2,

            //bottom face
            18, 17, 16,
            16, 19, 18,
        };

        D3D11_BUFFER_DESC cubeIndexBufferDesc = {};
        //ByteWidth gets the length of the indices and reserves the correct amount of data for it to be loaded
        cubeIndexBufferDesc.ByteWidth = sizeof(CubeIndexData);
        //immutable means unchangable, meaning that this data cant be altered during runtime
        cubeIndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        //this tells the GPU that its specifically an index buffer, and so it uses the correct specifications for indicies
        cubeIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA cubeIndexData = { CubeIndexData };

        _device->CreateBuffer(&cubeIndexBufferDesc, &cubeIndexData, &_cubeIndexBuffer);
    }
    else if (!inverted)
    {
        WORD CubeIndexData[] =
        {
            //top face
            20, 21, 22,
            22, 23, 20,

            //front face
            4, 5, 6,
            6, 7, 4,

            //left face
            8, 9, 10,
            10, 11, 8,

            //right face
            12, 13, 14,
            14, 15, 12,

            //back face
            0, 1, 2,
            2, 3, 0,

            //bottom face
            16, 17, 18,
            18, 19, 16,
        };

        D3D11_BUFFER_DESC cubeIndexBufferDesc = {};
        //ByteWidth gets the length of the indices and reserves the correct amount of data for it to be loaded
        cubeIndexBufferDesc.ByteWidth = sizeof(CubeIndexData);
        //immutable means unchangable, meaning that this data cant be altered during runtime
        cubeIndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        //this tells the GPU that its specifically an index buffer, and so it uses the correct specifications for indicies
        cubeIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA cubeIndexData = { CubeIndexData };

        _device->CreateBuffer(&cubeIndexBufferDesc, &cubeIndexData, &_cubeIndexBuffer);
    }
}

void Cube::VertexData(ID3D11Device* _device)
{
    SimpleVertex CubeVertexData[] =
    {
        //top face
        //Position                           //Normal                           //Texture Coords
        { XMFLOAT3(1.00f,  1.00f, 1.00f),    XMFLOAT3(1.00f,  1.00f, 1.00f),    XMFLOAT2(1.00f, 0.00f)},
        { XMFLOAT3(1.00f,  1.00f, -1.00f),   XMFLOAT3(1.00f,  1.00f, -1.00f),   XMFLOAT2(1.00f, 1.00f)},
        { XMFLOAT3(-1.00f, 1.00f, -1.00f),   XMFLOAT3(-1.00f, 1.00f, -1.00f),   XMFLOAT2(0.00f, 1.00f)},
        { XMFLOAT3(-1.00f, 1.00f, 1.00f),    XMFLOAT3(-1.00f, 1.00f, 1.00f),    XMFLOAT2(0.00f, 0.00f)},

        //front face
        //Position                           //Normal                           //Texture Coords                  
        { XMFLOAT3(1.00f,  -1.00f, -1.00f),  XMFLOAT3(1.00f,  -1.00f, -1.00f),  XMFLOAT2(1.00f, 0.00f)},
        { XMFLOAT3(-1.00f,  -1.00f, -1.00f), XMFLOAT3(-1.00f,  -1.00f, -1.00f), XMFLOAT2(1.00f, 1.00f)},
        { XMFLOAT3(-1.00f, 1.00f, -1.00f),   XMFLOAT3(-1.00f, 1.00f, -1.00f),   XMFLOAT2(0.00f, 1.00f)},
        { XMFLOAT3(1.00f, 1.00f, -1.00f),    XMFLOAT3(1.00f, 1.00f, -1.00f),    XMFLOAT2(0.00f, 0.00f)},

        //left face
        //Position                           //Normal                           //Texture Coords
        { XMFLOAT3(1.00f,  1.00f, 1.00f),    XMFLOAT3(1.00f,  1.00f, 1.00f),    XMFLOAT2(1.00f, 0.00f)},
        { XMFLOAT3(1.00f,  -1.00f, 1.00f),   XMFLOAT3(1.00f,  -1.00f, 1.00f),   XMFLOAT2(1.00f, 1.00f)},
        { XMFLOAT3(1.00f, -1.00f, -1.00f),   XMFLOAT3(1.00f, -1.00f, -1.00f),   XMFLOAT2(0.00f, 1.00f)},
        { XMFLOAT3(1.00f, 1.00f, -1.00f),    XMFLOAT3(1.00f, 1.00f, -1.00f),    XMFLOAT2(0.00f, 0.00f)},

        //right face
        //Position                           //Normal                           //Texture Coords                      
        { XMFLOAT3(-1.00f,  1.00f, 1.00f),   XMFLOAT3(-1.00f,  1.00f, 1.00f),   XMFLOAT2(1.00f, 0.00f)},
        { XMFLOAT3(-1.00f,  1.00f, -1.00f),  XMFLOAT3(-1.00f,  1.00f, -1.00f),  XMFLOAT2(1.00f, 1.00f)},
        { XMFLOAT3(-1.00f, -1.00f, -1.00f),  XMFLOAT3(-1.00f, -1.00f, -1.00f),  XMFLOAT2(0.00f, 1.00f)},
        { XMFLOAT3(-1.00f, -1.00f, 1.00f),   XMFLOAT3(-1.00f, -1.00f, 1.00f),   XMFLOAT2(0.00f, 0.00f)},

        //back face
        //Position                           //Normal                           //Texture Coords
        { XMFLOAT3(1.00f,  1.00f, 1.00f),    XMFLOAT3(1.00f,  1.00f, 1.00f),    XMFLOAT2(1.00f, 0.00f)},
        { XMFLOAT3(-1.00f,  1.00f, 1.00f),   XMFLOAT3(-1.00f,  1.00f, 1.00f),   XMFLOAT2(1.00f, 1.00f)},
        { XMFLOAT3(-1.00f, -1.00f, 1.00f),   XMFLOAT3(-1.00f, -1.00f, 1.00f),   XMFLOAT2(0.00f, 1.00f)},
        { XMFLOAT3(1.00f, -1.00f, 1.00f),    XMFLOAT3(1.00f, -1.00f, 1.00f),    XMFLOAT2(0.00f, 0.00f)},

        //bottom face
        //Position                           //Normal                           //Texture Coords                 
        { XMFLOAT3(-1.00f,  -1.00f, -1.00f), XMFLOAT3(-1.00f,  -1.00f, -1.00f), XMFLOAT2(1.00f, 0.00f)},
        { XMFLOAT3(1.00f,  -1.00f, -1.00f),  XMFLOAT3(1.00f,  -1.00f, -1.00f),  XMFLOAT2(1.00f, 1.00f)},
        { XMFLOAT3(1.00f, -1.00f, 1.00f),    XMFLOAT3(1.00f, -1.00f, 1.00f),    XMFLOAT2(0.00f, 1.00f)},
        { XMFLOAT3(-1.00f, -1.00f, 1.00f),   XMFLOAT3(-1.00f, -1.00f, 1.00f),   XMFLOAT2(0.00f, 0.00f)},
    };

    //Cube Vertex Initialization
    D3D11_BUFFER_DESC cubeVertexBufferDesc = {};
    cubeVertexBufferDesc.ByteWidth = sizeof(CubeVertexData);
    cubeVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    cubeVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA cubeVertexData = { CubeVertexData };

    _device->CreateBuffer(&cubeVertexBufferDesc, &cubeVertexData, &_cubeVertexBuffer);
}

void Cube::Draw(ID3D11DeviceContext* _immediateContext, ID3D11Buffer* _constantBuffer)
{
    //Set object variables and draw
    UINT stride = { sizeof(SimpleVertex) };
    UINT offset = 0;

    _immediateContext->PSSetShaderResources(0, 1, GetShaderResource());

    //Loads the buffer information for the cube
    _immediateContext->IASetVertexBuffers(0, 1, &_cubeVertexBuffer, &stride, &offset);
    _immediateContext->IASetIndexBuffer(_cubeIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    //this draws the indicies, and must have enough space for all three index points of a triangle to work
    _immediateContext->DrawIndexed(36, 0, 0);
}
