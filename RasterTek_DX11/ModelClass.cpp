#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_VertexBuffer = 0;
	m_IndexBuffer = 0;
}

ModelClass::ModelClass(const ModelClass&)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialise(ID3D11Device* Device)
{
	bool Result = InitialiseBuffers(Device);
	if (!Result)
	{
		return false;
	}
	
	return true;
}

void ModelClass::Shutdown()
{
	ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext* DeviceContext)
{
	RenderBuffers(DeviceContext);
}

bool ModelClass::InitialiseBuffers(ID3D11Device* Device)
{
	VertexType* Vertices;
	unsigned long* Indices;
	D3D11_BUFFER_DESC VertexBufferDesc = {};
	D3D11_BUFFER_DESC IndexBufferDesc = {};
	D3D11_SUBRESOURCE_DATA VertexData = {};
	D3D11_SUBRESOURCE_DATA IndexData = {};
	HRESULT Result;

	m_VertexCount = 3;
	m_IndexCount = 3;

	Vertices = new VertexType[m_VertexCount];
	if (!Vertices)
	{
		return false;
	}

	Indices = new unsigned long[m_IndexCount];
	if (!Indices)
	{
		return false;
	}

	Vertices[0].Position = DirectX::XMFLOAT3(-1.f, -1.f, 0.f);
	Vertices[0].Color = DirectX::XMFLOAT4(0.f, 0.f, 1.f, 1.f);
	Vertices[1].Position = DirectX::XMFLOAT3( 0.f,  1.f, 0.f);
	Vertices[1].Color = DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);
	Vertices[2].Position = DirectX::XMFLOAT3( 1.f, -1.f, 0.f);
	Vertices[2].Color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);

	Indices[0] = 0u;
	Indices[1] = 1u;
	Indices[2] = 2u;

	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	VertexData.pSysMem = Vertices;

	Result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &m_VertexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	IndexData.pSysMem = Indices;

	Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &m_IndexBuffer);
	if (FAILED(Result))
	{
		return false;
	}
	
	delete[] Vertices;
	Vertices = 0;

	delete[] Indices;
	Indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	if (m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = 0;
	}

	if (m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = 0;
	}
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* DeviceContext)
{
	unsigned int Stride, Offset;

	Stride = sizeof(VertexType);
	Offset = 0;

	DeviceContext->IASetVertexBuffers(0u, 1u, &m_VertexBuffer, &Stride, &Offset);
	DeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0u);
	DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
