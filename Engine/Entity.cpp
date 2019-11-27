#include "EngineStdafx.h"
#include "Entity.h"

namespace Ellison_Engine
{
  CEntity::CEntity(void)
    : _parent(nullptr)
  {
  }

  CEntity::~CEntity(void)
  { 
    _parent = nullptr;
    ReleaseAllComponents();
  }

  void CEntity::SetParent(CEntity* entity)
  {
    _parent = entity;
    entity->AddAsChild(this);
  }

  void CEntity::AddAsChild(CEntity* entity)
  {
    _children.push_back(entity);
    entity->SetParent(this);
  }

  CEntity* CEntity::GetParent(void) const
  {
    return _parent;
  }

  CEntity* CEntity::GetChild(UINT index) const
  {
    return _children[index];
  }

  std::vector<CEntity*> CEntity::GetChildren(void)
  {
    return _children;
  }

  void CEntity::ReleaseAllComponents(void)
  {
    for (auto itr : _components)
    {
      itr.second.reset();
    }
    _components.clear();
  }
}