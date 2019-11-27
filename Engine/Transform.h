#pragma once

namespace Ellison_Engine
{
  class CTransform
  {
  public:
    CTransform(void);
    ~CTransform(void);

		void LocalTranslate(DirectX::XMFLOAT3 t);
		void LocalRotate(DirectX::XMFLOAT3 r);

		void Translate(DirectX::XMFLOAT3 t);
		void Rotate(DirectX::XMFLOAT3 r);

		void Scailing(DirectX::XMFLOAT3 s);

		void Position(DirectX::XMFLOAT3 p);
		void Rotation(DirectX::XMFLOAT3 r);
	
		DirectX::XMMATRIX& GetTranslateMatrix();
		DirectX::XMMATRIX& GetRotateMatrix();
		DirectX::XMMATRIX& GetScailingMatrix();

		DirectX::XMMATRIX GetTransformMatrix();

  private:
    bool _isTranslationChanged, _isRotationChanged, _isScaleChanged;

    DirectX::XMVECTOR _translation, _rotation, _scale;

		DirectX::XMMATRIX _oldTranslationMatrix, _oldRotationMatrix, _oldScaleMatrix;
  };
}