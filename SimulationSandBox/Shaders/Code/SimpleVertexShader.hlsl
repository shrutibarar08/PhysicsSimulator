cbuffer VertexConstantBuffer
{
    matrix World;
    matrix View;
    matrix Projection;
    float3 Velocity;
    float Elastic;
};

struct InputData
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct OutputData
{
    float4 position : SV_POSITION; // Clip space position
    float3 worldPos : TEXCOORD0; // World space position
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD1;
};

OutputData main(InputData input)
{
    OutputData output_data;

        // Prevent NaN issues by safely normalizing velocity
    float3 safeVelocity = length(Velocity) > 0.0001 ? normalize(Velocity) : float3(0, 0, 0);
    float3 stretch = safeVelocity * saturate(1.0 - Elastic) * 0.2f;

    // Apply stretch
    float3 position = input.position + stretch;

    // Compute world-space position
    float4 worldPos = mul(float4(position, 1.0f), World);
    // float4 worldPos = mul(float4(input.position, 1.0f), World);
    output_data.worldPos = worldPos.xyz;

    // Transform position to clip space (optimized)
    matrix ViewProjection = mul(View, Projection);
    output_data.position = mul(worldPos, ViewProjection);

    // Transform normal to world space (fixing transformation)
    output_data.normal = normalize(mul((float3x3) World, input.normal));

    // Pass through color and texture coordinates
    output_data.texCoord = input.texCoord;

    return output_data;
}
