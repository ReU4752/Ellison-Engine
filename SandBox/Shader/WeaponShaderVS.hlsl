cbuffer CameraMatrixBuffer : register(b0)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

cbuffer ObjectMatrixBuffer : register(b1)
{
	float4x4 worldMatrix;
}

struct VS_INPUT
{
	float4 position : POSITION;
  float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 T : TEXCOORD0;
	float3 B : TEXCOORD1;
	float3 N : TEXCOORD2;
	float2 texCoord : TEXCOORD3;
};

PS_INPUT VertexShaderEntry(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	float3 worldTangent = mul(input.tangent, (float3x3)worldMatrix);
	output.T = normalize(worldTangent);

	float3 worldBinormal = mul(input.binormal, (float3x3)worldMatrix);
	output.B = normalize(worldBinormal);

	float3 worldNormal = mul(input.normal, (float3x3)worldMatrix);
	output.N = normalize(worldNormal);

	output.texCoord = input.texCoord;

	return output;
}