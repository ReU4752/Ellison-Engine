#include "EngineStdafx.h"
#include "RenderComponent.h"
#include "Entity.h"
#include "Shader.h"

namespace Ellison_Engine
{
  DECLARE_COMPONENT(CRenderComponent)

  CRenderComponent::CRenderComponent()
  {
    _mesh = nullptr;
    _shader = nullptr;
  }

  CRenderComponent::~CRenderComponent()
  {
  }

  bool CRenderComponent::Initialize()
  {

    return true;
  }

  void CRenderComponent::Release()
  {

  }

  void CRenderComponent::Draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* cameraMatrixBuffer, const DirectX::XMMATRIX& worldMatrix)
  {
		unsigned int stride;
		unsigned int offset;

		stride = _mesh->GetAttributes().GetVertexBuffer()->GetAttributes().GetVertexByte();
		offset = 0;

		ID3D11Buffer* vb = (ID3D11Buffer*)_mesh->GetAttributes().GetVertexBuffer()->GetNativeVertexBufferPtr();
		deviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

		ID3D11Buffer* ib = (ID3D11Buffer*)_mesh->GetAttributes().GetIndexBuffer()->GetNativeIndexBufferPtr();
		deviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    _shader->Render(deviceContext, _mesh->GetAttributes().GetIndexBuffer()->GetAttributes().GetNumIndices(), worldMatrix, cameraMatrixBuffer);
  }

  void CRenderComponent::SetMesh(IMesh* mesh)
  {
    _mesh = mesh;
  }

  void CRenderComponent::SetShader(CShader* shader)
  {
    _shader = shader;
  }
}