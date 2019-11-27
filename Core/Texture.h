#pragma once

namespace Ellison_Engine
{
	enum TextureType
	{
		TEX_TYPE_1D,
		TEX_TYPE_2D,
		TEX_TYPE_3D
	};

	enum PixelFormat
	{
		PF_R32,
		PF_R32G32,
		PF_R32G32B32,
		PF_R32G32B32A32
	};

	struct TEXTURE_DESC
	{
		TextureType type;

		PixelFormat format;

		uint32_t width;
		uint32_t height;
		uint32_t numMipMaps;
	};

	class CTextureAttributes
	{
	public:
    CTextureAttributes(const TEXTURE_DESC& desc);

    TextureType GetTextureType() const { return _attributes.type; }

    PixelFormat GetFormat() const { return _attributes.format; }

    uint32_t GetWidth() const { return _attributes.width; }

    uint32_t GetHeight() const { return _attributes.height; }

	protected:
    TEXTURE_DESC _attributes;
	};

  class ITexture
  {
  public:
		ITexture(const CTextureAttributes& desc);
    virtual ~ITexture();

    const CTextureAttributes& GetAttributes() { return _attributes; }

  protected:
    virtual bool Initialize() = 0;
    virtual void Release() = 0;

  protected:
    CTextureAttributes _attributes;
  };
}