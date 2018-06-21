#include "System.h"

System::System()
  : m_appName(L"EllisonEngine"), m_hWnd(NULL), m_width(800), m_height(600)
{
  m_hInst = GetModuleHandle(NULL);
}

bool System::Init()
{
  if (!InitWindow(200, 200))
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

bool System::InitWindow(INT posX, INT posY)
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
    
  m_hWnd = CreateWindow(wc.lpszClassName, m_appName, WS_OVERLAPPEDWINDOW, posX, posY, m_width, m_height, NULL, NULL, m_hInst, NULL);
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