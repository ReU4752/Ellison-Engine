#include "EngineStdafx.h"
#include "WindowSystem.h"
#include "../SandBox/resource.h"

namespace Ellison_Engine
{
  static CWindowSystem* windowSystem = nullptr;

  CWindowSystem::CWindowSystem(void)
    : _hInstance(nullptr), _hWnd(nullptr), _className(), _gameName(),
    _style(0), _posX(0), _posY(0), _sizeX(0), _sizeY(0)
  {
  }

  CWindowSystem::CWindowSystem(const CWindowSystem& other)
  {
  }

  CWindowSystem::~CWindowSystem(void)
  {
  }

  CWindowSystem* CWindowSystem::Create(HINSTANCE hInstance, LPCWSTR className, LPCWSTR gameName, INT32 posX, INT32 posY, INT32 sizeX, INT32 sizeY)
  {
    CWindowSystem* temp = new CWindowSystem;
    temp->_hInstance = hInstance;

    temp->_className = className;
    temp->_gameName = gameName;
    temp->_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    temp->_posX = posX;
    temp->_posY = posY;
    temp->_sizeX = sizeX;
    temp->_sizeY = sizeY;

    if (!temp->Initialize())
    {
      temp->Release();
      delete temp;
      temp = nullptr;
    }

    return temp;
  }

  LRESULT CWindowSystem::WndProc(HWND hWnd, UINT32 message, WPARAM wParam, LPARAM lParam)
  {
    switch (message)
    {
		case WM_CREATE:
			return 0;

    case WM_CLOSE:
      PostQuitMessage(0);
      return 0;
    }

    return ::DefWindowProc(hWnd, message, wParam, lParam);
  }

  bool CWindowSystem::Initialize(void)
  {
    EncryptionClassName();

     if (!MakeWindow())
      return false;

    return true;
  }

  void CWindowSystem::Update(void)
  {
  }

  void CWindowSystem::Release(void)
  {
    UnregisterClass(_className, _hInstance);
    DestroyWindow(_hWnd);

    delete _className;
    _className = 0;
  }

  HWND CWindowSystem::GetWindowHandle(void)
  {
    return _hWnd;
  }

  void CWindowSystem::EncryptionClassName()
  {
    LPWSTR encryptionStr;
    encryptionStr = new WCHAR[lstrlen(_className) + 1];
    lstrcpy(encryptionStr, _className);

    for (int i = 0; i < lstrlen(encryptionStr); i++)
    {
      encryptionStr[i] = encryptionStr[i] + 4;
    }

    _className = encryptionStr;
  }

  bool CWindowSystem::MakeWindow(void)
  {
		windowSystem = this;

    if (_hWnd != nullptr)
    {
      MessageBox(_hWnd, L"Already had made window", L"WindowSystem - MakeWindow()", MB_OK);
      return false;
    }

    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = _hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = _className;
    wc.cbSize = sizeof(WNDCLASSEX);

    RegisterClassEx(&wc);

    RECT wr = { 0, 0, _sizeX, _sizeY };
    AdjustWindowRect(&wr, _style, FALSE);

    _hWnd = ::CreateWindowEx(WS_EX_APPWINDOW, _className, _gameName, _style, _posX, _posY, wr.right - wr.left, wr.bottom - wr.top, NULL, nullptr, _hInstance, nullptr);
    if (!_hWnd)
    {
      MessageBox(_hWnd, L"CreateWindow() - Failed", L"WindowSystem - MakeWindow()", MB_OK);
      return false;
    }

    ShowWindow(_hWnd, SW_SHOW);
    SetForegroundWindow(_hWnd);
    SetFocus(_hWnd);
    UpdateWindow(_hWnd);

    // Set out pointer

    return true;
  }
}