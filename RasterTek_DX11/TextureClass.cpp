#include "TextureClass.h"
#include "MyMacros.h"

TextureClass::TextureClass()
{
	m_TargaData = 0;
	m_Texture = 0;
	m_TextureView = 0;
}

TextureClass::TextureClass(const TextureClass&)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialise(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, char* Filename)
{
	bool Result;
	D3D11_TEXTURE2D_DESC TextureDesc = {};
	HRESULT hResult;
	unsigned int RowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	FALSE_IF_FAILED(LoadTarga32Bit(Filename));

	TextureDesc.Height = m_Height;
	TextureDesc.Width = m_Width;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	HFALSE_IF_FAILED(Device->CreateTexture2D(&TextureDesc, NULL, &m_Texture));

	RowPitch = (m_Width * 4) * sizeof(unsigned char);
	DeviceContext->UpdateSubresource(m_Texture, 0u, NULL, m_TargaData, RowPitch, 0u);

	srvDesc.Format = TextureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = -1;

	HFALSE_IF_FAILED(Device->CreateShaderResourceView(m_Texture, &srvDesc, &m_TextureView));

	DeviceContext->GenerateMips(m_TextureView);

	delete[] m_TargaData;
	m_TargaData = 0;

	return true;
}

void TextureClass::Shutdown()
{
	if (m_TextureView)
	{
		m_TextureView->Release();
		m_TextureView = 0;
	}

	if (m_Texture)
	{
		m_Texture->Release();
		m_Texture = 0;
	}
	
	if (m_TargaData)
	{
		delete[] m_TargaData;
		m_TargaData = 0;
	}
}

bool TextureClass::LoadTarga32Bit(char* Filename)
{
	int Error, bpp, ImageSize, Index, i, j, k;
	FILE* FilePtr;
	unsigned int Count;
	TargaHeader TargaFileHeader;
	unsigned char* TargaImage;

	Error = fopen_s(&FilePtr, Filename, "rb");
	if (Error != 0)
	{
		return false;
	}

	Count = (unsigned int)fread(&TargaFileHeader, sizeof(TargaHeader), 1, FilePtr);
	if (Count != 1)
	{
		return false;
	}

	m_Height = (int)TargaFileHeader.Height;
	m_Width = (int)TargaFileHeader.Width;
	bpp = (int)TargaFileHeader.bpp;

	if (bpp != 32)
	{
		return false;
	}

	ImageSize = m_Width * m_Height * 4;
	TargaImage = new unsigned char[ImageSize];

	Count = (unsigned int)fread(TargaImage, 1, ImageSize, FilePtr);
	if (Count != ImageSize)
	{
		return false;
	}

	Error = fclose(FilePtr);
	if (Error != 0)
	{
		return false;
	}

	// since the image is loaded upside down and NOT in RGBA order, we loop through and fix these
	m_TargaData = new unsigned char[ImageSize];
	Index = 0;
	k = (m_Width * m_Height * 4) - (m_Width * 4);
	for (j = 0; j < m_Height; j++)
	{
		for (i = 0; i < m_Width; i++)
		{
			m_TargaData[Index + 0] = TargaImage[k + 2]; // red
			m_TargaData[Index + 1] = TargaImage[k + 1]; // green
			m_TargaData[Index + 2] = TargaImage[k + 0]; // blue
			m_TargaData[Index + 3] = TargaImage[k + 3]; // alpha

			k += 4;
			Index += 4;
		}

		k -= (m_Width * 8);
	}

	delete[] TargaImage;
	TargaImage = 0;

	return true;
}
