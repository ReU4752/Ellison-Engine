#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Ellison_Engine
{
	class IGraphicsBufferManager
	{
	public:
		IGraphicsBufferManager();
		virtual ~IGraphicsBufferManager();

		IVertexBuffer* CreateVertexBuffer(const VERTEX_BUFFER_DESC& desc);

		IIndexBuffer* CreateIndexBuffer(const INDEX_BUFFER_DESC& desc);

		static IGraphicsBufferManager** InstanceOriginal()
		{
			return &_instance;
		}

    static IGraphicsBufferManager* Instance()
    {
			return _instance;
    }

	protected:
		virtual IVertexBuffer* VertexBufferCreationHelper(const VERTEX_BUFFER_DESC& desc) = 0;

		virtual IIndexBuffer* IndexBufferCreationHelper(const INDEX_BUFFER_DESC& desc) = 0;

	private:
		static IGraphicsBufferManager* _instance;
	};
}