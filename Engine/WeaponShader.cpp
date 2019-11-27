#include "EngineStdafx.h"
#include "WeaponShader.h"

namespace Ellison_Engine
{
	CWeaponShader::CWeaponShader()
	{
	}

	CWeaponShader::~CWeaponShader()
	{
	}

	bool CWeaponShader::Initialize(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename)
	{
		CShader::Initialize(device, vsFilename, psFilename);

		CreateConstantBuffer(device);

		////////////////
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		if (FAILED(device->CreateSamplerState(&samplerDesc, &_sampleState)))
		{
			return false;
		}
		///////////

		_ambientDown = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		_ambientUp = DirectX::XMFLOAT3(0.15f, 0.15f, 0.15f);

		return true;
	}

	void CWeaponShader::Release()
	{
		CShader::Release();
		if (_dirLightAttribute)
		{
			_dirLightAttribute->Release();
			_dirLightAttribute = nullptr;
		}
		if (_sampleState)
		{
			_sampleState->Release();
			_sampleState = nullptr;
		}
	}

	void CWeaponShader::Render(ID3D11DeviceContext* deviceContext, UINT indexCount, DirectX::XMMATRIX worldMatrix, ID3D11Buffer* cameraMatrixBuffer)
	{
		HRESULT result;

		D3D11_MAPPED_SUBRESOURCE mappedResource;

		////////////////////////////////////
		result = deviceContext->Map(_objectMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if FAILED(result)
		{
			return;
		}

		ObjectMatrixBufferType* dataPtr;
		dataPtr = (ObjectMatrixBufferType*)mappedResource.pData;

		worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
		dataPtr->world = worldMatrix;

		deviceContext->Unmap(_objectMatrixBuffer, 0);
		////////////////////////////////////
		result = deviceContext->Map(_dirLightAttribute, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if FAILED(result)
		{
			return;
		}

		DirLightAttribute* dataPtr2;
		dataPtr2 = (DirLightAttribute*)mappedResource.pData;

		dataPtr2->ambientDown = GammaToLinear(_ambientDown);
		dataPtr2->ambientRange = DirectX::XMVectorSubtract(GammaToLinear(_ambientUp), GammaToLinear(_ambientDown));

		deviceContext->Unmap(_dirLightAttribute, 0);
		////////////////////////////////////

		///////////////
		unsigned int bufferNumber;
		bufferNumber = 0;
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &cameraMatrixBuffer);
		bufferNumber = 1;
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_objectMatrixBuffer);

		deviceContext->PSSetConstantBuffers(0, 1, &_dirLightAttribute);

		ID3D11ShaderResourceView* diffuse = _diffuseTexture->GetTexture();
		deviceContext->PSSetShaderResources(0, 1, &diffuse);

		ID3D11ShaderResourceView* normal = _normalTexture->GetTexture();
		deviceContext->PSSetShaderResources(1, 1, &normal);
		///////////////

		deviceContext->PSSetSamplers(0, 1, &_sampleState);

		deviceContext->IASetInputLayout(_inputLayout);

		deviceContext->VSSetShader(_vertexShader, NULL, 0);
		deviceContext->PSSetShader(_pixelShader, NULL, 0);

		deviceContext->DrawIndexed(indexCount, 0, 0);
	}

	void CWeaponShader::SetDiffuseTexture(DX11Texture* texture)
	{
		_diffuseTexture = texture;
	}

	void CWeaponShader::SetNormalTexture(DX11Texture* texture)
	{
		_normalTexture = texture;
	}

	bool CWeaponShader::CreateConstantBuffer(ID3D11Device* device)
	{
		HRESULT result;

		D3D11_BUFFER_DESC lightAttribute;
		lightAttribute.Usage = D3D11_USAGE_DYNAMIC;
		lightAttribute.ByteWidth = sizeof(DirLightAttribute);
		lightAttribute.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lightAttribute.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightAttribute.MiscFlags = 0;
		lightAttribute.StructureByteStride = 0;

		result = device->CreateBuffer(&lightAttribute, NULL, &_dirLightAttribute);
		if FAILED(result)
		{
			return false;
		}

		return true;
	}

	bool CWeaponShader::CreateInputLayout(ID3D11Device* device, ID3D10Blob* vsBuffer)
	{
		HRESULT result;

		D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

		polygonLayout[2].SemanticName = "NORMAL";
		polygonLayout[2].SemanticIndex = 0;
		polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[2].InputSlot = 0;
		polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[2].InstanceDataStepRate = 0;

		polygonLayout[3].SemanticName = "TANGENT";
		polygonLayout[3].SemanticIndex = 0;
		polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[3].InputSlot = 0;
		polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[3].InstanceDataStepRate = 0;

		polygonLayout[4].SemanticName = "BINORMAL";
		polygonLayout[4].SemanticIndex = 0;
		polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[4].InputSlot = 0;
		polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[4].InstanceDataStepRate = 0;

		unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

		result = device->CreateInputLayout(polygonLayout, numElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(),
			&_inputLayout);
		if FAILED(result)
		{
			return false;
		}

		return true;
	}
}