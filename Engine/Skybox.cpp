#include "EngineStdafx.h"
#include "Skybox.h"
#include "../Core/MeshGenerator.h"
#include "SkyboxShader.h"
#include "Locator.h"
#include "RenderComponent.h"

namespace Ellison_Engine
{
	bool CSkyboxEntity::Initialize(void)
	{
		AddComponent<CRenderComponent>();

		_mesh = CMeshGenerator::CreateMeshFromEEMFile(L"Model/Box.eem");
		if (!_mesh)
		{
			return false;
		}

		_shader = CSkyBoxShader::CreateShader<CSkyBoxShader>(CLocator::GetDirect3D()->GetDevice(), L"Shader/Skybox.vs", L"Shader/Skybox.ps");
		if (!_shader)
		{
			return false;
		}

		TEXTURE_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		_diffuseTexture = DX11Texture::Create(*CLocator::GetDirect3D()->GetDevice(), L"Texture/Skybox.dds", desc);

		static_cast<CSkyBoxShader*>(_shader)->SetSkyBoxTexture(_diffuseTexture);

		GetComponent<CRenderComponent>()->SetMesh(_mesh);
		GetComponent<CRenderComponent>()->SetShader(_shader);

		return true;
	}

	void CSkyboxEntity::Release(void)
	{
		if (_mesh)
		{
			delete _mesh;
			_mesh = 0;
		}
		if (_shader)
		{
			_shader->Release();
			delete _shader;
			_shader = 0;
		}
	}

	/////////////////////////////////////

	CSkybox::CSkybox()
	{
	}

	CSkybox::~CSkybox()
	{
	}

	bool CSkybox::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		_entity.Initialize();
		AttachEntity(&_entity);

		return true;
	}

	void CSkybox::Release(void)
	{
		_entity.Release();
	}

	bool CSkybox::RenderSkyBox(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix, ID3D11Buffer* cameraMatrixBuffer, 
		DirectX::XMFLOAT3 cameraPos)
	{
		DirectX::XMMATRIX scaleMatrix;
		DirectX::XMMATRIX translationMatrix;

		worldMatrix = DirectX::XMMatrixIdentity();
		scaleMatrix = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
		translationMatrix = DirectX::XMMatrixTranslation(cameraPos.x, cameraPos.y, cameraPos.z);
		worldMatrix = scaleMatrix * translationMatrix;

		_entity.GetComponent<CRenderComponent>()->Draw(deviceContext, cameraMatrixBuffer, worldMatrix);

		return true;
	}
}