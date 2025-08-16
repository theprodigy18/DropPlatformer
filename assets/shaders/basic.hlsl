struct VSInput
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    output.pos = float4(input.pos, 1.0);
    output.uv  = input.uv;

    return output;
}

float4 PSMain(VSOutput input) : SV_Target
{
    return float4(1.0, 1.0, 1.0, 1.0);
}