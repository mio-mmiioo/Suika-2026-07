#pragma once
#include <d3d11.h>
#include <string>
#include <DirectXTex.h>

class Texture
{
public:
	Texture();
	~Texture();

	/// <summary>
	/// 画像の読み込み
	/// </summary>
	/// <param name="fileName">ファイルのパス</param>
	/// <returns>成功したかどうか( 失敗した場合、原因が返ってくる )</returns>
	HRESULT Load(std::string fileName);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// サンプラーの取得
	/// </summary>
	/// <returns>サンプラー</returns>
	ID3D11SamplerState* GetSampler() { return pSampler_; }
	
	/// <summary>
	/// シェーダーリソースビューの取得
	/// </summary>
	/// <returns>シェーダーリソースビュー</returns>
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }

	/// <summary>
	/// 画像サイズの取得
	/// </summary>
	/// <returns>画像のサイズ</returns>
	DirectX::XMFLOAT3 GetGraphSize() const { return size_; }

private:
	ID3D11SamplerState* pSampler_;		// サンプラー
	ID3D11ShaderResourceView* pSRV_;	// シェーダーリソースビュー
	DirectX::XMFLOAT3 size_;			// 画像のサイズ
};
