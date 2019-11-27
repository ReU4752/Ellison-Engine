#pragma once

namespace Ellison_Engine
{
	struct VERTEX_BUFFER_DESC
	{
		void* vertices;
		size_t vertexByte;
		size_t numVertices;
	};

	class CVertexBufferAttributes
	{
	public:
		CVertexBufferAttributes(void* vertices, const size_t& vertexByte, const size_t& numVertices);

		void* GetVertices() const { return _vertices; }
		size_t GetVertexByte() const { return _vertexByte; }
		size_t GetNumVertices() const { return _numVertices; }

	private:
		void* _vertices;
		size_t _vertexByte;
		size_t _numVertices;
	};

	class IVertexBuffer
	{
	public:
		IVertexBuffer(const VERTEX_BUFFER_DESC& desc);
		virtual ~IVertexBuffer();

		virtual void* GetNativeVertexBufferPtr() = 0;

		const CVertexBufferAttributes& GetAttributes() { return _attributes; }

    static IVertexBuffer* Create(const VERTEX_BUFFER_DESC& desc);

	protected:
		friend class IGraphicsBufferManager;

		virtual bool Initialize() = 0;
		virtual void Release() = 0;

	protected:
		CVertexBufferAttributes _attributes;
	};
}