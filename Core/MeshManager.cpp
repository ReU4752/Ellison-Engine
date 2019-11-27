#include "CoreStdafx.h"
#include "MeshManager.h"

namespace Ellison_Engine
{
	IMeshManager* IMeshManager::_instance = nullptr;

  IMeshManager::IMeshManager()
  {
  }

  IMeshManager::~IMeshManager()
  {
  }

  IMesh* IMeshManager::CreateMesh(const MESH_DESC& desc)
  {
    IMesh* mesh = MeshCreationHelper(desc);
    mesh->Initialize();

    return mesh;
  }

  CMeshManager::CMeshManager()
  {
  }

  CMeshManager::~CMeshManager()
  {
  }

  IMesh* CMeshManager::MeshCreationHelper(const MESH_DESC& desc)
  {
    CMesh* mesh = new CMesh(desc);
    
    return mesh;
  }
}