#pragma once

namespace Ellison_Engine
{
  struct INDEX_BUFFER_DESC
  {
    void* indices;
    size_t indexByte;
    size_t numIndices;
  };

  class CIndexBufferAttributes
  {
  public:
    CIndexBufferAttributes(const INDEX_BUFFER_DESC& desc);

    void* GetIndices() const { return _indices; }
    size_t GetIndexByte() const { return _indexByte; }
    size_t GetNumIndices() const { return _numIndices; }

  private:
    void* _indices;
    size_t _indexByte;
    size_t _numIndices;
  };

  class IIndexBuffer
  {
  public:
    IIndexBuffer(const INDEX_BUFFER_DESC& desc);
    virtual ~IIndexBuffer();

    virtual void* GetNativeIndexBufferPtr() = 0;

    const CIndexBufferAttributes& GetAttributes() { return _attributes; }

    static IIndexBuffer* Create(const INDEX_BUFFER_DESC& desc);

  protected:
		friend class IGraphicsBufferManager;

    virtual bool Initialize() = 0;
    virtual void Release() = 0;

  protected:
    CIndexBufferAttributes _attributes;
  };
}