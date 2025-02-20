cbuffer VertexConstantBuffer
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct InputData
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct OutputData
{
    float4 color : COLOR;
    float4 position : SV_POSITION;
};

OutputData main(InputData input)
{
    OutputData output_data;

    output_data.position = mul(float4(input.position, 1.0f), World);
    output_data.position = mul(output_data.position, View);
    output_data.position = mul(output_data.position, Projection);

    output_data.color = input.color;
    
    return output_data;
}
