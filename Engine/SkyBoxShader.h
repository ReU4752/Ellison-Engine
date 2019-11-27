#pragma once

#include "Shader.h"
#include "../Graphics/DX11Texture.h"

namespace Ellison_Engine
{
	class CSkyBoxShader : public CShader
	{
	public:
		CSkyBoxShader();
		~CSkyBoxShader();
	
		virtual bool Initialize(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename);
		virtual void Release();

		virtual void Render(ID3D11DeviceContext* deviceContext, UINT indexCount, DirectX::XMMATRIX worldMatrix, ID3D11Buffer* cameraMatrixBuffer);

		void SetSkyBoxTexture(DX11Texture* texture);

	protected:
		virtual bool CreateInputLayout(ID3D11Device* device, ID3D10Blob* vsBuffer) override;

	private:
		DX11Texture* _skyboxTexture;

		ID3D11SamplerState* _sampleState;
	};
}