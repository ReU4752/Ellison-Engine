#include "GraphicsStdafx.h"
#include "DX11Texture.h"

namespace Ellison_Engine
{
	DX11Texture::DX11Texture(ID3D11Device& device, std::wstring path, const TEXTURE_DESC& desc)
		: ITexture(desc), _path(path), _device(device)
	{
	}

	DX11Texture::~DX11Texture()
	{
	}

	DX11Texture* DX11Texture::Create(ID3D11Device& device, std::wstring path, const TEXTURE_DESC& desc)
	{
		DX11Texture* tex = new DX11Texture(device, path, desc);

		tex->Initialize();

		return tex;
	}

	bool DX11Texture::Initialize()
	{
		HRESULT result;

		// Load the texture in.
		result = D3DX11CreateShaderResourceViewFromFile(&_device, _path.c_str(), NULL, NULL, &_texture, NULL);
		if (FAILED(result))
		{
			return false;
		}

		return true;
	}

	void DX11Texture::Release()
	{
		if (_texture)
		{
			_texture->Release();
			_texture = 0;
		}
	}
}