#include "EngineStdafx.h"
#include "Camera.h"
#include "Locator.h"

namespace Ellison_Engine
{
	using namespace DirectX;

  CCamera::CCamera(void)
  {
    _viewMatrixBuffer = nullptr;

		_cameraType = LANDOBJECT;

		_pos = XMVectorSet(0.0f, 0.0f, 0.0f, 1);
		_right = XMVectorSet(1.0f, 0.0f, 0.0f, 1);
		_up = XMVectorSet(0.0f, 1.0f, 0.0f, 1);
		_look = XMVectorSet(0.0f, 0.0f, 1.0f, 1);
  }

  CCamera::CCamera(const CCamera& other)
  {
  }

  CCamera::~CCamera(void)
  {
  }

  bool CCamera::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const float viewportWidth, const float viewportHeight, bool isMainCamera, const float backgroundColor[4], CameraMode cameraMode)
  {
    HRESULT result;

    _isMainCamera = isMainCamera;
    memcpy(_backgroundColor, backgroundColor, sizeof(float) * 4);

    _viewport.Width = viewportWidth;
    _viewport.Height = viewportHeight;
    _viewport.MinDepth = 0.0f;
    _viewport.MaxDepth = 1.0f;
    _viewport.TopLeftX = 0.0f;
    _viewport.TopLeftY = 0.0f;

    deviceContext->RSSetViewports(1, &_viewport);

    if (cameraMode == CameraMode::PERSPECTIVE)
    {
      // Setup the projection matrix for Perspective.
      _fieldOfView = (float)D3DX_PI / 4.0f;
      _screenAspect = (float)viewportWidth / (float)viewportHeight;
      _screenNear = 0.01f;
      _screenDepth = 2000.0f;

			_projectionMatrix = XMMatrixPerspectiveFovLH(_fieldOfView, _screenAspect, _screenNear, _screenDepth);
			_projectionMatrix = XMMatrixTranspose(_projectionMatrix);
    }
    else
    {

    }

    D3D11_BUFFER_DESC viewBufferDesc;
    viewBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    viewBufferDesc.ByteWidth = sizeof(CameraMatrixBufferType);
    viewBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    viewBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    viewBufferDesc.MiscFlags = 0;
    viewBufferDesc.StructureByteStride = 0;

    result = device->CreateBuffer(&viewBufferDesc, NULL, &_viewMatrixBuffer);
    if FAILED(result)
    {
      return false;
    }

    return true;
  }

  void CCamera::Release(void)
  {
    if (_viewMatrixBuffer)
    {
      _viewMatrixBuffer->Release();
      _viewMatrixBuffer = 0;
    }
  }

  bool CCamera::Update(ID3D11DeviceContext* deviceContext)
  {
    HRESULT result;

		static float t = 0;
		t += 0.5f * CLocator::GetTime()->DeltaTime();

		_pos = XMVectorSet(cosf(t) * 743.768311f, 242.f, sinf(t) * 743.768311f, 1);
		XMVECTOR target = XMVectorSet(0, 102.f, 0, 1);
		XMVECTOR up = XMVectorSet(0, 1, 0, 1);

		XMMATRIX viewMatrix;
		//getViewMatrix(&viewMatrix);
		viewMatrix = DirectX::XMMatrixLookAtLH(_pos, target, up);

		viewMatrix = XMMatrixTranspose(viewMatrix);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    result = deviceContext->Map(_viewMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if FAILED(result)
    {
      return false;
    }

    CameraMatrixBufferType* dataPtr;
    dataPtr = (CameraMatrixBufferType*)mappedResource.pData;

    dataPtr->viewMatrix = viewMatrix;
    dataPtr->projectionMatrix = _projectionMatrix;

		deviceContext->Unmap(_viewMatrixBuffer, 0);

    return true;
  } 

  ID3D11Buffer* CCamera::GetCameraMatrixBufferPtr(void) const
  {
    return _viewMatrixBuffer;
  }

  D3D11_VIEWPORT* CCamera::GetViewPortPtr(void)
  {
    return &_viewport;
  }

  float* CCamera::GetBackgroundColor(void)
  {
    return _backgroundColor;
  }

	DirectX::XMMATRIX& CCamera::GetProjectionMatrix(void)
  {
    return _projectionMatrix;
  }

	void CCamera::getPosition(DirectX::XMFLOAT3* pos)
	{
		XMStoreFloat3(pos, _pos);
	}

	void CCamera::setPosition(DirectX::XMFLOAT3* pos)
	{
		_pos = XMLoadFloat3(pos);
	}

	void CCamera::getRight(DirectX::XMFLOAT3* right)
	{
		XMStoreFloat3(right, _right);
	}

	void CCamera::getUp(DirectX::XMFLOAT3* up)
	{
		_up = XMLoadFloat3(up);
	}

	void CCamera::getLook(DirectX::XMFLOAT3* look)
	{
		XMStoreFloat3(look, _look);
	}

	void CCamera::walk(float units)
	{
		// move only on xz plane for land object
		if (_cameraType == LANDOBJECT)
			_pos += XMVectorScale(XMVectorSet(XMVectorGetX(_look), 0.0f, XMVectorGetZ(_look), 1), units);

		if (_cameraType == AIRCRAFT)
			_pos += _look * units;
	}

	void CCamera::strafe(float units)
	{
		// move only on xz plane for land object
		if (_cameraType == LANDOBJECT)
			_pos += XMVectorScale(XMVectorSet(XMVectorGetX(_right), 0.0f, XMVectorGetZ(_right), 1), units);

		if (_cameraType == AIRCRAFT)
			_pos += _right * units;
	}

	void CCamera::fly(float units)
	{
		// move only on y-axis for land object
		if (_cameraType == LANDOBJECT)
		{
			XMFLOAT3 pos;
			XMStoreFloat3(&pos, _pos);
			pos.y += units;

			_pos = XMLoadFloat3(&pos);
		}

		if (_cameraType == AIRCRAFT)
			_pos += _up * units;
	}

	void CCamera::pitch(float angle)
	{
		angle = XMConvertToRadians(angle);

		XMMATRIX T;
		T = XMMatrixRotationAxis(_right, angle);
		
		_up = XMVector3TransformCoord(_up, T);
		_look = XMVector3TransformCoord(_look, T);
	}

	void CCamera::yaw(float angle)
	{
		angle = XMConvertToRadians(angle);

		XMMATRIX T;

		// rotate around world y (0, 1, 0) always for land object
		if (_cameraType == LANDOBJECT)
			T = XMMatrixRotationY(angle);

		// rotate around own up vector for aircraft
		if (_cameraType == AIRCRAFT)
			T = XMMatrixRotationAxis(_up, angle);

		// rotate _right and _look around _up or y-axis
		_right = XMVector3TransformCoord(_right, T);
		_look = XMVector3TransformCoord(_look, T);
	}

	void CCamera::roll(float angle)
	{
		angle = XMConvertToRadians(angle);

		// only roll for aircraft type
		if (_cameraType == AIRCRAFT)
		{
			XMMATRIX T;
			T = XMMatrixRotationAxis(_look, angle);

			// rotate _up and _right around _look vector
			_right = XMVector3TransformCoord(_right, T);
			_up = XMVector3TransformCoord(_up, T);
		}
	}

	void CCamera::getViewMatrix(XMMATRIX* outV)
	{
		XMFLOAT4X4 V;

		// Keep camera's axes orthogonal to eachother
		_look = XMVector3Normalize(_look);

		_up = XMVector3Cross(_look, _right);
		_up = XMVector3Normalize(_up);

		_right = XMVector3Cross(_up, _look);
		_right = XMVector3Normalize(_right);

		// Build the view matrix:
		float x = -XMVectorGetX(XMVector3Dot(_right, _pos));
		float y = -XMVectorGetX(XMVector3Dot(_up, _pos));
		float z = -XMVectorGetX(XMVector3Dot(_look, _pos));

		XMFLOAT3 right;
		XMFLOAT3 up;
		XMFLOAT3 look;

		XMStoreFloat3(&right, _right);
		XMStoreFloat3(&up, _up);
		XMStoreFloat3(&look, _look);
		
		(V)(0, 0) = right.x; (V)(0, 1) = up.x; (V)(0, 2) = look.x; (V)(0, 3) = 0.0f;
		(V)(1, 0) = right.y; (V)(1, 1) = up.y; (V)(1, 2) = look.y; (V)(1, 3) = 0.0f;
		(V)(2, 0) = right.z; (V)(2, 1) = up.z; (V)(2, 2) = look.z; (V)(2, 3) = 0.0f;
		(V)(3, 0) = x;        (V)(3, 1) = y;     (V)(3, 2) = z;       (V)(3, 3) = 1.0f;

		*outV = XMLoadFloat4x4(&V);
	}

	void CCamera::setCameraType(CameraType cameraType)
	{
		_cameraType = cameraType;
	}

  //void CCamera::CalculateViewMatrix(void)
  //{
  //  CSceneNode* parentNode;
		//XMMATRIX parentMatrix;
		//parentMatrix = XMMatrixIdentity();

  //  while ((parentNode = this->GetParent()) != nullptr)
  //  {
  //    parentMatrix *= parentNode->GetTransform().GetTransformMatrix();
  //  }

		//XMMATRIX T = GetTransform().GetTranslateMatrix();
		//XMMATRIX R = GetTransform().GetRotateMatrix();
		//XMVECTOR pos = XMVectorSet(0, 0, 0, 1); //카메라 로칼 원점
		//XMVECTOR target = XMVectorSet(0, 0, 1, 1); //카메라 로칼 시선
		//XMVECTOR up = XMVectorSet(0, 1, 0, 1); //카메라 로칼 위방향

		//pos = XMVector3Transform(pos, T);
		//target = XMVector3Transform(target, R);
		//up = XMVector3Transform(up, R);

		//_viewMatrix = XMMatrixLookAtLH(pos, target, up) * parentMatrix;

		////XMMATRIX translationMatrix = GetTransform().GetTransformMatrix();

		////XMFLOAT4X4 transformFloatMatrix;
		////XMStoreFloat4x4(&transformFloatMatrix, translationMatrix);
		////transformFloatMatrix._41 = -transformFloatMatrix._41; 
		////transformFloatMatrix._42 = -transformFloatMatrix._42; 
		////transformFloatMatrix._43 = -transformFloatMatrix._43;

		////translationMatrix = XMLoadFloat4x4(&transformFloatMatrix);

		////XMMATRIX rotationMatrix = GetTransform().GetRotateMatrix();

  ////  _viewMatrix = rotationMatrix * translationMatrix * parentMatrix;
  //}
}