#include "EngineStdafx.h"
#include "Component.h"
//#include "Entity.h"

namespace Ellison_Engine
{
  DECLARE_COMPONENT(CComponent)

  CComponent::CComponent(void)
    : _ownerEntity(nullptr)
  {
  }

  CComponent::CComponent(const CComponent& other)
  {
  }

  CComponent::~CComponent(void)
  {
  }

  void CComponent::SetOwner(CEntity* owner)
  {
    _ownerEntity = owner;
  }

  CEntity* CComponent::GetOwnerEntity(void) const
  {
    assert(_ownerEntity);
    return _ownerEntity;
  }
}