#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Ellison_Engine
{
  struct MESH_DESC
  {
    IVertexBuffer* vertexBuffer;
    IIndexBuffer* indexBuffer;
  };

  class CMeshAttributes
  {
  public:
    CMeshAttributes(IVertexBuffer* vertexBuffer, IIndexBuffer* indexBuffer);
    ~CMeshAttributes();

    IVertexBuffer* GetVertexBuffer() { return _vertexBuffer; }
    IIndexBuffer* GetIndexBuffer() { return _indexBuffer; }

  private:
    IVertexBuffer* _vertexBuffer;
    IIndexBuffer* _indexBuffer;
  };

  class IMesh
  {
  public:
    IMesh(const MESH_DESC& desc);
    virtual ~IMesh();

    CMeshAttributes& GetAttributes() { return _attributes; }

    static IMesh* Create(const MESH_DESC& desc);

  protected:
    friend class IMeshManager;

    virtual bool Initialize() = 0;
    virtual void Release() = 0;

  private:
    CMeshAttributes _attributes;
  };

  class CMesh : public IMesh
  {
  public:
    CMesh(const MESH_DESC& desc);
    ~CMesh();

  protected:
    bool Initialize() override;
    void Release() override;
  };
}