#pragma once

#include "ComponentManager.h"

namespace Ellison_Engine
{
  class CEntity
  {
  public:
    CEntity(void);
    virtual ~CEntity(void);

    virtual bool Initialize(void) = 0;
    virtual void Release(void) = 0;

    //////////////////////////////////////////////////
    // Entity Family Tree
    //////////////////////////////////////////////////

    virtual void SetParent(CEntity* entity);
    virtual void AddAsChild(CEntity* entity);

    virtual CEntity* GetParent(void) const;
    virtual CEntity* GetChild(UINT index) const;
    virtual std::vector<CEntity*> GetChildren(void);

    //////////////////////////////////////////////////
    // Entity Components
    //////////////////////////////////////////////////

    template <class component_t>
    void AddComponent(void);

    template <class component_t>
    component_t* GetComponent(void);

    void ReleaseAllComponents(void);

  private:
    CEntity* _parent;
    std::vector<CEntity*> _children;
    std::unordered_map<size_t, std::weak_ptr<CComponent>> _components;
  };

  template<class component_t>
  void CEntity::AddComponent(void)
  {
    if (this->_components.find(get_component<component_t>::type::GetComponentId()) != this->_components.end()) // 찾았는데 있으면
    {
      MessageBox(nullptr, L"Already had Component", L"Error", MB_OK);
    }

    std::weak_ptr<CComponent> component = std::move(CComponentManager::GetInstance().CreateComponent<component_t>(this));

    this->_components[get_component<component_t>::type::GetComponentId()] = component;
  }

  template<class component_t>
  component_t* CEntity::GetComponent(void)
  {
    size_t componentId = get_component<component_t>::type::GetComponentId();

    if (this->_components.find(componentId) == this->_components.end()) // 찾았는데 없거나
      return nullptr;

    std::shared_ptr<CComponent> com = _components[componentId].lock();
    if (com == nullptr) // 만료되었으면
    {
      _components.erase(componentId);
      return nullptr;
    }

    return reinterpret_cast<component_t*>(com.get());
  }
}