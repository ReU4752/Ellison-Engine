#pragma once

#include "../Core/Renderer.h"
#include "DirectX11App.h"
#include "../Engine/SceneManager.h"

namespace Ellison_Engine
{
	class CEmptyNode : public Ellison_Engine::CSceneNode
	{
	public:
		CEmptyNode(DirectX::XMFLOAT3 pos)
		{
			GetTransform().Position(pos);
		}
		virtual ~CEmptyNode() {}
	};

	class CDX11ForwardRenderer : public IRenderer
	{
	public:
		CDX11ForwardRenderer();
		~CDX11ForwardRenderer();

		static CDX11ForwardRenderer* Create(UINT screenWidth, UINT screenHeight, bool vsyncOn, bool fullscreen, HWND hWnd, float depth, float nearf);

		void Render() override;

	protected:
		bool Initialize() override;
		void Release() override;

	private:
		void RenderTree(CSceneNode* node, ID3D11Buffer* cameraMatrixBuffer, const DirectX::XMMATRIX& parentWorldMatrix);

	private:
		UINT _screenWidth;
		UINT _screenHeight;
		bool _vsyncOn;
		bool _fullscreen;
		HWND _hWnd;
		float _depth;
		float _near;

		CDirectX11App _dxApp;

		CSceneNode* _pivot;
	};
}