#include "TextureShaderClass.h"

#include <cstdlib>
#include <cwchar>
#include<cstring>

TextureShaderClass::TextureShaderClass()
{
	m_VertexShader = 0;
	m_PixelShader = 0;
	m_Layout = 0;
	m_MatrixBuffer = 0;
	m_SampleState = 0;
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass&)
{
}

TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Initialise(ID3D11Device* Device, HWND hwnd)
{
	bool Result;
	wchar_t vsFilename[128];
	wchar_t psFilename[128];
	int Error;

	Error = wcscpy_s(vsFilename, 128, L"Shaders/Texture.vs");
	if (Error != 0)
	{
		return false;
	}

	Error = wcscpy_s(psFilename, 128, L"Shaders/Texture.ps");
	if (Error != 0)
	{
		return false;
	}

	Result = InitialiseShader(Device, hwnd, vsFilename, psFilename);
	if (!Result)
	{
		return false;
	}

	return true;
}

void TextureShaderClass::Shutdown()
{
	ShutdownShader();
}

bool TextureShaderClass::Render(ID3D11DeviceContext* DeviceContext, int IndexCount, DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix, ID3D11ShaderResourceView* Texture)
{
	bool Result = SetShaderParameters(DeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, Texture);
	if (!Result)
	{
		return false;
	}

	RenderShader(DeviceContext, IndexCount);

	return true;
}

bool TextureShaderClass::InitialiseShader(ID3D11Device* Device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT Result;
	ID3D10Blob* ErrorMessage = 0;
	ID3D10Blob* VertexShaderBuffer = 0;
	ID3D10Blob* PixelShaderBuffer = 0;
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[2];
	unsigned int NumElements;
	D3D11_BUFFER_DESC MatrixBufferDesc = {};
	D3D11_SAMPLER_DESC SamplerDesc = {};

	Result = D3DCompileFromFile(vsFilename, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &VertexShaderBuffer, &ErrorMessage);
	if (FAILED(Result))
	{
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing shader file!", MB_OK);
		}

		return false;
	}

	Result = D3DCompileFromFile(psFilename, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &PixelShaderBuffer, &ErrorMessage);
	if (FAILED(Result))
	{
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, psFilename, L"Missing shader file!", MB_OK);
		}

		return false;
	}

	Result = Device->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader);
	if (FAILED(Result))
	{
		return false;
	}

	Result = Device->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader);
	if (FAILED(Result))
	{
		return false;
	}

	PolygonLayout[0].SemanticName = "POSITION";
	PolygonLayout[0].SemanticIndex = 0;
	PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[0].InputSlot = 0;
	PolygonLayout[0].AlignedByteOffset = 0;
	PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[0].InstanceDataStepRate = 0;

	PolygonLayout[1].SemanticName = "TEXCOORD";
	PolygonLayout[1].SemanticIndex = 0;
	PolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	PolygonLayout[1].InputSlot = 0;
	PolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[1].InstanceDataStepRate = 0;

	NumElements = sizeof(PolygonLayout) / sizeof(PolygonLayout[0]);

	Result = Device->CreateInputLayout(PolygonLayout, NumElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &m_Layout);
	if (FAILED(Result))
	{
		return false;
	}

	VertexShaderBuffer->Release();
	VertexShaderBuffer = 0;
	PixelShaderBuffer->Release();
	PixelShaderBuffer = 0;

	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;

	Result = Device->CreateBuffer(&MatrixBufferDesc, NULL, &m_MatrixBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	Result = Device->CreateSamplerState(&SamplerDesc, &m_SampleState);
	if (FAILED(Result))
	{
		return false;
	}

	return true;
}

void TextureShaderClass::ShutdownShader()
{
	if (m_SampleState)
	{
		m_SampleState->Release();
		m_SampleState = 0;
	}
	
	if (m_MatrixBuffer)
	{
		m_MatrixBuffer->Release();
		m_MatrixBuffer = 0;
	}

	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = 0;
	}

	if (m_PixelShader)
	{
		m_PixelShader->Release();
		m_PixelShader = 0;
	}

	if (m_VertexShader)
	{
		m_VertexShader->Release();
		m_VertexShader = 0;
	}
}

void TextureShaderClass::OutputShaderErrorMessage(ID3D10Blob* ErrorMessage, HWND hwnd, WCHAR* ShaderFilename)
{
	char* CompileErrors;
	unsigned long long BufferSize, i;
	std::ofstream fout;

	CompileErrors = (char*)(ErrorMessage->GetBufferPointer());
	BufferSize = ErrorMessage->GetBufferSize();
	fout.open("shader-error.txt");

	const char* str1 = "Error compiling shader!\n\n";

	size_t len1 = std::strlen(str1) + 1;
	size_t len2 = std::strlen(CompileErrors) + 1;

	wchar_t* wstr1 = new wchar_t[len1];
	wchar_t* wstr2 = new wchar_t[len2];

	size_t Converted1, Converted2;
	mbstowcs_s(&Converted1, wstr1, len1, str1, len1 - 1);
	mbstowcs_s(&Converted2, wstr2, len2, CompileErrors, len2 - 1);

	size_t CombinedLen = len1 + len2 - 1;
	wchar_t* CombinedWstr = new wchar_t[CombinedLen];

	wcscpy_s(CombinedWstr, CombinedLen, wstr1);
	wcscat_s(CombinedWstr, CombinedLen, wstr2);

	for (i = 0; i < BufferSize; i++)
	{
		fout << CompileErrors[i];
	}

	fout.close();

	ErrorMessage->Release();
	ErrorMessage = 0;

	MessageBox(hwnd, CombinedWstr, ShaderFilename, MB_OK);

	delete[] wstr1;
	delete[] wstr2;
	delete[] CombinedWstr;
}

bool TextureShaderClass::SetShaderParameters(ID3D11DeviceContext* DeviceContext, DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix, ID3D11ShaderResourceView* Texture)
{
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE MappedSubresouce;
	MatrixBufferType* DataPtr;
	unsigned int BufferNumber;

	// remember to transpose from row major before sending to shaders
	WorldMatrix = DirectX::XMMatrixTranspose(WorldMatrix);
	ViewMatrix = DirectX::XMMatrixTranspose(ViewMatrix);
	ProjectionMatrix = DirectX::XMMatrixTranspose(ProjectionMatrix);

	Result = DeviceContext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubresouce);
	if (FAILED(Result))
	{
		return false;
	}

	DataPtr = (MatrixBufferType*)MappedSubresouce.pData;

	DataPtr->World = WorldMatrix;
	DataPtr->View = ViewMatrix;
	DataPtr->Projection = ProjectionMatrix;

	DeviceContext->Unmap(m_MatrixBuffer, 0u);

	BufferNumber = 0;

	DeviceContext->VSSetConstantBuffers(BufferNumber, 1u, &m_MatrixBuffer);

	DeviceContext->PSSetShaderResources(0u, 1u, &Texture);

	return true;
}

void TextureShaderClass::RenderShader(ID3D11DeviceContext* DeviceContext, int IndexCount)
{
	DeviceContext->IASetInputLayout(m_Layout);

	DeviceContext->VSSetShader(m_VertexShader, NULL, 0u);
	DeviceContext->PSSetShader(m_PixelShader, NULL, 0u);
	DeviceContext->PSSetSamplers(0u, 1u, &m_SampleState);

	DeviceContext->DrawIndexed(IndexCount, 0u, 0);
}
