#pragma once

#include "../Graphics/DX11ForwardRenderer.h"

namespace Ellison_Engine
{
	class CSystem;
	class CTime;

  class CAppFramework
  {
  public: 
    CAppFramework(void);
    CAppFramework(const CAppFramework& other);
    virtual ~CAppFramework(void);

    virtual bool Initialize(std::wstring name, int screenWidth, int screenHeight);
    virtual void Release(void);
    virtual void Update(void);

  private:
    CSystem* _windows;
		IRenderer* _renderer;
		CTime* _time;

    CSceneManager* _sceneManager;
  };

  int ApplicationMain(void);
}