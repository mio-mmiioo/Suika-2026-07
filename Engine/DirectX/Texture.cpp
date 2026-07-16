#include "Texture.h"
#include <wincodec.h>
#include "Direct3D.h"

#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "windowscodecs.lib")

Texture::Texture()
	:pSampler_(nullptr),
	pSRV_(nullptr),
	size_(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f))
{
}

Texture::~Texture()
{
}

HRESULT Texture::Load(std::string fileName)
{
	std::wstring wfileName(fileName.begin(), fileName.end());
	HRESULT hr;
	
	hr = CoInitialize(NULL);
	IWICImagingFactory* pFactory = NULL;
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pFrame = NULL;
	IWICFormatConverter* pFormatConverter = NULL;
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&pFactory));
	hr = pFactory->CreateDecoderFromFilename(wfileName.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture Load()：ファイルの読み込みに失敗しました", L"エラー", MB_OK);
		return hr;
	}
	pDecoder->GetFrame(0, &pFrame);
	pFactory->CreateFormatConverter(&pFormatConverter);
	pFormatConverter->Initialize(pFrame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, NULL, 1.0f, WICBitmapPaletteTypeMedianCut);
	UINT imageWidth;
	UINT imageHeight;
	pFormatConverter->GetSize(&imageWidth, &imageHeight);
	size_ = DirectX::XMFLOAT3((float)imageWidth, (float)imageHeight, 0);

	// テクスチャの設定
	ID3D11Texture2D* pTexture;
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = imageWidth;
	textureDesc.Height = imageHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;
	hr = Direct3D::pDevice->CreateTexture2D(&textureDesc, NULL, &pTexture);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture Load()：テクスチャの設定に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	// テクスチャを送信
	D3D11_MAPPED_SUBRESOURCE hMappedres;
	Direct3D::pContext->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedres);
	pFormatConverter->CopyPixels(NULL, hMappedres.RowPitch, hMappedres.RowPitch * imageHeight, (BYTE*)hMappedres.pData);
	Direct3D::pContext->Unmap(pTexture, 0);

	// サンプラー作成
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	Direct3D::pDevice->CreateSamplerState(&samplerDesc, &pSampler_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture Load()：サンプラーの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	// シェーダーリソースビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	hr = Direct3D::pDevice->CreateShaderResourceView(pTexture, &srv, &pSRV_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture Load()：シェーダーリソースビューの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	SAFE_RELEASE(pFormatConverter);
	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pFrame);
	SAFE_RELEASE(pFactory);

	return S_OK;
}

void Texture::Release()
{
	SAFE_RELEASE(pSRV_);
	SAFE_RELEASE(pSampler_);
}
