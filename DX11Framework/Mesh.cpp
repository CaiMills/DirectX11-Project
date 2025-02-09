#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::Mesh(MeshData* meshData) : _meshData(meshData)
{

}

Mesh::~Mesh()
{
    _indexBuffer = nullptr;
    _vertexBuffer = nullptr;
}

MeshData* Mesh::CreateCube()
{
    D3D11_BUFFER_DESC bufferDesc;
    D3D11_SUBRESOURCE_DATA InitData;

    SimpleVertex CubeVertices[] =
    {
        // Top face
        // Position                       // Normal                       // Texture Coords
        { XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT2(0.0f, 0.0f)},

        // Front face
        // Position                       // Normal                       // Texture Coords                 
        { XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT2(0.0f, 0.0f)},

        // Left face
        // Position                       // Normal                       // Texture Coords
        { XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT2(0.0f, 0.0f)},

        // Right face
        // Position                       // Normal                       // Texture Coords                     
        { XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT2(0.0f, 0.0f)},

        // Back face
        // Position                       // Normal                       // Texture Coords
        { XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT2(0.0f, 0.0f)},

        // Bottom face
        // Position                       // Normal                       // Texture Coords                
        { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT2(0.0f, 0.0f)},
    };

    // Cube Vertex Initialization
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(SimpleVertex) * 24;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = CubeVertices;

    _device->CreateBuffer(&bufferDesc, &InitData, &_vertexBuffer);

    // Find the min and max variables
    for (size_t i = 0; i < 24; i++)
    {
        XMFLOAT3& pos = CubeVertices[i].Position;

        //Update min and max values
        _min.x = min(_min.x, pos.x);
        _min.y = min(_min.y, pos.y);
        _min.z = min(_min.z, pos.z);

        _max.x = max(_max.x, pos.x);
        _max.y = max(_max.y, pos.y);
        _max.z = max(_max.z, pos.z);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    WORD CubeIndices[] =
    {
        // Top
        0, 1, 3,
        3, 1, 2,

        // Front
        6, 4, 5,
        7, 4, 6,

        // Right
        8, 9, 11,
        11, 9, 10,

        // Left
        14, 12, 13,
        15, 12, 14,

        // Back
        18, 16, 17,
        19, 16, 18,

        // Bottom
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

    // Creates the Mesh
    _meshData->IndexBuffer = _indexBuffer;
    _meshData->VertexBuffer = _vertexBuffer;
    _meshData->IndexCount = 36;
    _meshData->VBOffset = 0;
    _meshData->VBStride = sizeof(SimpleVertex);

    return _meshData;
}

MeshData* Mesh::CreateInvertedCube()
{
    D3D11_BUFFER_DESC bufferDesc;
    D3D11_SUBRESOURCE_DATA InitData;

    SimpleVertex CubeVertices[] =
    {
        // Top face
        // Position                       // Normal                       // Texture Coords
        { XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT2(0.0f, 0.0f)},

        // Front face
        // Position                       // Normal                       // Texture Coords                 
        { XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT2(0.0f, 0.0f)},

        // Left face
        // Position                       // Normal                       // Texture Coords
        { XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT2(0.0f, 0.0f)},

        // Right face
        // Position                       // Normal                       // Texture Coords                     
        { XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT2(0.0f, 0.0f)},

        // Back face
        // Position                       // Normal                       // Texture Coords
        { XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT2(0.0f, 0.0f)},

        // Bottom face
        // Position                       // Normal                       // Texture Coords                
        { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT2(0.0f, 0.0f)},
    };

    // Cube Vertex Initialization
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(SimpleVertex) * 24;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = CubeVertices;

    _device->CreateBuffer(&bufferDesc, &InitData, &_vertexBuffer);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    WORD CubeIndices[] =
    {
        // Top
        3, 1, 0,
        2, 1, 3,

        // Front
        5, 4, 6,
        6, 4, 7,

        // Right
        11, 9, 8,
        10, 9, 11,

        // Left
        13, 12, 14,
        14, 12, 15,

        // Back
        17, 16, 18,
        18, 16, 19,

        // Bottom
        21, 20, 22,
        22, 20, 23
    };

    ZeroMemory(&bufferDesc, sizeof(bufferDesc));

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(WORD) * 36;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = CubeIndices;

    _device->CreateBuffer(&bufferDesc, &InitData, &_indexBuffer);

    // Creates the Mesh
    _meshData->IndexBuffer = _indexBuffer;
    _meshData->VertexBuffer = _vertexBuffer;
    _meshData->IndexCount = 36;
    _meshData->VBOffset = 0;
    _meshData->VBStride = sizeof(SimpleVertex);

    return _meshData;
}

MeshData* Mesh::CreatePyramid()
{
    D3D11_BUFFER_DESC bufferDesc;
    D3D11_SUBRESOURCE_DATA InitData;

    SimpleVertex PyramidVertices[] =
    {
        // Front
        // Position                      // Normal                           
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.00f, -1.0f, -1.0f)},
        { XMFLOAT3(0.0f, 1.0f, 0.0f),    XMFLOAT3(0.0f, 1.0f, 0.0f)},
        { XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT3(1.0f, -1.0f, -1.0f)},

        // Back
        // Position                      // Normal                            
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT3(-1.0f, -1.0f, 1.0f)},
        { XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT3(1.0f, -1.0f, 1.0f)},
    };

    // Pyramid Vertex Initialization
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
        // Front
        0, 1, 2,

        // Right
        2, 1, 4,

        // Left
        3, 1, 0,

        // Back
        3, 4, 1,

        // Bottom
        3, 0, 2,
        2, 4, 3,
    };

    // Pyramid Index Initialization
    bufferDesc.ByteWidth = sizeof(SimpleVertex);
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = PyramidIndices;

    _device->CreateBuffer(&bufferDesc, &InitData, &_indexBuffer);

    // Creates the Mesh
    _meshData->IndexBuffer = _indexBuffer;
    _meshData->VertexBuffer = _vertexBuffer;
    _meshData->IndexCount = 18;
    _meshData->VBOffset = 0;
    _meshData->VBStride = sizeof(SimpleVertex);

    return _meshData;
}

MeshData* Mesh::CreatePlane()
{
    D3D11_BUFFER_DESC bufferDesc;
    D3D11_SUBRESOURCE_DATA InitData;

    // Create vertex buffer
    SimpleVertex planeVertices[] =
    {
        // Position                     // Normal                     // Texture Coords         
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

    // Creates the Mesh
    _meshData->IndexBuffer = _indexBuffer;
    _meshData->VertexBuffer = _vertexBuffer;
    _meshData->IndexCount = 6;
    _meshData->VBOffset = 0;
    _meshData->VBStride = sizeof(SimpleVertex);

    return _meshData;
}

// This currently doesnt work
void Mesh::SetMinAndMax()
{

}