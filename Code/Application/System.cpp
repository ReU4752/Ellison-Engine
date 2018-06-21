#include "System.h"

System::System()
  : m_appName(L"EllisonEngine"), m_hWnd(NULL), m_width(800), m_height(600)
{
  m_hInst = GetModuleHandle(NULL);
}

bool System::Init()
{
  WINDOW_DESC wDesc;
  wDesc.width = 800;
  wDesc.height = 600;
  wDesc.isFullscreen = false;
  wDesc.drawTitle = true;
  wDesc.adjustWindow = true;

  if (!InitWindow(wDesc))
  {
    return false;
  }

  return true;
}

void System::Run()
{
  MSG msg;
  ZeroMemory(&msg, sizeof(msg));

  while (!m_appExit)
  {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    if (msg.message == WM_QUIT)
      m_appExit = true;
  }
}

void System::Release()
{
  ReleaseWindow();
}

LRESULT CALLBACK System::MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  return DefWindowProc(hWnd, message, wParam, lParam);
}

bool System::InitWindow(const WINDOW_DESC& desc)
{
  g_system = this;

  WNDCLASS wc;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = m_hInst;
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = m_appName;

  RegisterClass(&wc);
  
  // Why use EnumDisplaySettings() than GetSystemMetrics()
  // Because, GetSystemMetrics() function return virtual resolution. (Vista or higher OS.)
  // You can see the scaling option in display setting.
  // https://code-examples.net/ko/q/46aafc
  // https://social.msdn.microsoft.com/Forums/ko-KR/0a83bb55-31f3-4298-acbb-7ac1f48e3cad/why-getsystemmetricssmcxscreen-returns-1280-on-surface-pro-instead-of-1920?forum=winappswithnativecode
  DEVMODE devMode;
  EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
  const DWORD desktopResolutionWidth = devMode.dmPelsWidth;
  const DWORD desktopResolutionHeight = devMode.dmPelsHeight;

  INT applyWidth = desktopResolutionWidth;
  INT applyHeight = desktopResolutionHeight;

  INT posX = 0;
  INT posY = 0;

  DWORD dwStyle = 0;
  if (desc.drawTitle)
    dwStyle = WS_OVERLAPPEDWINDOW;
  else
    dwStyle = WS_POPUP;

  if (desc.isFullscreen)
  {
    DEVMODE dmScreenSettings;
    memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
    dmScreenSettings.dmSize = sizeof(dmScreenSettings);
    dmScreenSettings.dmPelsWidth = desktopResolutionWidth;
    dmScreenSettings.dmPelsHeight = desktopResolutionHeight;
    dmScreenSettings.dmBitsPerPel = 32;
    dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
    ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

    posX = posY = 0;
  }
  else
  {
    RECT rcClient = RECT{ 0, 0,  desc.width, desc.height };

    if (desc.adjustWindow)
      AdjustWindowRect(&rcClient, dwStyle, FALSE);

    applyWidth = rcClient.right - rcClient.left;
    applyHeight = rcClient.bottom - rcClient.top;

    posX = (GetSystemMetrics(SM_CXSCREEN) - applyWidth) / 2;
    posY = (GetSystemMetrics(SM_CYSCREEN) - applyHeight) / 2;
  }

  m_hWnd = CreateWindow(wc.lpszClassName, m_appName, dwStyle, posX, posY, applyWidth, applyHeight, NULL, NULL, m_hInst, NULL);
  if (m_hWnd == NULL)
    return false;

  ShowWindow(m_hWnd, SW_SHOW);

  return true;
}

void System::ReleaseWindow()
{
  DestroyWindow(m_hWnd);
  m_hWnd = NULL;

  UnregisterClass(m_appName, m_hInst);
  m_hInst = NULL;

  g_system = NULL;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }

  return g_system->MessageHandler(hWnd, message, wParam, lParam);
}