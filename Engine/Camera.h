#pragma once

#include "SceneNode.h"

namespace Ellison_Engine
{
  class CCamera : public CSceneNode
  {
  private:
    struct CameraMatrixBufferType
    {
			DirectX::XMMATRIX viewMatrix;
			DirectX::XMMATRIX projectionMatrix;
    };

  public:
    enum class CameraMode
    {
      PERSPECTIVE,
      ORTHOGRAPHIC
    };
		enum CameraType 
		{ 
			LANDOBJECT,
			AIRCRAFT 
		};

  public:
    CCamera(void);
    CCamera(const CCamera& other);
    ~CCamera(void);

    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const float viewportWidth, const float viewportHeight, bool isMainCamera, const float backgroundColor[4], CameraMode cameraMode);
    void Release(void);
    bool Update(ID3D11DeviceContext* deviceContext);

    ID3D11Buffer* GetCameraMatrixBufferPtr(void) const;
    D3D11_VIEWPORT* GetViewPortPtr(void);
    float* GetBackgroundColor(void);
		DirectX::XMMATRIX& GetProjectionMatrix(void);

		//////////////////////////////////////////////////

		void strafe(float units); // left/right
		void fly(float units);    // up/down
		void walk(float units);   // forward/backward

		void pitch(float angle); // rotate on right vector
		void yaw(float angle);   // rotate on up vector
		void roll(float angle);  // rotate on look vector

		void getViewMatrix(DirectX::XMMATRIX* outV);
		void setCameraType(CameraType cameraType);
		void getPosition(DirectX::XMFLOAT3* pos);
		void setPosition(DirectX::XMFLOAT3* pos);

		void getRight(DirectX::XMFLOAT3* right);
		void getUp(DirectX::XMFLOAT3* up);
		void getLook(DirectX::XMFLOAT3* look);

  private:
    bool _isMainCamera;
    float _backgroundColor[4];
    D3D11_VIEWPORT _viewport;

		CameraType  _cameraType;
		DirectX::XMVECTOR _right;
		DirectX::XMVECTOR _up;
		DirectX::XMVECTOR _look;
		DirectX::XMVECTOR _pos;

    float _fieldOfView;
    float _screenAspect;
    float _screenNear;
    float _screenDepth;
		DirectX::XMMATRIX _projectionMatrix;
    ID3D11Buffer* _viewMatrixBuffer;
  };
}