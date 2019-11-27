#include "EngineStdafx.h"
#include "Transform.h"

namespace Ellison_Engine
{
	using namespace DirectX;

  CTransform::CTransform(void)
    : _isTranslationChanged(true), _isRotationChanged(true), _isScaleChanged(true)
  {
		_translation = XMVectorSet(0, 0, 0, 1);
		_rotation = XMQuaternionRotationRollPitchYaw(0, 0, 0);
		_scale = XMVectorSet(1, 1, 1, 1);
  }

  CTransform::~CTransform(void)
  {
  }

	void CTransform::LocalTranslate(DirectX::XMFLOAT3 t)
	{
		_isTranslationChanged = true;

		XMMATRIX m;
		XMVECTOR v;

		m = XMMatrixRotationQuaternion(_rotation);
		v = XMVector3Transform(XMVectorSet(t.x, t.y, t.z, 1), m);

		_translation = XMVectorAdd(_translation, v);
	}

	void CTransform::LocalRotate(DirectX::XMFLOAT3 r)
	{
		_isRotationChanged = true;

		XMMATRIX m;
		XMVECTOR v;

		FLOAT rx = XMConvertToRadians(r.x);
		FLOAT ry = XMConvertToRadians(r.y);
		FLOAT rz = XMConvertToRadians(r.z);

		m = XMMatrixRotationQuaternion(_rotation);
		v = XMVector3Transform(XMVectorSet(rx, ry, rz, 1), m);

		XMVECTOR n = XMVector3Normalize(v);
		XMVECTOR l = XMVector3Length(v);

		if (XMVector3NotEqual(n, XMVectorZero()) == true)
		{
			XMVECTOR q = XMQuaternionRotationNormal(n, XMVectorGetX(l));

			_rotation = XMQuaternionMultiply(_rotation, q);
		}
		_rotation = XMQuaternionRotationRollPitchYaw(rx, ry, rz);
	}

	void CTransform::Translate(DirectX::XMFLOAT3 t)
	{
		_isTranslationChanged = true;

		XMVECTOR v = XMVectorSet(t.x, t.y, t.z, 0);

		_translation = XMVectorAdd(_translation, v);
	}

	void CTransform::Rotate(DirectX::XMFLOAT3 r)
	{
		_isRotationChanged = true;

		FLOAT rx = XMConvertToRadians(r.x);
		FLOAT ry = XMConvertToRadians(r.y);
		FLOAT rz = XMConvertToRadians(r.z);

		XMVECTOR q = XMQuaternionRotationRollPitchYaw(rx, ry, rz);

		if (XMVector3NotEqual(q, XMVectorZero()) == true)
		{
			_rotation = XMQuaternionMultiply(_rotation, q);
		}
	}

	void CTransform::Position(DirectX::XMFLOAT3 p)
	{
		_isTranslationChanged = true;

		_translation = XMVectorSet(p.x, p.y, p.z, 1);
	}

	void CTransform::Rotation(DirectX::XMFLOAT3 r)
	{
		_isRotationChanged = true;

		FLOAT rx = XMConvertToRadians(r.x);
		FLOAT ry = XMConvertToRadians(r.y);
		FLOAT rz = XMConvertToRadians(r.z);

		_rotation = XMQuaternionRotationRollPitchYaw(rx, ry, rz);
	}

	void CTransform::Scailing(DirectX::XMFLOAT3 s)
	{
		_isScaleChanged = true;

		_scale = XMVectorSet(s.x, s.y, s.z, 1);
	}

	XMMATRIX& CTransform::GetTranslateMatrix()
	{
		if (_isTranslationChanged)
		{
			_isTranslationChanged = false;
			_oldTranslationMatrix = XMMatrixTranslationFromVector(_translation);
		}

		return _oldTranslationMatrix;
	}

	XMMATRIX& CTransform::GetRotateMatrix()
	{
		if (_isRotationChanged)
		{
			_isRotationChanged = false;
			_oldRotationMatrix = XMMatrixRotationQuaternion(_rotation);
		}

		return _oldRotationMatrix;
	}

	XMMATRIX& CTransform::GetScailingMatrix()
	{
		if (_isScaleChanged)
		{
			_isScaleChanged = false;
			_oldScaleMatrix = XMMatrixScalingFromVector(_scale);
		}

		return _oldScaleMatrix;
	}

	XMMATRIX CTransform::GetTransformMatrix()
	{
		XMMATRIX m;

		m = GetScailingMatrix() * GetRotateMatrix() * GetTranslateMatrix();

		return m;
	}
}