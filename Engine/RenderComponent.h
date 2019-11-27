#pragma once

#include "Component.h"

#include "../Core/Mesh.h"
#include "Shader.h"

namespace Ellison_Engine
{
  class CRenderComponent : public CComponent
  {
  public:
    DEFINE_COMPONENT(CRenderComponent, CComponent, true)

  public:
    CRenderComponent();
    virtual ~CRenderComponent();

    virtual bool Initialize(void) override;
    virtual void Release(void) override;

    void Draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* cameraMatrixBuffer, const DirectX::XMMATRIX& worldMatrix);

    void SetMesh(IMesh* mesh);
    void SetShader(CShader* shader);

  private:
		IMesh* _mesh;
    CShader* _shader;
  };
}