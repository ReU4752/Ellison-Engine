#pragma once

#include "Transform.h"
#include "Entity.h"

namespace Ellison_Engine
{
  class CSceneNode
  {
    friend class CSceneManager;
    friend class CDX11ForwardRenderer;

    public:
      CSceneNode();
      CSceneNode(const CSceneNode& other);
      virtual ~CSceneNode();

      CSceneNode* CreateChildSceneNode(void);
      void AttachEntity(CEntity* entity);

      CSceneNode* GetParent(void);
      std::vector<CSceneNode*>& GetChildNodes(void);
      CTransform& GetTransform(void);
      CEntity* GetEntity(void);

    private:
      CSceneNode* _parent;
      std::vector<CSceneNode*> _childNodes;
      CTransform _transform;
      CEntity* _data;
  };
}