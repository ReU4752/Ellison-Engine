#include <Engine\EngineStdafx.h>
#include "TestEntity.h"
#include <Engine\Locator.h>

#include <Engine\RenderComponent.h>
#include <Core\MeshGenerator.h>

using namespace Ellison_Engine;

CTestEntity::CTestEntity()
{
  _mesh = nullptr;
  _shader = nullptr;
}

CTestEntity::~CTestEntity()
{
}

bool CTestEntity::Initialize(void)
{
  AddComponent<CRenderComponent>();

  //_mesh = CMeshGenerator::CreateTriangle(2.0f);
  //_mesh = Ellison_Engine::CModel::CreateQuad(device, 0.3f, 0.7f);
  _mesh = CMeshGenerator::CreateMeshFromEEMFile(L"Model/Terrain.eem");
  if (!_mesh)
  {
    return false;
  }

	_shader = CWeaponShader::CreateShader<CWeaponShader>(CLocator::GetDirect3D()->GetDevice(), L"Shader/WeaponShaderVS.hlsl", L"Shader/WeaponShaderPS.hlsl");
  if (!_shader)
  {
    return false;
  }

	TEXTURE_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	_diffuseTexture = DX11Texture::Create(*CLocator::GetDirect3D()->GetDevice(), L"Texture/Diffuse.png", desc);

	ZeroMemory(&desc, sizeof(desc));
	_normalTexture = DX11Texture::Create(*CLocator::GetDirect3D()->GetDevice(), L"Texture/Normal.png", desc);

	static_cast<CWeaponShader*>(_shader)->SetDiffuseTexture(_diffuseTexture);
	static_cast<CWeaponShader*>(_shader)->SetNormalTexture(_normalTexture);

	GetComponent<CRenderComponent>()->SetMesh(_mesh);
  GetComponent<CRenderComponent>()->SetShader(_shader);

  return true;
}

void CTestEntity::Release(void)
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