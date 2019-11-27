cbuffer CameraMatrixBuffer : register(b0)
{
     float4x4 viewMatrix;
     float4x4 projectionMatrix;
};

cbuffer ObjectMatrixBuffer : register(b1)
{
     float4x4 worldMatrix;
};

struct VS_INPUT
{
     float4 position : POSITION;
     float4 color : COLOR;
};

struct PS_INPUT
{
     float4 position : SV_POSITION;
     float4 color : COLOR;
};

PS_INPUT VertexShader(VS_INPUT input)
{
     PS_INPUT output = (PS_INPUT)0;

     input.position.w = 1.0f;

     output.position = mul(input.position, worldMatrix);
     output.position = mul(output.position, viewMatrix);
     output.position = mul(output.position, projectionMatrix);

     output.color = input.color;

     return output;
}