#pragma once

#include "../Core/IndexBuffer.h"

namespace Ellison_Engine
{
  class CDX11IndexBuffer : public IIndexBuffer
  {
  public:
    CDX11IndexBuffer(ID3D11Device& device, const INDEX_BUFFER_DESC& desc);
    ~CDX11IndexBuffer();

    virtual void* GetNativeIndexBufferPtr() override;

  protected:
    virtual bool Initialize() override;
    virtual void Release() override;

  private:
    bool CreateBuffer();

  private:
    ID3D11Device& _device;
    ID3D11Buffer* _indexBuffer;
  };
}