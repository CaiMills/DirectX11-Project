#include "Appearance.h"

Appearance::Appearance(MeshData meshData) : _meshData(meshData)
{
	_texture = nullptr;
}

Appearance::~Appearance()
{
	_texture = nullptr;
	_meshData.IndexBuffer = nullptr;
	_meshData.VertexBuffer = nullptr;
}

void Appearance::SetMinAndMax()
{
	////Write constant buffer data onto GPU
	//D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	//HRESULT hr = _immediateContext->Map(_meshData.VertexBuffer, 0, D3D11_MAP_READ, 0, &mappedSubresource);
	//if (FAILED(hr))
	//{
	//	printf("Failed to map vertex buffer. HRESULT: 0x%X\n", hr);
	//	return;
	//}
	//SimpleVertex* vertices = static_cast<SimpleVertex*>(mappedSubresource.pData);

	//for (size_t i = 0; i < _meshData.IndexCount; ++i)
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
	//_immediateContext->Unmap(_meshData.VertexBuffer, 0);



    //Get the vertex buffer description
    D3D11_BUFFER_DESC vertexDesc;
    _meshData.VertexBuffer->GetDesc(&vertexDesc);
    
    //Create a staging buffer with the same size as the vertex buffer
	//This is needed as the data has been created beforehand, meaning that its stored within the CPU, with this buffer allowing for access to the CPU data
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
    _immediateContext->CopyResource(stagingBuffer, _meshData.VertexBuffer);
    
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

		// Update min and max values
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

void Appearance::Draw()
{
	SetMinAndMax();
	DebugPrintF("Max x is %f, Max y is %f, Max z is %f\n", _max.x, _max.y, _max.z);
	DebugPrintF("Min x is %f, Min y is %f, Min z is %f\n", _min.x, _min.y, _min.z);

	//A majority of the draw code need for the game object class (some I couldnt move to this class due to _cbData or something simular)
	if (_meshData.VertexBuffer != nullptr && _meshData.IndexBuffer != nullptr)
	{
		_immediateContext->PSSetShaderResources(0, 1, &_texture);

		// Set vertex and index buffers
		_immediateContext->IASetVertexBuffers(0, 1, &_meshData.VertexBuffer, &_meshData.VBStride, &_meshData.VBOffset);
		_immediateContext->IASetIndexBuffer(_meshData.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		_immediateContext->DrawIndexed(_meshData.IndexCount, 0, 0);
	}
}
