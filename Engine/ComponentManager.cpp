#include "EngineStdafx.h"
#include "ComponentManager.h"

#include "Component.h"

namespace Ellison_Engine
{
  bool CComponentManager::_destroyed = false;
  CComponentManager* CComponentManager::_instance = nullptr;

  void CComponentManager::DeleteAllComponents()
  {
    for (auto component = _components.begin(); component != _components.end(); )
    {
      for (auto itr = component->second.begin(); itr != component->second.end(); )
      {
        if ((*itr))
        {
          (*itr).reset();
          itr = component->second.erase(itr);
        }
        else
        {
          ++itr;
        }
      }

      component = _components.erase(component);
    }
  }
}