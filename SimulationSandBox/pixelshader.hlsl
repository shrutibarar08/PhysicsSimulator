struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 PSMain(PSInput input) : SV_Target
{
    return input.color; 
}
