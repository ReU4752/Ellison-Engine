#pragma once

namespace Ellison_Engine
{
  class CShader
  {
  protected:
    struct ObjectMatrixBufferType
    {
			DirectX::XMMATRIX world;
    };

  public:
    CShader();
    CShader(const CShader& other);
    virtual ~CShader();

		template <typename T>
    static T* CreateShader(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename);

    virtual bool Initialize(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename);
		virtual void Release();

    virtual void Render(ID3D11DeviceContext* deviceContext, UINT indexCount, DirectX::XMMATRIX worldMatrix, ID3D11Buffer* cameraMatrixBuffer);

  protected:
    bool InitializeShader(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename);
    bool CreateVertexShader(ID3D11Device* device, WCHAR* vsFilename, ID3D10Blob** vsBuffer);
    bool CreatePixelShader(ID3D11Device* device, WCHAR* psFilename, ID3D10Blob** psBuffer);
    virtual bool CreateInputLayout(ID3D11Device* device, ID3D10Blob* vsBuffer);
    void ReleaseShader();
    void OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename);

		const DirectX::XMVECTOR GammaToLinear(const DirectX::XMFLOAT3& color)
		{
			DirectX::XMVECTOR v;
			v = DirectX::XMVectorSet(powf(color.x, 1 / 2.2f), powf(color.y, 1 / 2.2f), powf(color.z, 1 / 2.2f), 1);
			return v;
		}

  protected:
    ID3D11VertexShader* _vertexShader;
    ID3D11PixelShader* _pixelShader;
    ID3D11InputLayout* _inputLayout;
    ID3D11Buffer* _objectMatrixBuffer;
  };

	template<typename T>
	T* CShader::CreateShader(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename)
	{
		T* temp = new T;
		if (!temp)
		{
			return nullptr;
		}

		if (!temp->Initialize(device, vsFilename, psFilename))
		{
			delete temp;
			temp = nullptr;

			return nullptr;
		}

		return temp;
	}
}