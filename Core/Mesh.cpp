#include "CoreStdafx.h"
#include "MeshManager.h"

namespace Ellison_Engine
{
  CMeshAttributes::CMeshAttributes(IVertexBuffer* vertexBuffer, IIndexBuffer* indexBuffer)
    : _vertexBuffer(vertexBuffer), _indexBuffer(indexBuffer)
  {
  }

  CMeshAttributes::~CMeshAttributes()
  {
    _vertexBuffer = nullptr;
    _indexBuffer = nullptr;
  }

  IMesh::IMesh(const MESH_DESC& desc)
    : _attributes(desc.vertexBuffer, desc.indexBuffer)
  {
  }

  IMesh::~IMesh()
  {
  }

  IMesh* IMesh::Create(const MESH_DESC& desc)
  {
    return CMeshManager::Instance()->CreateMesh(desc);
  }

  CMesh::CMesh(const MESH_DESC& desc)
    : IMesh(desc)
  {
  }

  CMesh::~CMesh()
  {
    Release();
  }

  bool CMesh::Initialize()
  {
    return true;
  }

  void CMesh::Release()
  {
  }
}