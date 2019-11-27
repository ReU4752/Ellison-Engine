#pragma once

#include "SceneNode.h"
#include "../Core/Mesh.h"
#include "Shader.h"
#include "../Graphics/DX11Texture.h"

namespace Ellison_Engine
{
	class CSkyboxEntity : public CEntity
	{
	public:
		virtual bool Initialize(void) override;
		virtual void Release(void) override;

	private:
		Ellison_Engine::IMesh* _mesh;
		Ellison_Engine::CShader* _shader;
		Ellison_Engine::DX11Texture* _diffuseTexture;
	};

	class CSkybox : public CSceneNode
	{
	public:
		CSkybox();
		~CSkybox();

		bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		void Release(void);

		bool RenderSkyBox(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix, ID3D11Buffer* cameraMatrixBuffer, 
			DirectX::XMFLOAT3 cameraPos);

	private:
		CSkyboxEntity _entity;
	};
}