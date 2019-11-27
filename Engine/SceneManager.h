#pragma once

#include "SceneNode.h"

#include "Camera.h"
#include "Skybox.h"

namespace Ellison_Engine
{
  class CSceneManager final
  {
  public:
    CSceneManager();
    CSceneManager(const CSceneManager& other);
    ~CSceneManager();

    void DeleteNode(CSceneNode* node);
    void ClearScene();

    void AttachCamera(CCamera* camera);
		void AttachSkyBox(CSkybox* skybox);

    CSceneNode* GetRootNode(void);
    CCamera* GetMainCamera(void);
		CSkybox* GetSkyBox(void);

  private:
    CSceneNode _rootNode;
    CCamera* _mainCamera;
		CSkybox* _skyBox;
  };
}