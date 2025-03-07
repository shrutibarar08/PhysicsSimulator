cbuffer VertexConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

cbuffer LightCountBuffer : register(b1)
{
    int numLights;
    float3 padding;
}

struct Light
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Position;
    float Range;
    float3 Direction;
    float Spot;
    float3 Att;
    int id;
};

StructuredBuffer<Light> Lights : register(t0);

Texture2D textureMap : register(t1);
SamplerState samplerState : register(s0);

struct InputData
{
    float4 position : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD1;
};

float3 ComputeSpotLight(Light light, float3 normal, float3 fragPos, float3 viewDir)
{
    float3 lightDir = normalize(light.Position - fragPos);
    float theta = dot(lightDir, normalize(light.Direction));
    float epsilon = light.Spot - 0.05;
    float intensity = smoothstep(epsilon, light.Spot, theta);

    float diff = max(dot(normal, lightDir), 0.0f);
    float3 diffuse = light.Diffuse.rgb * diff;

    float3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), light.Specular.a);
    float3 specular = light.Specular.rgb * spec;

    float dist = length(light.Position - fragPos);
    float attenuation = 1.0f / (light.Att.x + light.Att.y * dist + light.Att.z * (dist * dist));

    return (light.Ambient.rgb + (diffuse + specular) * attenuation);
}

float3 ComputeDirectionalLight(Light light, float3 normal, float3 fragPos, float3 viewDir)
{
    float3 lightDir = normalize(-light.Direction);
    float diff = max(dot(normal, lightDir), 0.0f);
    float3 diffuse = light.Diffuse.rgb * diff;

    float3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), light.Specular.a);
    float3 specular = light.Specular.rgb * spec;

    return light.Ambient.rgb + diffuse + specular;
}

float3 ComputePointLight(Light light, float3 normal, float3 fragPos, float3 viewDir)
{
    float3 lightVec = light.Position - fragPos;
    float dist = length(lightVec);
    if (dist > light.Range)
        return float3(0.0f, 0.0f, 0.0f);

    lightVec /= dist;
    float3 ambient = light.Ambient.rgb;
    float diffuseFactor = max(dot(normal, lightVec), 0.0f);
    float3 diffuse = diffuseFactor * light.Diffuse.rgb;

    float3 reflectDir = reflect(-lightVec, normal);
    float specFactor = pow(max(dot(normalize(viewDir), reflectDir), 0.0f), light.Specular.a);
    float3 specular = specFactor * light.Specular.rgb;

    float attenuation = 1.0f / (light.Att.x + light.Att.y * dist + light.Att.z * (dist * dist));

    return (ambient + (diffuse + specular) * attenuation);
}

float4 main(InputData input) : SV_Target
{
    float3 normal = normalize(input.normal);
    float3 fragPos = input.worldPos;
    float3 viewDir = normalize(-fragPos);

    float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    for (uint i = 0; i < numLights; i++)
    {
        if (Lights[i].id == 0)
        {
            finalColor += ComputeDirectionalLight(Lights[i], normal, fragPos, viewDir);
        }
        if (Lights[i].id == 1)
        {
            finalColor += ComputePointLight(Lights[i], normal, fragPos, viewDir);
        }
        if (Lights[i].id == 2)
        {
            finalColor += ComputeSpotLight(Lights[i], normal, fragPos, viewDir);
        }
    }

    float4 textureColor = textureMap.Sample(samplerState, input.texCoord);
    return float4(finalColor * textureColor.rgb, textureColor.a);
}
