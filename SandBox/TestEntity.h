#pragma once

#include <Engine\Entity.h>
#include <Core\Mesh.h>
#include <Engine\WeaponShader.h>
#include <Graphics\DX11Texture.h>

class CTestEntity : public Ellison_Engine::CEntity
{
public:
  CTestEntity();
  virtual ~CTestEntity();

  virtual bool Initialize(void) override;
  virtual void Release(void) override;

private:
  Ellison_Engine::IMesh* _mesh;
  Ellison_Engine::CShader* _shader;
	Ellison_Engine::DX11Texture* _diffuseTexture;
	Ellison_Engine::DX11Texture* _normalTexture;
};