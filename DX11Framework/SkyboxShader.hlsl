Texture2D skybox : register(t0);
SamplerState bilinearSampler : register(s0);

struct SkyboxVS_Out
{
    float4 position : SV_Position;
    float3 texCoord : TEXCOORD;
};

SkyboxVS_Out VS_main(float4 Position : POSITION, float3 TexCoord : TEXCOORD)
{
    SkyboxVS_Out output = (SkyboxVS_Out) 0;
    
    output.position = output.position.xyww;
    output.texCoord = Position;
    
    return output;
}

SkyboxVS_Out PS_main(SkyboxVS_Out input) : SV_TARGET
{
    return input;
}