#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::JSONLoad(ID3D11Device* _device)
{
    //Json Parser
    json jFile;

    std::ifstream fileOpen("JSON/gameObjects.json");

    //validates to see if the file has been opened
    if (!fileOpen.is_open() || fileOpen.fail())
    {
        std::cerr << "Error: file could not be opened." << std::endl;
        return;
    }
    //this is causing me an error
    jFile = json::parse(fileOpen);

    std::string v = jFile["version"].get<std::string>();

    json& fileData = jFile["GameObjects"]; //← gets an array
    int size = fileData.size();

    std::vector<gameObjectData> gameobjects;

    for (unsigned int i = 0; i < size; i++)
    {
        gameObjectData g;
        json& gameObjectDesc = fileData.at(i);
        g.objFilePath = gameObjectDesc["FilePath"];
        g.specularTexture = gameObjectDesc["Specular"];
        g.colorTexture = gameObjectDesc["Color"];
        g.startPos.x = gameObjectDesc["StartPos"][0];
        g.startPos.y = gameObjectDesc["StartPos"][1];
        g.startPos.z = gameObjectDesc["StartPos"][2];
        g.startScale = gameObjectDesc["StartScale"];

        gameobjects.push_back(g);
    }

    for (int i = 0; i < gameobjects.size(); i++)
    {
        _meshData = OBJLoader::Load(gameobjects.at(i).objFilePath, _device, false);
        SetMeshData(_meshData); //pass the meshData into the GameObject Class

        //code below doesnt work as I cant get a reference to _cbData

        //_hasSpecularMap = 1;
        //CreateDDSTextureFromFile(_device, gameobjects.at(i).specularTexture, nullptr, &_texture);

        //_hasTexture = 1;
        //CreateDDSTextureFromFile(_device, gameobjects.at(i).colorTexture, nullptr, &_texture);
        //gameObject.SetShaderResource(_texture);

        //need to do world data
        //gameObject.SetWorldMatrix(_starOBJWorldMatrix);

        //_cbData.World = XMMatrixTranspose(XMLoadFloat4x4(gameObject.GetWorldMatrix()));
    }
}

void GameObject::CreateTexture(ID3D11Device* _device, const wchar_t* filePath, GameObject gameObject)
{
    ID3D11ShaderResourceView* _texture;

    CreateDDSTextureFromFile(_device, filePath, nullptr, &_texture);

    SetShaderResource(_texture);
}

void GameObject::Draw(GameObject gameObject, ID3D11DeviceContext* _immediateContext, ID3D11Buffer* _constantBuffer)
{
    //A majority of the draw code need for the game object class (some I couldnt move to this class due to _cbData or something simular)
	if (_meshData.VertexBuffer != nullptr && _meshData.IndexBuffer != nullptr)
	{
		_immediateContext->PSSetShaderResources(0, 1, gameObject.GetShaderResource());

		_immediateContext->IASetVertexBuffers(0, 1, &gameObject.GetMeshData()->VertexBuffer, &gameObject.GetMeshData()->VBStride, &gameObject.GetMeshData()->VBOffset);
		_immediateContext->IASetIndexBuffer(gameObject.GetMeshData()->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		_immediateContext->DrawIndexed(gameObject.GetMeshData()->IndexCount, 0, 0);
	}
}