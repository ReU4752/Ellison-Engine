#pragma once

#include "Component.h"
#undef new

namespace Ellison_Engine
{
  class CComponentManager final
  {
  public:
    static CComponentManager& GetInstance()
    {
      if (_destroyed)
      {
        new(_instance) CComponentManager;
        atexit(KillPhoenix);
        _destroyed = false;
      }
      else if (_instance == nullptr)
      {
        Create();
      }

      return *_instance;
    }

    template <class component_t>
    std::weak_ptr<CComponent> CreateComponent();
    template <class component_t>
    std::weak_ptr<CComponent> CreateComponent(CEntity* owner);

    template <class component_t>
    std::list<std::shared_ptr<CComponent>>* GetComponentListById();

    void DeleteAllComponents();

  private:
    ~CComponentManager()
    {
      _destroyed = true;
    }
    static void Create()
    {
      static CComponentManager instance;
      _instance = &instance;
    }
    static void KillPhoenix()
    {
      _instance->~CComponentManager();
    }

  private:
    static bool _destroyed;
    static CComponentManager* _instance;

    std::unordered_map<size_t, std::list<std::shared_ptr<CComponent>>> _components;
  };

  template<class component_t>
  std::weak_ptr<CComponent> CComponentManager::CreateComponent()
  {
    std::shared_ptr<CComponent> pReturn(new component_t, [](CComponent* com) { com->Release(); delete com; });
    (pReturn.get())->Initialize();

    _components[get_component<component_t>::type::GetComponentId()].push_back(pReturn);

    return std::weak_ptr<CComponent>(pReturn);
  }

  template<class component_t>
  std::weak_ptr<CComponent> CComponentManager::CreateComponent(CEntity* owner)
  {
    std::shared_ptr<CComponent> pReturn(new component_t, [](CComponent* com) { com->Release(); delete com; });
    (pReturn.get())->SetOwner(owner);
    (pReturn.get())->Initialize();

    _components[get_component<component_t>::type::GetComponentId()].push_back(pReturn);

    return std::weak_ptr<CComponent>(pReturn);
  }

  template<class component_t>
  std::list<std::shared_ptr<CComponent>>* CComponentManager::GetComponentListById()
  {
    return (&_components[get_component<component_t>::type::GetComponentId()]);;
  }
}