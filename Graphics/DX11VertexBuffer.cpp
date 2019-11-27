#include "GraphicsStdafx.h"
#include "DX11VertexBuffer.h"

namespace Ellison_Engine
{
  CDX11VertexBuffer::CDX11VertexBuffer(ID3D11Device& device, const VERTEX_BUFFER_DESC& desc)
    : IVertexBuffer(desc), _device(device), _vertexBuffer(nullptr)
  {
  }

  CDX11VertexBuffer::~CDX11VertexBuffer()
  {
    Release();
  }

  void* CDX11VertexBuffer::GetNativeVertexBufferPtr()
  {
    return _vertexBuffer;
  }

  bool CDX11VertexBuffer::Initialize()
  {
    if (!CreateBuffer())
    {
      return false;
    }

    return true;
  }

  void CDX11VertexBuffer::Release()
  {
    if (_vertexBuffer)
    {
      _vertexBuffer->Release();
      _vertexBuffer = nullptr;
    }
  }

  bool CDX11VertexBuffer::CreateBuffer()
  {
    HRESULT result;
    D3D11_BUFFER_DESC bufferDesc;
    D3D11_SUBRESOURCE_DATA subData;

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = _attributes.GetVertexByte() * _attributes.GetNumVertices();
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    subData.pSysMem = _attributes.GetVertices();
    subData.SysMemPitch = 0;
    subData.SysMemSlicePitch = 0;

    result = _device.CreateBuffer(&bufferDesc, &subData, &_vertexBuffer);
    if FAILED(result)
    {
      return false;
    }

    return true;
  }
}