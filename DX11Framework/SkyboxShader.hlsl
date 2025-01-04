Texture2D skybox : register(t0);
SamplerState bilinearSampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    //16bit
    float4x4 Projection;
    float4x4 View;
    float4x4 World;
    
    //16bit
    float4 diffuseLight;
    float4 diffuseMaterial;
    float4 ambientLight;
    float4 ambientMaterial;
    
    //16bit
    float4 specularLight;
    float4 specularMaterial;
    float3 cameraPosition;
    float specPower;
    
    //3bit
    float3 lightDir;
    
    //textures
    uint hasTexture;
    uint hasSpecularMap;
}

struct SkyboxVS_Out
{
    float4 position : SV_POSITION;
    float3 posW : POSITION0;
    float2 texCoord : TEXCOORD;
};

SkyboxVS_Out VS_main(float3 Position : SV_POSITION, float2 TexCoord : TEXCOORD)
{
    SkyboxVS_Out output = (SkyboxVS_Out) 0;
    
    float4 Pos4 = float4(Position, 1.0f);
    output.position = mul(Pos4, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    output.posW = mul(Pos4, World);
    
    output.position = output.position.xyww;
    output.texCoord = Position;
    
    return output;
}

float4 PS_main(SkyboxVS_Out input) : SV_TARGET
{
    float4 texColor = skybox.Sample(bilinearSampler, input.texCoord);

    return texColor;
}