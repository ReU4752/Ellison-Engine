#pragma once

#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

class System
{
public:
  System();
  System(const System&) = delete;

  bool Init();
  void Run();
  void Release();

  LRESULT CALLBACK MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
  bool InitWindow(INT posX, INT posY);
  void ReleaseWindow();

private:
  LPTSTR m_appName;
  HWND m_hWnd;
  HINSTANCE m_hInst;
  INT m_width, m_height;
  bool m_appExit;
};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static System* g_system = nullptr;