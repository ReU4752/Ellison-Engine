#pragma once

#include "../Graphics/DirectX11App.h"
#include "SceneManager.h"
#include "Time.h"

namespace Ellison_Engine
{
  class CLocator
  {
  public:
    static CDirectX11App* GetDirect3D() 
    {
      return _direct3D;
    }
    static CSceneManager* GetSceneManager()
    {
      return _sceneManager;
    }
		static CTime* GetTime()
		{
			return _time;
		}

    static void Provide(CDirectX11App* direct3D)
    {
      _direct3D = direct3D;
    }
    static void Provide(CSceneManager* sceneManager)
    {
      _sceneManager = sceneManager;
    }
		static void Provide(CTime* time)
		{
			_time = time;
		}

  private:
    static CDirectX11App* _direct3D;
    static CSceneManager* _sceneManager;
		static CTime* _time;
  };
}