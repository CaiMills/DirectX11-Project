TextureCube skybox : register(t0);
SamplerState bilinearSampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    // 16 Bit
    float4x4 Projection;
    float4x4 View;
    float4x4 World;
    
    // 16 Bit
    float4 diffuseLight;
    float4 diffuseMaterial;
    float4 ambientLight;
    float4 ambientMaterial;
    
    // 16 Bit
    float4 specularLight;
    float4 specularMaterial;
    float3 cameraPosition;
    float specPower;
    
    // 3 Bit
    float3 lightDir;
    
    // Textures
    uint hasTexture;
    uint hasSpecularMap;
}

struct SkyboxVS_Out
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

SkyboxVS_Out VS_main(float3 Position : POSITION, float3 TexCoord : TEXCOORD)
{
    SkyboxVS_Out output = (SkyboxVS_Out) 0;
    
    output.position = normalize(mul(float4(Position, 1), World).xyww);
    output.texCoord = Position;
    
    return output;
}

float4 PS_main(SkyboxVS_Out input) : SV_TARGET
{
    return skybox.Sample(bilinearSampler, input.texCoord);
}