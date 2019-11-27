#include "GraphicsStdafx.h"
#include "DX11IndexBuffer.h"

namespace Ellison_Engine
{
  CDX11IndexBuffer::CDX11IndexBuffer(ID3D11Device& device, const INDEX_BUFFER_DESC& desc)
    : IIndexBuffer(desc), _device(device), _indexBuffer(nullptr)
  {
  }

  CDX11IndexBuffer::~CDX11IndexBuffer()
  {
    Release();
  }

  void* CDX11IndexBuffer::GetNativeIndexBufferPtr()
  {
    return _indexBuffer;
  }

  bool CDX11IndexBuffer::Initialize()
  {
    if (!CreateBuffer())
    {
      return false;
    }

    return true;
  }

  void CDX11IndexBuffer::Release()
  {
    if (_indexBuffer)
    {
      _indexBuffer->Release();
      _indexBuffer = nullptr;
    }
  }

  bool CDX11IndexBuffer::CreateBuffer()
  {
    HRESULT result;
    D3D11_BUFFER_DESC bufferDesc;
    D3D11_SUBRESOURCE_DATA subData;

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = _attributes.GetIndexByte() * _attributes.GetNumIndices();
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    subData.pSysMem = _attributes.GetIndices();
    subData.SysMemPitch = 0;
    subData.SysMemSlicePitch = 0;

    result = _device.CreateBuffer(&bufferDesc, &subData, &_indexBuffer);
    if FAILED(result)
    {
      return false;
    }

    return true;
  }
}