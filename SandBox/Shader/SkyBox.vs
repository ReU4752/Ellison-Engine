cbuffer CameraMatrixBuffer : register(b0)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

cbuffer ObjectMatrixBuffer : register(b1)
{
	float4x4 worldMatrix;
}

struct VertexInputType
{
     float4 position : POSITION;
};

struct PixelInputType
{
     float4 position : SV_POSITION;
     float3 texCoord : TEXCOORD;
};

PixelInputType VertexShaderEntry(VertexInputType input)
{
     PixelInputType output;
    
     // Change the position vector to be 4 units for proper matrix calculations.
     input.position.w = 1.0f;

     output.position = mul(input.position, worldMatrix);
     output.position = mul(output.position, viewMatrix);
     output.position = mul(output.position, projectionMatrix);

     output.texCoord = input.position;

     return output;
}