#pragma once

#include "System.h"

namespace Ellison_Engine
{
  class CWindowSystem : public CSystem
  {
  public:
    CWindowSystem(void);
    CWindowSystem(const CWindowSystem& other);
    virtual ~CWindowSystem(void);

    static CWindowSystem* Create(HINSTANCE hInstance, LPCWSTR className, LPCWSTR gameName, INT32 posX, INT32 posY, INT32 sizeX, INT32 sizeY);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT32 message, WPARAM wParam, LPARAM lParam);

    virtual bool Initialize(void) override;
    virtual void Update(void) override;
    virtual void Release(void) override;

    HWND GetWindowHandle(void);

  private:
    void EncryptionClassName();
    bool MakeWindow(void);

  private:
    HINSTANCE _hInstance;
    HWND _hWnd;

    LPCWSTR _className;
    LPCWSTR _gameName;
    DWORD _style;
    INT32 _posX;
    INT32 _posY;
    INT32 _sizeX;
    INT32 _sizeY;
  };
}