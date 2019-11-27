#include "GraphicsStdafx.h"
#include "DX11GraphicsBufferManager.h"
#include "DX11VertexBuffer.h"
#include "DX11IndexBuffer.h"

namespace Ellison_Engine
{
	CDX11GraphicsBufferManager::CDX11GraphicsBufferManager(ID3D11Device& device)
		: _device(device)
	{
	}

	IVertexBuffer* CDX11GraphicsBufferManager::VertexBufferCreationHelper(const VERTEX_BUFFER_DESC& desc)
	{
		CDX11VertexBuffer* buf = new CDX11VertexBuffer(_device, desc);

		return buf;
	}

	IIndexBuffer* CDX11GraphicsBufferManager::IndexBufferCreationHelper(const INDEX_BUFFER_DESC& desc)
	{
    CDX11IndexBuffer* buf = new CDX11IndexBuffer(_device, desc);

		return buf;
	}
}