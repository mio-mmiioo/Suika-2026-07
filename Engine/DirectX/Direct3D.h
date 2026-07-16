#pragma once
#include <d3d11.h>

// リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_DELETE(p) if (p != nullptr) {delete p; p = nullptr;}
#define SAFE_RELEASE(p) if (p != nullptr) { p->Release(); p = nullptr;}

namespace Direct3D
{
	extern ID3D11Device* pDevice;			// デバイス
	extern ID3D11DeviceContext* pContext;	// デバイスコンテキスト

	// シェーダーの種類
	enum SHADER_TYPE
	{
		SHADER_3D,		// 3Dで使用するシェーダー
		SHADER_2D,		// 2Dで使用するシェーダー
		MAX_SHADER_TYPE // シェーダーの最大数
	};

	
	/// <summary>
	/// Direct3Dの初期化
	/// </summary>
	/// <param name="windowWidth">ウィンドウの横幅</param>
	/// <param name="windowHeight">ウィンドウの縦幅</param>
	/// <param name="hWnd">ウィンドウ</param>
	/// <returns>成功したか失敗したか( 失敗した場合、理由が返ってくる )</returns>
	HRESULT Initialize(int windowWidth, int windowHeight, HWND hWnd);

	/// <summary>
	/// 全てのシェーダーの初期化をまとめて行う
	/// </summary>
	/// <returns>成功したか失敗したか</returns>
	HRESULT InitShader();

	/// <summary>
	/// 3D版シェーダーの初期化
	/// </summary>
	/// <returns>成功したか失敗したか( 失敗した場合、理由が返ってくる )</returns>
	HRESULT InitShader3D();

	/// <summary>
	/// 2D版シェーダーの初期化
	/// </summary>
	/// <returns>成功したか失敗したか( 失敗した場合、理由が返ってくる )</returns>
	HRESULT InitShader2D();

	/// <summary>
	/// 使用するシェーダーをセットする
	/// </summary>
	/// <param name="type">使用するシェーダーの種類</param>
	void SetShader(SHADER_TYPE type);

	/// <summary>
	/// 描画処理の前に呼び出す
	/// </summary>
	void BeginDraw();

	/// <summary>
	/// 描画処理の後に呼び出す
	/// </summary>
	void EndDraw();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();
}
