#include <Engine\EngineStdafx.h>
#include <Engine\AppFramework.h>
#include <Engine\Locator.h> 
#include <Engine\Camera.h>
#include <Engine\Skybox.h>
#include "TestEntity.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

using namespace Ellison_Engine;

class CTestScene
{
public:
  bool Initialize()
  {
    CSceneManager* scenemanager = CLocator::GetSceneManager();

		////////////// Camera
    float color[4] = { 0.192156f, 0.301960f, 0.874509f, 1.0f };
    if (!_mainCamera.Initialize(CLocator::GetDirect3D()->GetDevice(), CLocator::GetDirect3D()->GetDeviceContext(), SCREEN_WIDTH, SCREEN_HEIGHT, false, color, CCamera::CameraMode::PERSPECTIVE))
    {
      return false;
    }

		//_mainCamera.GetTransform().Position(DirectX::XMFLOAT3(0, 0, -10.0f));
    _mainCamera.GetTransform().Translate(DirectX::XMFLOAT3(-543.768311f, 152.f, 104.817230f));
		//_mainCamera.GetTransform().Rotate(DirectX::XMFLOAT3(-15, 0, 0));

    scenemanager->AttachCamera(&_mainCamera);
		////////////// Camera

		////////////// SkyBox
		_skyBox.Initialize(CLocator::GetDirect3D()->GetDevice(), CLocator::GetDirect3D()->GetDeviceContext());
		scenemanager->AttachSkyBox(&_skyBox);
		////////////// SkyBox

		////////////// Entity
    if (!_entity.Initialize())
    {
       return false;
    }
    scenemanager->GetRootNode()->AttachEntity(&_entity);
		////////////// Entity

    return true;
  }

  void Release()
  {
    _entity.Release();
		_skyBox.Release();
    _mainCamera.Release();
  }

private:
  CCamera _mainCamera;
	CSkybox _skyBox;
  CTestEntity _entity;
};

class CHello : public CAppFramework
{
public:
  virtual bool Initialize()
  {
    if (!CAppFramework::Initialize(L"Ellison - Sandbox Project", SCREEN_WIDTH, SCREEN_HEIGHT))
    {
      return false;
    }

    _scene = new CTestScene;
    _scene->Initialize();

    return true;
  }

  virtual void Release()
  {
    _scene->Release();
    delete _scene;
    _scene = nullptr;

    CAppFramework::Release();
  }

private:
  CTestScene* _scene;
};

int Ellison_Engine::ApplicationMain(void)
{
  CHello app;

  if(app.Initialize())
  {
    app.Update();
  }

  app.Release();

	FreeConsole();

  return 0;
}