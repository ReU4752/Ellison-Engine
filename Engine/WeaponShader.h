#pragma once

#include "Shader.h"
#include "../Graphics/DX11Texture.h"

namespace Ellison_Engine
{
	class CWeaponShader : public CShader
	{
	private:
		struct DirLightAttribute
		{
			DirectX::XMVECTOR ambientDown;
			DirectX::XMVECTOR ambientRange;
		};

	public:
		CWeaponShader();
		~CWeaponShader();

		virtual bool Initialize(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename);
		virtual void Release();

		virtual void Render(ID3D11DeviceContext* deviceContext, UINT indexCount, DirectX::XMMATRIX worldMatrix, ID3D11Buffer* cameraMatrixBuffer);

		void SetDiffuseTexture(DX11Texture* texture);
		void SetNormalTexture(DX11Texture* texture);

	protected:
		bool CreateConstantBuffer(ID3D11Device* device);
		virtual bool CreateInputLayout(ID3D11Device* device, ID3D10Blob* vsBuffer) override;

	private:
		DirectX::XMFLOAT3 _ambientDown;
		DirectX::XMFLOAT3 _ambientUp;

		ID3D11Buffer* _dirLightAttribute;

		DX11Texture* _diffuseTexture;
		DX11Texture* _normalTexture;

		ID3D11SamplerState* _sampleState;
	};
}