struct VSInput
{
    float2 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
};

cbuffer ProjectionParams : register(b0)
{
    float4x4 projection;
};

VSOutput VSMain(VSInput input)
{
    float4 pos = mul(projection, float4(input.pos, 0.0, 1.0));

    VSOutput output;
    output.pos = pos;
    output.uv  = input.uv;

    return output;
}

float4 PSMain(VSOutput input) : SV_Target
{
    return float4(1.0, 1.0, 1.0, 1.0);
}