#include "CoreStdafx.h"
#include "IndexBuffer.h"
#include "GraphicsBufferManager.h"

namespace Ellison_Engine
{
  CIndexBufferAttributes::CIndexBufferAttributes(const INDEX_BUFFER_DESC& desc)
    : _indices(desc.indices), _indexByte(desc.indexByte), _numIndices(desc.numIndices)
  {
  }

  IIndexBuffer::IIndexBuffer(const INDEX_BUFFER_DESC& desc)
    : _attributes(desc)
  {
  }

  IIndexBuffer::~IIndexBuffer()
  {
  }

  IIndexBuffer* IIndexBuffer::Create(const INDEX_BUFFER_DESC& desc)
  {
    return IGraphicsBufferManager::Instance()->CreateIndexBuffer(desc);
  }
}