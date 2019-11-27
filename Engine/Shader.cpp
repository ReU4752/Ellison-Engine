#include "EngineStdafx.h"
#include "Shader.h"

namespace Ellison_Engine
{
  CShader::CShader()
  {
    _vertexShader = nullptr;
    _pixelShader = nullptr;
    _inputLayout = nullptr;
    _objectMatrixBuffer = nullptr;
  }

  CShader::CShader(const CShader& other)
  {
  }

  CShader::~CShader()
  {
  }

  bool CShader::Initialize(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename)
  {
    assert(device);
    if (!InitializeShader(device, vsFilename, psFilename))
    {
      return false;
    }

    return true;
  }

  void CShader::Release()
  {
    ReleaseShader();
  }

  void CShader::Render(ID3D11DeviceContext* deviceContext, UINT indexCount, DirectX::XMMATRIX worldMatrix, ID3D11Buffer* cameraMatrixBuffer)
  {
    HRESULT result;

    D3D11_MAPPED_SUBRESOURCE mappedResource;
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

    unsigned int bufferNumber;
    bufferNumber = 0;
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &cameraMatrixBuffer);
    bufferNumber = 1;
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_objectMatrixBuffer);

    deviceContext->IASetInputLayout(_inputLayout);

    deviceContext->VSSetShader(_vertexShader, NULL, 0);
    deviceContext->PSSetShader(_pixelShader, NULL, 0);

    deviceContext->DrawIndexed(indexCount, 0, 0);
  }

  bool CShader::InitializeShader(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename)
  {
    HRESULT result;

    ID3D10Blob* vsBuffer = nullptr, *psBuffer = nullptr;
    CreateVertexShader(device, vsFilename, &vsBuffer);
    CreatePixelShader(device, psFilename, &psBuffer);
    CreateInputLayout(device, vsBuffer);


    if (vsBuffer != nullptr)
    {
      vsBuffer->Release();
      vsBuffer = 0;
    }
    if (psBuffer != nullptr)
    {
      psBuffer->Release();
      psBuffer = 0;
    }

    D3D11_BUFFER_DESC objectMatrixBufferDesc;
		objectMatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		objectMatrixBufferDesc.ByteWidth = sizeof(ObjectMatrixBufferType);
    objectMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		objectMatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		objectMatrixBufferDesc.MiscFlags = 0;
		objectMatrixBufferDesc.StructureByteStride = 0;

    result = device->CreateBuffer(&objectMatrixBufferDesc, NULL, &_objectMatrixBuffer);
    if FAILED(result)
    {
      return false;
    }

    return true;
  }

  bool CShader::CreateVertexShader(ID3D11Device* device, WCHAR* vsFilename, ID3D10Blob** vsBuffer)
  {
    HRESULT result;
    ID3D10Blob* errorMessage = nullptr;

    result = D3DX11CompileFromFile(vsFilename, nullptr, nullptr, "VertexShaderEntry", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr,
      vsBuffer, &errorMessage, nullptr);
    if (FAILED(result))
    {
      // If the shader failed to compile it should have writen something to the error message.
      if (errorMessage)
      {
        OutputShaderErrorMessage(errorMessage, vsFilename);
      }
      // If there was nothing in the error message then it simply could not find the shader file itself.
      else
      {
        MessageBox(nullptr, vsFilename, L"Missing Shader File", MB_OK);
      }

      return false;
    }

    result = device->CreateVertexShader((*vsBuffer)->GetBufferPointer(), (*vsBuffer)->GetBufferSize(), nullptr, &_vertexShader);
    if FAILED(result)
    {
      return false;
    }

    return true;
  }

  bool CShader::CreatePixelShader(ID3D11Device* device, WCHAR* psFilename, ID3D10Blob** psBuffer)
  {
    HRESULT result;
    ID3D10Blob* errorMessage = nullptr;

    result = D3DX11CompileFromFile(psFilename, NULL, NULL, "PixelShaderEntry", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
      psBuffer, &errorMessage, NULL);
    if (FAILED(result))
    {
      // If the shader failed to compile it should have writen something to the error message.
      if (errorMessage)
      {
        OutputShaderErrorMessage(errorMessage, psFilename);
      }
      // If there was  nothing in the error message then it simply could not find the file itself.
      else
      {
        MessageBox(nullptr, psFilename, L"Missing Shader File", MB_OK);
      }

      return false;
    }

    result = device->CreatePixelShader((*psBuffer)->GetBufferPointer(), (*psBuffer)->GetBufferSize(), nullptr, &_pixelShader);
    if FAILED(result)
    {
      return false;
    }

    return true;
  }

  bool CShader::CreateInputLayout(ID3D11Device* device, ID3D10Blob* vsBuffer)
  {
    HRESULT result;

    D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

		//polygonLayout[1].SemanticName = "COLOR";
		//polygonLayout[1].SemanticIndex = 0;
		//polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//polygonLayout[1].InputSlot = 0;
		//polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		//polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		//polygonLayout[1].InstanceDataStepRate = 0;

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

    unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    result = device->CreateInputLayout(polygonLayout, numElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(),
      &_inputLayout);
    if FAILED(result)
    {
      return false;
    }

    return false;
  }

  void CShader::ReleaseShader()
  {
    if (_objectMatrixBuffer != nullptr)
    {
      _objectMatrixBuffer->Release();
      _objectMatrixBuffer = 0;
    }
    if (_inputLayout != nullptr)
    {
      _inputLayout->Release();
      _inputLayout = 0;
    }
    if (_pixelShader != nullptr)
    {
      _pixelShader->Release();
      _pixelShader = 0;
    }
    if (_vertexShader != nullptr)
    {
      _vertexShader->Release();
      _vertexShader = 0;
    }
  }

  void CShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename)
  {
    char* compileErrors;
    SIZE_T bufferSize;
    unsigned long  i;
    std::ofstream fout;


    // Get a pointer to the error message text buffer.
    compileErrors = (char*)(errorMessage->GetBufferPointer());

    // Get the length of the message.
    bufferSize = errorMessage->GetBufferSize();

    // Open a file to write the error message to.
    fout.open("shader-error.txt");

    // Write out the error message.
    for (i = 0; i<bufferSize; i++)
    {
      fout << compileErrors[i];
    }

    // Close the file.
    fout.close();

    // Release the error message.
    errorMessage->Release();
    errorMessage = 0;

    // Pop a message up on the screen to notify the user to check the text file for compile errors.
    MessageBox(nullptr, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
  }
}