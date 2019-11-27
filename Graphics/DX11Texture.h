#pragma once

#include "../Core/Texture.h"

namespace Ellison_Engine
{
	class DX11Texture : public ITexture
	{
	public:
		DX11Texture(ID3D11Device& device, std::wstring path, const TEXTURE_DESC& desc);
		~DX11Texture();

		ID3D11ShaderResourceView* GetTexture() { return _texture; }

		static DX11Texture* Create(ID3D11Device& device, std::wstring path, const TEXTURE_DESC& desc);

	protected:
		bool Initialize() override;
		void Release() override;

	private:
		ID3D11Device& _device;
		std::wstring _path;
		ID3D11ShaderResourceView*  _texture;
	};
}