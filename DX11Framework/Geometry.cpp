#include "Geometry.h"

Geometry::Geometry()
{
    _indexBuffer = nullptr;
    _vertexBuffer = nullptr;
    _noOfIndices = 0;
    _isInverted = false;
}

Geometry::~Geometry()
{
    _indexBuffer = nullptr;
    _vertexBuffer = nullptr;
}

MeshData Geometry::Cube(ID3D11Device* _device, bool inverted)
{
    D3D11_BUFFER_DESC bufferDesc;
    D3D11_SUBRESOURCE_DATA InitData;

    SimpleVertex CubeVertices[] =
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
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(SimpleVertex) * 24;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = CubeVertices;

    _device->CreateBuffer(&bufferDesc, &InitData, &_vertexBuffer);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    if (_isInverted)
    {
        WORD CubeIndices[] =
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

        //Cube Index Initialization
        //ByteWidth gets the length of the indices and reserves the correct amount of data for it to be loaded
        bufferDesc.ByteWidth = sizeof(SimpleVertex);
        //immutable means unchangable, meaning that this data cant be altered during runtime
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        //this tells the GPU that its specifically an index buffer, and so it uses the correct specifications for indicies
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;

        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = CubeIndices;

        _device->CreateBuffer(&bufferDesc, &InitData, &_indexBuffer);
    }
    else if (!_isInverted)
    {
        WORD CubeIndices[] =
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

        //Cube Index Initialization
        bufferDesc.ByteWidth = sizeof(SimpleVertex);
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;

        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = CubeIndices;

        _device->CreateBuffer(&bufferDesc, &InitData, &_indexBuffer);
    }

    MeshData cubeMesh;
    cubeMesh.IndexBuffer = _indexBuffer;
    cubeMesh.VertexBuffer = _vertexBuffer;
    cubeMesh.IndexCount = 36;
    cubeMesh.VBOffset = 0;
    cubeMesh.VBStride = sizeof(SimpleVertex);

    return cubeMesh;
}

MeshData Geometry::Pyramid(ID3D11Device* _device)
{
    D3D11_BUFFER_DESC bufferDesc;
    D3D11_SUBRESOURCE_DATA InitData;

    SimpleVertex PyramidVertices[] =
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
    bufferDesc.ByteWidth = sizeof(SimpleVertex);
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = PyramidVertices;

    _device->CreateBuffer(&bufferDesc, &InitData, &_indexBuffer);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    WORD PyramidIndices[] =
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

    //Pyramid Index Initialization
    bufferDesc.ByteWidth = sizeof(SimpleVertex);
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = PyramidIndices;

    _device->CreateBuffer(&bufferDesc, &InitData, &_indexBuffer);

    MeshData pyramidMesh;
    pyramidMesh.IndexBuffer = _indexBuffer;
    pyramidMesh.VertexBuffer = _vertexBuffer;
    pyramidMesh.IndexCount = 18;
    pyramidMesh.VBOffset = 0;
    pyramidMesh.VBStride = sizeof(SimpleVertex);

    return pyramidMesh;
}

MeshData Geometry::Plane(ID3D11Device* _device)
{
    D3D11_BUFFER_DESC bufferDesc;
    D3D11_SUBRESOURCE_DATA InitData;

    SimpleVertex PlaneVertices[] =
    {
        { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 5.0f) },
        { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(5.0f, 5.0f) },
        { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(5.0f, 0.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
    };

    //Plane Vertex Initialization
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(SimpleVertex) * 4;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = PlaneVertices;

    _device->CreateBuffer(&bufferDesc, &InitData, &_indexBuffer);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    WORD PlaneIndices[] =
    {
        0, 3, 1,
        3, 2, 1,
    };

    //Plane Index Initialization
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(WORD) * 6;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = PlaneIndices;

    _device->CreateBuffer(&bufferDesc, &InitData, &_indexBuffer);

    MeshData planeMesh;
    planeMesh.IndexBuffer = _indexBuffer;
    planeMesh.VertexBuffer = _vertexBuffer;
    planeMesh.IndexCount = 6;
    planeMesh.VBOffset = 0;
    planeMesh.VBStride = sizeof(SimpleVertex);

    return planeMesh;
}