#include "CoreStdafx.h"
#include "Texture.h"

namespace Ellison_Engine
{
  CTextureAttributes::CTextureAttributes(const TEXTURE_DESC& desc)
    : _attributes(desc)
  {
  }

  ITexture::ITexture(const CTextureAttributes& desc)
    : _attributes(desc)
  {
  }

  ITexture::~ITexture()
  {
  }
}