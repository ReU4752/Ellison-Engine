#include "GraphicsStdafx.h"
#include "DX11ForwardRenderer.h"
#include "../Engine/RenderComponent.h"
#include "DX11GraphicsBufferManager.h"
#include "../Engine/Locator.h"
#include "../Core/MeshManager.h"

namespace Ellison_Engine
{
	using namespace DirectX;

	CDX11ForwardRenderer::CDX11ForwardRenderer()
	{
	}


	CDX11ForwardRenderer::~CDX11ForwardRenderer()
	{
	}

	CDX11ForwardRenderer* CDX11ForwardRenderer::Create(UINT screenWidth, UINT screenHeight, bool vsyncOn, bool fullscreen, HWND hWnd, float depth, float nearf)
	{
		CDX11ForwardRenderer* renderer = new CDX11ForwardRenderer;
		renderer->_screenWidth = screenWidth;
		renderer->_screenHeight = screenHeight;
		renderer->_vsyncOn = vsyncOn;
		renderer->_fullscreen = fullscreen;
		renderer->_hWnd = hWnd;
		renderer->_depth = depth;
		renderer->_near = nearf;
		renderer->Initialize();

		return renderer;
	}

	void CDX11ForwardRenderer::Render()
	{
		CCamera* mainCamera = CLocator::GetSceneManager()->GetMainCamera();
		mainCamera->_parent = _pivot;

		if (::GetAsyncKeyState('W') & 0x8000f)
			mainCamera->walk(4.0f);

		if (::GetAsyncKeyState('S') & 0x8000f)
			mainCamera->walk(-4.0f);

		if (::GetAsyncKeyState('A') & 0x8000f)
			mainCamera->strafe(-4.0f);

		if (::GetAsyncKeyState('D') & 0x8000f)
			mainCamera->strafe(4.0f);

		if (::GetAsyncKeyState('R') & 0x8000f)
			mainCamera->fly(4.0f);

		if (::GetAsyncKeyState('F') & 0x8000f)
			mainCamera->fly(-4.0f);

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			mainCamera->pitch(1.0f);

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			mainCamera->pitch(-1.0f);

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			mainCamera->yaw(-1.0f);

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			mainCamera->yaw(1.0f);

		if (::GetAsyncKeyState('N') & 0x8000f)
			mainCamera->roll(1.0f);

		if (::GetAsyncKeyState('M') & 0x8000f)
			mainCamera->roll(-1.0f);

		if (::GetAsyncKeyState('P') & 0x0001)
		{
			DirectX::XMFLOAT3 pos(-543.768311f, 152.f, 104.817230f);

			mainCamera->setPosition(&pos);
			mainCamera->pitch(3.50f);
			mainCamera->yaw(100.0f);
		}

		if (::GetAsyncKeyState('O') & 0x0001)
		{
			DirectX::XMFLOAT3 pos(-4, 0, -4);

			mainCamera->setPosition(&pos);
			mainCamera->yaw(45);
		}

		//if (::GetAsyncKeyState('L') & 0x0001)
		//{
		//	WCHAR str[500];
		//	XMFLOAT3 cpos;
		//	XMFLOAT3 right;
		//	XMFLOAT3 up;
		//	XMFLOAT3 look;
		//	mainCamera->getPosition(&cpos);
		//	mainCamera->getRight(&right);
		//	mainCamera->getUp(&up);
		//	mainCamera->getLook(&look);
		//	_snwprintf_s(str, lstrlen(str) - 1, L"%f | %f | %f . %f | %f | %f . %f | %f | %f . %f | %f | %f", cpos.x, cpos.y, cpos.z, right.x, right.y, right.z, up.x, up.y, up.z, look.x, look.y, look.z);
		//	SetWindowText(_hWnd, str);
		//}


		mainCamera->Update(_dxApp.GetDeviceContext());
		ID3D11Buffer* cameraMatrixBuffer = mainCamera->GetCameraMatrixBufferPtr();

		CSkybox* skybox = CLocator::GetSceneManager()->GetSkyBox();

		float color[4] = { mainCamera->GetBackgroundColor()[0], mainCamera->GetBackgroundColor()[1], mainCamera->GetBackgroundColor()[2], mainCamera->GetBackgroundColor()[3] };
		_dxApp.BeginScene(color[0], color[1], color[2], color[3]);

		XMMATRIX worldMat;
		worldMat = XMMatrixIdentity();

		//// SKYBOX
		XMFLOAT3 camPos;
		mainCamera->getPosition(&camPos);

		_dxApp.TurnZBufferOff();
		_dxApp.GetDeviceContext()->RSSetState(_dxApp.GetCullNoneRasterizerState());
		skybox->RenderSkyBox(_dxApp.GetDeviceContext(), worldMat, cameraMatrixBuffer, camPos);
		_dxApp.GetDeviceContext()->RSSetState(_dxApp.GetDefaultRasterizerState());
		_dxApp.TurnZBufferOn();
		//// SKYBOX

		RenderTree(CLocator::GetSceneManager()->GetRootNode(), cameraMatrixBuffer, worldMat);

		_dxApp.EndScene();
	}

	bool CDX11ForwardRenderer::Initialize()
	{
		_dxApp.Initialize(_screenWidth, _screenHeight, _vsyncOn, _hWnd, _fullscreen, _depth, _near);
		CLocator::Provide(&_dxApp);

		*CMeshManager::InstanceOriginal() = new CMeshManager();
		*CDX11GraphicsBufferManager::InstanceOriginal() = new CDX11GraphicsBufferManager(*_dxApp.GetDevice());

		_pivot = new CEmptyNode(XMFLOAT3(0, 0, 0));

		return true;
	}

	void CDX11ForwardRenderer::Release()
	{
		_dxApp.Shutdown();
	}

	void CDX11ForwardRenderer::RenderTree(CSceneNode* node, ID3D11Buffer* cameraMatrixBuffer, const DirectX::XMMATRIX& parentWorldMatrix)
	{
		XMMATRIX worldMatrix = node->_transform.GetTransformMatrix() * parentWorldMatrix;

		CRenderComponent* com = node->_data->GetComponent<CRenderComponent>();
		if (com)
		{
			com->Draw(_dxApp.GetDeviceContext(), cameraMatrixBuffer, worldMatrix);
		}

		for (auto itr : node->_childNodes)
		{
			RenderTree(itr, cameraMatrixBuffer, worldMatrix);
		}
	}
}