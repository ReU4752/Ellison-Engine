cbuffer DirLightAttribute : register(b0)
{
	float3 ambientDown	: packoffset(c0);
	float3 ambientRange	: packoffset(c1);
}

Texture2D DiffuseTexture;
Texture2D NormalTexture;
SamplerState Sampler;

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 T : TEXCOORD0;
	float3 B : TEXCOORD1;
	float3 N : TEXCOORD2;
	float2 texCoord : TEXCOORD3;
};

float3 CalcAmbient(float3 normal, float3 albedo)
{
	float up = normal.y * 0.5 + 0.5;

	float3 ambient = ambientDown + up * ambientRange;

	return ambient * albedo;
}

float3 CalcDirectional(float3 normal, float lightColor, float3 albedo)
{
	float3 lightDir = float3(-1.0, -1.0f, -1.0f);

	float NDotL = dot(normal, normalize(-lightDir));
	float3 finalColor = lightColor * saturate(NDotL);

	return finalColor * albedo;
}

float4 PixelShaderEntry(PS_INPUT input) : SV_TARGET
{
	float3 tangentNormal = NormalTexture.Sample(Sampler, input.texCoord).xyz;	
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(input.T), normalize(input.B), normalize(input.N));
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal);

	// Get texcel from texture
	float3 albedo = DiffuseTexture.Sample(Sampler, input.texCoord);
	albedo = pow(albedo, 2.2f);

	float3 finalColor = (float3)0;

	// Calculate ambient light
	finalColor = CalcAmbient(tangentNormal, albedo);

	// Calculate directional light
	float3 lightColor = pow(float3(1.0f, 1.0f, 1.0f), 2.2f);
	finalColor += CalcDirectional(tangentNormal, lightColor, albedo);

	return float4(pow(finalColor, 1 / 2.2f), 1);
}