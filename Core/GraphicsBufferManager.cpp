#include "CoreStdafx.h"
#include "GraphicsBufferManager.h"

namespace Ellison_Engine
{
	IGraphicsBufferManager* IGraphicsBufferManager::_instance = nullptr;

	IGraphicsBufferManager::IGraphicsBufferManager()
	{
	}

	IGraphicsBufferManager::~IGraphicsBufferManager()
	{
	}

	IVertexBuffer* IGraphicsBufferManager::CreateVertexBuffer(const VERTEX_BUFFER_DESC& desc)
	{
		IVertexBuffer* buf = VertexBufferCreationHelper(desc);
	  buf->Initialize();

		return buf;
	}

	IIndexBuffer* IGraphicsBufferManager::CreateIndexBuffer(const INDEX_BUFFER_DESC& desc)
	{
		IIndexBuffer* buf = IndexBufferCreationHelper(desc);
		buf->Initialize();

		return buf;
	}
}