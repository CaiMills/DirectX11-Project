Texture2D diffuseTex : register(t0);
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

struct VS_Out
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

VS_Out VS_main(float3 Position : POSITION, float3 Normal : NORMAL, float2 TexCoord : TEXCOORD)
{   
    VS_Out output = (VS_Out)0;
    
    output.position = float4(Position, 1.0f);
    output.position = mul(output.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    
    output.normal = normalize(mul(float4(Normal, 1), World));
    output.texCoord = TexCoord;
    
    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    float4 colour;
    
    input.normal = normalize(input.normal);
    
    float3 normalisedLightDir = normalize(lightDir);

    //specular
    float3 vertexToCamera = normalize(cameraPosition - input.position.xyz);
    float3 reflectedDir = normalize(reflect(-lightDir, input.normal));
    float specIntensity = saturate(dot(vertexToCamera, reflectedDir));
    specIntensity = specIntensity * pow(specIntensity, specPower);
    
    //diffuse
    float4 diffuseAmount = saturate(dot(input.normal, normalisedLightDir));
    
    //if there is a texture, then do the additional calculation to add the texture
    if (hasTexture == 1)
    {
        float4 texColor = diffuseTex.Sample(bilinearSampler, input.texCoord);
        float4 diffuse = diffuseAmount * (texColor * diffuseLight);
        float4 ambient = texColor * ambientLight;
        
        //if there is a specular map texture, then do the additional calculation to add the specular calculation
        if (hasSpecularMap == 1)
        {
            float4 specTexColor = diffuseTex.Sample(bilinearSampler, input.texCoord);
            float4 specular = specIntensity * (specTexColor * specularLight);
            float4 litColor = texColor * (ambient + diffuse) + specular;
            colour = diffuse + ambient + specular + litColor;
        }
        else
        {
            float4 specular = specIntensity * (specularMaterial * specularLight);
            float4 litColor = texColor * (ambient + diffuse) + specular;
            colour = diffuse + ambient + specular + litColor;
        }
    }
    else
    {
        float4 diffuse = diffuseAmount * (diffuseMaterial * diffuseLight);
        float4 ambient = ambientMaterial * ambientLight;
        float4 specular = specIntensity * (specularMaterial * specularLight);
        
        colour.rgb = diffuse + ambient + specular;
    }
   
    return colour;
}