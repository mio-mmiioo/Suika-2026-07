#pragma once
#include "DirectX/Direct3D.h"
#include <DirectXMath.h>

namespace Camera
{
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// カメラの位置を指定した値にセットする
	/// </summary>
	/// <param name="pos">位置</param>
	void SetPosition(DirectX::XMVECTOR pos);

	/// <summary>
	/// カメラの焦点を指定した値にセットする
	/// </summary>
	/// <param name="tar">焦点</param>
	void SetTarget(DirectX::XMVECTOR tar);

	/// <summary>
	/// ビュー行列を取得する
	/// </summary>
	/// <returns>ビュー行列</returns>
	DirectX::XMMATRIX GetViewMatrix();

	/// <summary>
	/// プロジェクション行列
	/// </summary>
	/// <returns>プロジェクション行列</returns>
	DirectX::XMMATRIX GetProjectionMatrix();
}
