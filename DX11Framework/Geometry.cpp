#include "Geometry.h"

Geometry::Geometry()
{
    _texture = nullptr;
	_transform = new Transform();
    _indexBuffer = nullptr;
    _vertexBuffer = nullptr;
    _noOfIndices = 0;
    _isInverted = false;
}

Geometry::~Geometry()
{
    _texture = nullptr;
    _transform = nullptr;
    _indexBuffer = nullptr;
    _vertexBuffer = nullptr;
}

void Geometry::CubeData(ID3D11Device* _device, bool inverted)
{
    if (_isInverted)
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

        _device->CreateBuffer(&cubeIndexBufferDesc, &cubeIndexData, &_indexBuffer);
    }
    else if (!_isInverted)
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

        _device->CreateBuffer(&cubeIndexBufferDesc, &cubeIndexData, &_indexBuffer);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

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

    _device->CreateBuffer(&cubeVertexBufferDesc, &cubeVertexData, &_vertexBuffer);

    _noOfIndices = 36;
}

void Geometry::PyramidData(ID3D11Device* _device)
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

    _device->CreateBuffer(&pyramidIndexBufferDesc, &pyramidIndexData, &_indexBuffer);

    ///////////////////////////////////////////////////////////////////////////////////////////////

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

    _device->CreateBuffer(&pyramidVertexBufferDesc, &pyramidVertexData, &_vertexBuffer);

    _noOfIndices = 18;
}

void Geometry::PlaneData(ID3D11Device* _device)
{
}

void Geometry::Draw(ID3D11DeviceContext* _immediateContext)
{
    //Set object variables and draw
    UINT stride = { sizeof(SimpleVertex) };
    UINT offset = 0;

    _immediateContext->PSSetShaderResources(0, 1, GetTexture());

    //Loads the buffer information for the cube
    _immediateContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
    _immediateContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

    //this draws the indicies, and must have enough space for all three index points of a triangle to work
    _immediateContext->DrawIndexed(_noOfIndices, 0, 0);
}
