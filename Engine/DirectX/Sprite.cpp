#include "Sprite.h"
#include "Texture.h"
#include "../Screen.h"

Sprite::Sprite()
	:pVertexBuffer_(nullptr),
	pIndexBuffer_(nullptr),
	pConstantBuffer_(nullptr),
	pTexture_(nullptr)
{
}

Sprite::~Sprite()
{
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

void Sprite::DrawGraph(int x, int y, RECT rect, int width, int height, float rotation, float alpha)
{
	Direct3D::SetShader(Direct3D::SHADER_2D);

	// コンスタントバッファに渡す情報
	S_CONSTANT_BUFFER constantBuffer;

	// テクスチャの座標変換行列を渡す
	{
		DirectX::XMFLOAT3 size = pTexture_->GetGraphSize();
		DirectX::XMMATRIX matrixTextureTranslate = DirectX::XMMatrixTranslation((float)rect.left / (float)size.x, (float)rect.top / (float)size.y, 0.0f);
		DirectX::XMMATRIX matrixTextureScale = DirectX::XMMatrixScaling((float)(rect.right - rect.left) / (float)size.x, (float)(rect.bottom - rect.top) / (float)size.y, 1.0f);
		DirectX::XMMATRIX matrixTexture = matrixTextureScale * matrixTextureTranslate;
		constantBuffer.uvTranslate = DirectX::XMMatrixTranspose(matrixTexture);
	}

	constantBuffer.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha); // 透明度を反映
	constantBuffer.position = DirectX::XMFLOAT2((float)x, (float)y);
	constantBuffer.size = DirectX::XMFLOAT2((float)width, (float)height);
	constantBuffer.screenSize = DirectX::XMFLOAT2((float)Screen::WINDOW_WIDTH, (float)Screen::WINDOW_HEIGHT);
	constantBuffer.imageSize = DirectX::XMFLOAT2(pTexture_->GetGraphSize().x, pTexture_->GetGraphSize().y);
	constantBuffer.rotation = rotation;

	D3D11_MAPPED_SUBRESOURCE pData;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData); // データを書き込むために、GPUからのデータアクセスを止める
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&constantBuffer), sizeof(constantBuffer)); // データを送る

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext->Unmap(pConstantBuffer_, 0); // 再開

	// 頂点バッファ
	UINT stride = sizeof(S_VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	// コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_); // 頂点シェーダー用
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_); // ピクセルシェーダー用

	Direct3D::pContext->DrawIndexed(6, 0, 0);
}

void Sprite::Release()
{
	SAFE_RELEASE(pTexture_);
	SAFE_DELETE(pTexture_);

	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}


HRESULT Sprite::LoadTexture(const char* fileName)
{
	HRESULT hr;
	// 頂点情報
	S_VERTEX vertices[] =
	{
		{ DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) },	// 左上
		{ DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) },	// 右上
		{ DirectX::XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f), DirectX::XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f) },	// 右下
		{ DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) }	// 左下
	};
	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bufferDescData;
	bufferDescData.ByteWidth = sizeof(vertices);
	bufferDescData.Usage = D3D11_USAGE_DEFAULT;
	bufferDescData.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescData.CPUAccessFlags = 0;
	bufferDescData.MiscFlags = 0;
	bufferDescData.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA dataVertices;
	dataVertices.pSysMem = vertices;
	hr = Direct3D::pDevice->CreateBuffer(&bufferDescData, &dataVertices, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Sprite LoadTexture()：頂点データ用バッファの設定に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	// インデックス情報
	int index[] = { 0, 2, 3,  0, 1, 2 };

	// インデックスバッファを作成
	D3D11_BUFFER_DESC indexBuffer;
	indexBuffer.ByteWidth = sizeof(index);
	indexBuffer.Usage = D3D11_USAGE_DEFAULT;
	indexBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffer.CPUAccessFlags = 0;
	indexBuffer.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = index;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;
	hr = Direct3D::pDevice->CreateBuffer(&indexBuffer, &initData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Sprite LoadTexture()：インデックスバッファの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	// コンスタントバッファを作成
	D3D11_BUFFER_DESC constantBuffer;
	constantBuffer.ByteWidth = sizeof(S_CONSTANT_BUFFER);
	constantBuffer.Usage = D3D11_USAGE_DYNAMIC;
	constantBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBuffer.MiscFlags = 0;
	constantBuffer.StructureByteStride = 0;
	hr = Direct3D::pDevice->CreateBuffer(&constantBuffer, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Sprite LoadTexture()：コンスタントバッファの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	std::string folder("Assets/image/");
	pTexture_ = new Texture();
	hr = pTexture_->Load(folder + fileName);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK; // 問題なく終了
}

DirectX::XMFLOAT2 Sprite::GetGraphSize()
{
	DirectX::XMFLOAT2 ret;
	ret.x = pTexture_->GetGraphSize().x;
	ret.y = pTexture_->GetGraphSize().y;
	return ret;
}
