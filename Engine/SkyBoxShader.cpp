#include "EngineStdafx.h"
#include "SkyBoxShader.h"

namespace Ellison_Engine
{
	CSkyBoxShader::CSkyBoxShader()
	{
	}

	CSkyBoxShader::~CSkyBoxShader()
	{
	}

	bool CSkyBoxShader::Initialize(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename)
	{
		CShader::Initialize(device, vsFilename, psFilename);

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

		return true;
	}

	void CSkyBoxShader::Release()
	{
		CShader::Release();
		if (_sampleState)
		{
			_sampleState->Release();
			_sampleState = nullptr;
		}
	}

	void CSkyBoxShader::Render(ID3D11DeviceContext* deviceContext, UINT indexCount, DirectX::XMMATRIX worldMatrix, ID3D11Buffer* cameraMatrixBuffer)
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

		///////////////
		unsigned int bufferNumber;
		bufferNumber = 0;
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &cameraMatrixBuffer);
		bufferNumber = 1;
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_objectMatrixBuffer);

		ID3D11ShaderResourceView* skybox = _skyboxTexture->GetTexture();
		deviceContext->PSSetShaderResources(0, 1, &skybox);
		///////////////

		deviceContext->PSSetSamplers(0, 1, &_sampleState);

		deviceContext->IASetInputLayout(_inputLayout);

		deviceContext->VSSetShader(_vertexShader, NULL, 0);
		deviceContext->PSSetShader(_pixelShader, NULL, 0);

		deviceContext->DrawIndexed(indexCount, 0, 0);
	}

	void CSkyBoxShader::SetSkyBoxTexture(DX11Texture* texture)
	{
		_skyboxTexture = texture;
	}

	bool CSkyBoxShader::CreateInputLayout(ID3D11Device* device, ID3D10Blob* vsBuffer)
	{
		HRESULT result;

		D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

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