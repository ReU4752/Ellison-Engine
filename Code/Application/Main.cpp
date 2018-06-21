#include <Windows.h>

#include "System.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
  System system;

  if (system.Init())
    system.Run();

  system.Release();

	return (INT)0;
}