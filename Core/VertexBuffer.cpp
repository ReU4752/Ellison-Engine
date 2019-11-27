#include "CoreStdafx.h"
#include "VertexBuffer.h"
#include "GraphicsBufferManager.h"

namespace Ellison_Engine
{
	CVertexBufferAttributes::CVertexBufferAttributes(void* vertices, const size_t& vertexByte, const size_t& numVertices)
		: _vertices(vertices), _vertexByte(vertexByte), _numVertices(numVertices)
	{
	}

	IVertexBuffer::IVertexBuffer(const VERTEX_BUFFER_DESC& desc)
		: _attributes(desc.vertices, desc.vertexByte, desc.numVertices)
	{
	}

	IVertexBuffer::~IVertexBuffer()
	{
	}

  IVertexBuffer* IVertexBuffer::Create(const VERTEX_BUFFER_DESC& desc)
  {
    return IGraphicsBufferManager::Instance()->CreateVertexBuffer(desc);
  }
}