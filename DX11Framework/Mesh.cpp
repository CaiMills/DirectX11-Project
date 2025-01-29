#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::Mesh(MeshData* meshData) : _meshData(meshData)
{
    SetMinAndMax();
}

Mesh::~Mesh()
{
    _indexBuffer = nullptr;
    _vertexBuffer = nullptr;
}

MeshData* Mesh::CreateCube(bool inverted)
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

        // left face
        // Position                       // Normal                       // Texture Coords
        { XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT2(0.0f, 0.0f)},

        // right face
        // Position                       // Normal                       // Texture Coords                     
        { XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT2(0.0f, 0.0f)},

        // back face
        // Position                       // Normal                       // Texture Coords
        { XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 0.0f)},
        { XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 1.0f)},
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT2(0.0f, 1.0f)},
        { XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT2(0.0f, 0.0f)},

        // bottom face
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

    if (_isInverted)
    {
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

        // Cube Index Initialization
        // ByteWidth gets the length of the indices and reserves the correct amount of data for it to be loaded
        bufferDesc.ByteWidth = sizeof(SimpleVertex);
        // Immutable means unchangable, meaning that this data cant be altered during runtime
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        // This tells the GPU that its specifically an index buffer, and so it uses the correct specifications for indicies
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
    }
    // Creates the Mesh
    _meshData->IndexBuffer = _indexBuffer;
    _meshData->VertexBuffer = _vertexBuffer;
    _meshData->IndexCount = 36;
    _meshData->VBOffset = 0;
    _meshData->VBStride = sizeof(SimpleVertex);

    SetMinAndMax();

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

void Mesh::SetMinAndMax()
{
    //// Write constant buffer data onto GPU
    //D3D11_MAPPED_SUBRESOURCE mappedSubresource;

    //HRESULT hr = _immediateContext->Map(_meshData->VertexBuffer, 0, D3D11_MAP_READ, 0, &mappedSubresource);
    //if (FAILED(hr))
    //{
    //	printf("Failed to map vertex buffer. HRESULT: 0x%X\n", hr);
    //	return;
    //}
    //SimpleVertex* vertices = static_cast<SimpleVertex*>(mappedSubresource.pData);

    //for (size_t i = 0; i < _meshData->IndexCount; ++i)
    //{
    //	XMFLOAT3 pos = vertices[i].Position;
    //
    //	// Update min and max values
    //	_min.x = min(_min.x, pos.x);
    //	_min.y = min(_min.y, pos.y);
    //	_min.z = min(_min.z, pos.z);

    //	_max.x = max(_max.x, pos.x);
    //	_max.y = max(_max.y, pos.y);
    //	_max.z = max(_max.z, pos.z);
    //}
    //// Unmap the vertex buffer
    //_immediateContext->Unmap(_meshData->VertexBuffer, 0);


    // Get the vertex buffer description
    D3D11_BUFFER_DESC vertexDesc;
    _meshData->VertexBuffer->GetDesc(&vertexDesc);

    // Create a staging buffer with the same size as the vertex buffer
    // This is needed as the data has been created beforehand, meaning that its stored within the CPU, with this buffer allowing for access to the CPU data
    D3D11_BUFFER_DESC stagingDesc = vertexDesc;
    stagingDesc.Usage = D3D11_USAGE_STAGING;
    stagingDesc.BindFlags = 0; // No binding
    stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    ID3D11Buffer* stagingBuffer = nullptr;
    HRESULT hr = _device->CreateBuffer(&stagingDesc, nullptr, &stagingBuffer);
    if (FAILED(hr))
    {
        printf("Failed to create staging buffer. HRESULT: 0x%X\n", hr);
        return;
    }

    // Copy the vertex buffer data to the staging buffer
    _immediateContext->CopyResource(stagingBuffer, _meshData->VertexBuffer);

    // Map the staging buffer to access the vertex data
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    hr = _immediateContext->Map(stagingBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
    if (FAILED(hr))
    {
        printf("Failed to map staging buffer. HRESULT: 0x%X\n", hr);
        stagingBuffer->Release();
        return;
    }

    // Access the vertex data
    SimpleVertex* vertices = static_cast<SimpleVertex*>(mappedResource.pData);
    size_t vertexCount = vertexDesc.ByteWidth / sizeof(SimpleVertex);

    // Iterate through vertices to find the bounding box
    for (size_t i = 0; i < vertexCount; ++i)
    {
        XMFLOAT3& pos = vertices[i].Position;

        //Update min and max values
        _min.x = min(_min.x, pos.x);
        _min.y = min(_min.y, pos.y);
        _min.z = min(_min.z, pos.z);

        _max.x = max(_max.x, pos.x);
        _max.y = max(_max.y, pos.y);
        _max.z = max(_max.z, pos.z);
    }

    // Unmap the staging buffer and release it
    _immediateContext->Unmap(stagingBuffer, 0);
    stagingBuffer->Release();
}