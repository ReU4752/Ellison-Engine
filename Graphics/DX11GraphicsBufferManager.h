#pragma once

#include "../Core/GraphicsBufferManager.h"

namespace Ellison_Engine
{
	class CDX11GraphicsBufferManager : public IGraphicsBufferManager
	{
	public:
		CDX11GraphicsBufferManager(ID3D11Device& device);

	protected:
		IVertexBuffer* VertexBufferCreationHelper(const VERTEX_BUFFER_DESC& desc) override;

		IIndexBuffer* IndexBufferCreationHelper(const INDEX_BUFFER_DESC& desc) override;

	private:
		ID3D11Device& _device;
	};
}