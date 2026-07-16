#pragma once
#include <dInput.h>
#include <string>
#include <DirectXMath.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")

namespace Input
{
	/// <summary>
	/// 初期化 アクションマップもここで初期化している
	/// </summary>
	/// <param name="hWnd">ウィンドウ</param>
	void Initialize(HWND hWnd);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	
	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// 指定した動作のボタンが、押された瞬間か確認する
	/// </summary>
	/// <param name="action">動作</param>
	/// <returns>押された瞬間ならtrue</returns>
	bool IsButtonDown(const std::string& action);
	
	/// <summary>
	/// 指定した動作のボタンが、押され続けているか確認する
	/// </summary>
	/// <param name="action">動作</param>
	/// <returns>押され続けているならtrue</returns>
	bool IsButtonKeepDown(const std::string& action);
	
	/// <summary>
	/// 指定した動作のボタンが、離された瞬間か確認する
	/// </summary>
	/// <param name="action">動作</param>
	/// <returns>離された瞬間ならtrue</returns>
	bool IsButtonUp(const std::string& action);

	/// <summary>
	/// マウスカーソルの位置を取得
	/// </summary>
	/// <returns>マウスカーソルの位置</returns>
	DirectX::XMFLOAT3 GetMousePosition();

	/// <summary>
	/// マウスカーソルの位置を取得
	/// </summary>
	/// <returns>マウスカーソルの位置</returns>
	DirectX::XMFLOAT3 GetPrevMousePosition();

	/// <summary>
	/// マウスカーソルの位置をセット
	/// </summary>
	/// <param name="x">x座標</param>
	/// <param name="y">y座標</param>
	void SetMousePosition(int x, int y);

	/// <summary>
	/// マウスの移動量を取得
	/// </summary>
	/// <returns>x,y：マウスの移動量　z：ホイールの回転量</returns>
	DirectX::XMFLOAT3 GetMouseMove();

	/// <summary>
	/// 左スティックの傾きを取得
	/// </summary>
	/// <returns>傾き</returns>
	DirectX::XMFLOAT3 GetPadStickLeft();

	/// <summary>
	/// 右スティックの傾きを取得
	/// </summary>
	/// <returns>傾き</returns>
	DirectX::XMFLOAT3 GetPadStickRight();

	/// <summary>
	/// 左トリガーの押し込み具合を取得
	/// </summary>
	/// <returns>押し込んでいる量</returns>
	float GetPadTrrigerLeft();

	/// <summary>
	/// 右トリガーの押し込み具合を取得
	/// </summary>
	/// <returns>押し込んでいる量</returns>
	float GetPadTrrigerRight();

	/// <summary>
	/// コントローラーを振動させる
	/// </summary>
	/// <param name="left">左モーターの強さ</param>
	/// <param name="right">右モーターの強さ</param>
	void SetPadVibration(int left, int right);
}
