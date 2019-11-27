#pragma once

#include "Mesh.h"

namespace Ellison_Engine
{
  class IMeshManager
  {
  public:
    IMeshManager();
    virtual ~IMeshManager();

    IMesh* CreateMesh(const MESH_DESC& desc);

		static IMeshManager** InstanceOriginal()
		{
			return &_instance;
		}

		static IMeshManager* Instance()
		{
			return _instance;
		}

  protected:
    virtual IMesh* MeshCreationHelper(const MESH_DESC& desc) = 0;
  
	private:
		static IMeshManager* _instance;
	};

  class CMeshManager : public IMeshManager
  {
  public:
    CMeshManager();
    ~CMeshManager();

  protected:
    IMesh* MeshCreationHelper(const MESH_DESC& desc) override;
  };
}