#pragma once

#include "../Core/VertexBuffer.h"

namespace Ellison_Engine
{
  class CDX11VertexBuffer : public IVertexBuffer
  {
  public:
    CDX11VertexBuffer(ID3D11Device& device, const VERTEX_BUFFER_DESC& desc);
    virtual ~CDX11VertexBuffer();

    virtual void* GetNativeVertexBufferPtr() override;

  protected:
    virtual bool Initialize() override;
    virtual void Release() override;

  private:
    bool CreateBuffer();

  private:
    ID3D11Device& _device;
    ID3D11Buffer* _vertexBuffer;
  };
}