#include "EngineStdafx.h"
#include "AppFramework.h"

#include "WindowSystem.h"
#include "../Graphics/DX11ForwardRenderer.h"
#include "Locator.h"
#include "Time.h"

namespace Ellison_Engine
{
  HINSTANCE hInstance;

  CAppFramework::CAppFramework(void)
    : _sceneManager(nullptr)
  {
  }

  CAppFramework::CAppFramework(const CAppFramework& other)
  {
  }

  CAppFramework::~CAppFramework(void)
  {
  }

  bool CAppFramework::Initialize(std::wstring name, int screenWidth, int screenHeight)
  {
		_windows = CWindowSystem::Create(hInstance, L"Project R", name.c_str(), 0, 0, screenWidth, screenHeight);
    if (_windows == nullptr)
    {
      MessageBox(nullptr, L"System Initialize Failed", L"Main Application - Error", MB_OK);
      return false;
    }

		HWND hw = static_cast<CWindowSystem*>(_windows)->GetWindowHandle();
		_renderer = CDX11ForwardRenderer::Create(screenWidth, screenHeight, true, false, hw, 2000.0f, 0.01f);
		if (_renderer == nullptr)
		{
			MessageBox(nullptr, L"System Initialize Failed", L"Main Application - Error", MB_OK);
			return false;
		}

    _sceneManager = new CSceneManager;
    CLocator::Provide(_sceneManager);

		_time = new CTime;
		_time->Initialize();
		CLocator::Provide(_time);

    return true;
  }

  void CAppFramework::Release(void)
  { 
    if (_sceneManager)
    {
      delete _sceneManager;
      _sceneManager = nullptr;
    }

    //for (auto itr = _systems.begin(); itr != _systems.end(); ++itr)
    //{
    //  if ((*itr))
    //  {
    //    (*itr)->Release();
    //    delete (*itr);
    //    (*itr) = nullptr;
    //  }
    //}
  }

  void CAppFramework::Update(void)
  {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT)
    {
      if (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      else
      {
				_time->Update();

				_windows->Update();
				_renderer->Render();
        //for (auto itr : _systems)
        //{
        //  itr->Update();
        //}
      }
    }
  }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, INT cmdShow)
{
  /* Check Memory Leak */
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  Ellison_Engine::hInstance = hInstance;
  return Ellison_Engine::ApplicationMain();
}