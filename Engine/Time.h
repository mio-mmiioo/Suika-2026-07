#pragma once
#include <windows.h>

namespace Time
{
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(HWND hWnd);
	
	/// <summary>
	/// 更新処理( 毎フレーム呼ぶ )
	/// </summary>
	void Update();

	/// <summary>
	/// 前のフレームから今回のフレームの経過時間を取得
	/// </summary>
	/// <returns>経過時間</returns>
	float GetDeltaTime();
}
