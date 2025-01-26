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
        //Position                        //Normal                        //Texture Coords
        { XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT2(0.0f, 0.0f)},

        //front face
        //Position                        //Normal                        //Texture Coords                 
        { XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT2(0.0f, 0.0f)},

        //left face
        //Position                        //Normal                        //Texture Coords
        { XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT2(0.0f, 0.0f)},

        //right face
        //Position                        //Normal                        //Texture Coords                     
        { XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT2(0.0f, 0.0f)},

        //back face
        //Position                        //Normal                        //Texture Coords
        { XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT2(0.0f, 0.0f)},

        //bottom face
        //Position                        //Normal                        //Texture Coords                
        { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT2(0.0f, 0.0f)},
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
            //top
            3, 1, 0,
            2, 1, 3,

            //front
            5, 4, 6,
            6, 4, 7,

            //right
            11, 9, 8,
            10, 9, 11,

            //left
            13, 12, 14,
            14, 12, 15,

            //back
            17, 16, 18,
            18, 16, 19,

            //bottom
            21, 20, 22,
            22, 20, 23
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
        //top
        0, 1, 3,
        3, 1, 2,

        //front
        6, 4, 5,
        7, 4, 6,

        //right
        8, 9, 11,
        11, 9, 10,

        //left
        14, 12, 13,
        15, 12, 14,

        //back
        18, 16, 17,
        19, 16, 18,

        //bottom
        22, 20, 21,
        23, 20, 22
        };

        ZeroMemory(&bufferDesc, sizeof(bufferDesc));

        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(WORD) * 36;
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
        //Position                       //Normal                           
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.00f, -1.0f, -1.0f)},
        { XMFLOAT3(0.0f, 1.0f, 0.0f),    XMFLOAT3(0.0f, 1.0f, 0.0f)},
        { XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT3(1.0f, -1.0f, -1.0f)},

        //back
        //Position                       //Normal                            
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT3(-1.0f, -1.0f, 1.0f)},
        { XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT3(1.0f, -1.0f, 1.0f)},
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

        //right
        2, 1, 4,

        //left
        3, 1, 0,

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

    // Create vertex buffer
    SimpleVertex planeVertices[] =
    {
        //Position                      //Normal                      //Texture Coords         
        { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(0.0f, 5.0f) },
        { XMFLOAT3(1.0f, -1.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(5.0f, 5.0f) },
        { XMFLOAT3(1.0f, 1.0f, 0.0f),   XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(5.0f, 0.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(0.0f, 0.0f) },
    };

    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(SimpleVertex) * 4;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = planeVertices;

    _device->CreateBuffer(&bufferDesc, &InitData, &_vertexBuffer);

    // Create plane index buffer
    WORD planeIndices[] =
    {
        0, 3, 1,
        3, 2, 1,
    };

    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(WORD) * 6;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = planeIndices;
    _device->CreateBuffer(&bufferDesc, &InitData, &_indexBuffer);

    MeshData planeMesh;
    planeMesh.IndexBuffer = _indexBuffer;
    planeMesh.VertexBuffer = _vertexBuffer;
    planeMesh.IndexCount = 6;
    planeMesh.VBOffset = 0;
    planeMesh.VBStride = sizeof(SimpleVertex);

    return planeMesh;
}