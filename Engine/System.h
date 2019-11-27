#pragma once

namespace Ellison_Engine
{
  class CSystem
  {
  public:
    CSystem(void);
    CSystem(const CSystem& other);
    virtual ~CSystem(void);
     
    virtual bool Initialize(void) = 0;
    virtual void Update(void) = 0;
    virtual void Release(void) = 0;
  };
}