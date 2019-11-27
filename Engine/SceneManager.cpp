#include "EngineStdafx.h"
#include "SceneManager.h"
#include "System.h"

namespace Ellison_Engine
{
  CSceneManager::CSceneManager()
  {
  }

  CSceneManager::CSceneManager(const CSceneManager& other)
  {
  }

  CSceneManager::~CSceneManager()
  {
  }

  void CSceneManager::DeleteNode(CSceneNode* node)
  {
    if (node == nullptr)
      return;

    for (auto itr = node->_childNodes.begin(); itr != node->_childNodes.end(); )
    {
      if (*itr)
      {
        DeleteNode((*itr));
        itr = node->_childNodes.erase(itr);
      }
      else
      {
        ++itr;
      }
    }

    if (node != &_rootNode)
    {
      delete node;
      node = nullptr;
    }
  }

  void CSceneManager::ClearScene()
  {
    DeleteNode(&_rootNode);
  }

  void CSceneManager::AttachCamera(CCamera* camera)
  {
    _mainCamera = camera;
  }

	void CSceneManager::AttachSkyBox(CSkybox* skybox)
	{
		_skyBox = skybox;
	}

  CSceneNode* CSceneManager::GetRootNode(void)
  {
    return &_rootNode;
  }

	CCamera* CSceneManager::GetMainCamera(void)
  {
    return _mainCamera;
  }

	CSkybox* CSceneManager::GetSkyBox(void)
	{
		return _skyBox;
	}
}