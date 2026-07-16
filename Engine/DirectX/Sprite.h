#pragma once
#include <DirectXMath.h>
#include <string>
#include "Direct3D.h"

// コンスタントバッファ
struct S_CONSTANT_BUFFER
{
	DirectX::XMMATRIX uvTranslate;	// テクスチャの座標変換行列
	DirectX::XMFLOAT4 color;		// テクスチャとの合成色

	DirectX::XMFLOAT2 position;		// スクリーンの座標
	DirectX::XMFLOAT2 size;			// スクリーンに描画する幅と高さ
	DirectX::XMFLOAT2 imageSize;	// 画像の幅と高さ
	DirectX::XMFLOAT2 screenSize;	// 画面の幅と高さ
	FLOAT rotation;					// スクリーンに描画するときの回転量
};

// 頂点情報
struct S_VERTEX
{
	DirectX::XMVECTOR position; // 位置
	DirectX::XMVECTOR uv;		// UV座標
};

class Texture;
class Transform;

// 2D画像を表示するクラス
class Sprite
{
public:
	Sprite();
	~Sprite();

	/// <summary>
	/// 2D画像を描画
	/// </summary>
	/// <param name="x">スクリーン上のx座標</param>
	/// <param name="y">スクリーン上のy座標</param>
	/// <param name="rect">画像を切り抜く左上の座標と右下の座標</param>
	/// <param name="width">描画する際の横幅</param>
	/// <param name="height">描画する際の縦幅</param>
	/// <param name="rotation">回転量</param>
	/// <param name="alpha">透明度</param>
	void DrawGraph(int x, int y, RECT rect, int width, int height, float rotation, float alpha); // 座標を指定して描画

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// 画像の読み込み
	/// </summary>
	/// <param name="fileName">ファイルのパス</param>
	/// <returns>成功したか失敗したか( 失敗した場合、その原因が返ってくる )</returns>
	HRESULT LoadTexture(const char* fileName);

	/// <summary>
	/// 画像の幅と高さの取得
	/// </summary>
	/// <returns>画像の幅と高さ</returns>
	DirectX::XMFLOAT2 GetGraphSize();

private:
	ID3D11Buffer* pVertexBuffer_;	// 頂点バッファ
	ID3D11Buffer* pIndexBuffer_;	// インデックスバッファ
	ID3D11Buffer* pConstantBuffer_; // コンスタントバッファ

	Texture* pTexture_; // 画像
};
