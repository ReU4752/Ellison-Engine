#include "EngineStdafx.h"
#include "SceneNode.h"

namespace Ellison_Engine
{
  CSceneNode::CSceneNode()
    : _parent(nullptr), _data(nullptr)
  {
  }

  CSceneNode::CSceneNode(const CSceneNode& other)
  {
  }

  CSceneNode::~CSceneNode()
  {
  }

  CSceneNode* CSceneNode::CreateChildSceneNode(void)
  {
    CSceneNode* newNode = new CSceneNode;

    _childNodes.push_back(newNode);

    return newNode;
  }

  void CSceneNode::AttachEntity(CEntity* entity)
  {
    _data = entity;
  }

  CSceneNode* CSceneNode::GetParent(void)
  {
    return nullptr;
  }

  std::vector<CSceneNode*>& CSceneNode::GetChildNodes(void)
  {
    return _childNodes;
  }

  CTransform& CSceneNode::GetTransform(void)
  {
    return _transform;
  }

  CEntity* CSceneNode::GetEntity(void)
  {
    assert(_data);
    return _data;
  }
}