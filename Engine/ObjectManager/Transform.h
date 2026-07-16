#pragma once
#include <DirectXMath.h>

class Transform
{
public:
	DirectX::XMFLOAT3 position_;	// 位置
	DirectX::XMFLOAT3 rotate_;		// 向き
	DirectX::XMFLOAT3 scale_;		// 拡大率
	Transform* pParent_;			// 親

	Transform();
	~Transform();

	/// <summary>
	/// 各行列の計算
	/// </summary>
	void Calclation();

	/// <summary>
	/// ワールド行列を取得する
	/// </summary>
	/// <returns>ワールド行列</returns>
	DirectX::XMMATRIX GetWorldMatrix();

	/// <summary>
	/// 法線行列を取得する
	/// </summary>
	/// <returns>法線行列</returns>
	DirectX::XMMATRIX GetNormalMatrix();

	/// <summary>
	/// transformに行列を掛ける
	/// </summary>
	/// <param name="matrix">行列</param>
	void MultiplyMatrices(DirectX::XMMATRIX matrix);

	DirectX::XMMATRIX GetMatrixTranslate() const { return matTranslate_; }

	/// <summary>
	/// 回転行列の取得
	/// </summary>
	/// <returns>回転行列</returns>
	DirectX::XMMATRIX GetMatrixRotate() const { return matRotate_; }

	/// <summary>
	/// 拡縮行列の取得
	/// </summary>
	/// <returns>拡縮行列</returns>
	DirectX::XMMATRIX GetMatrixScale() const { return matScale_; }

private:
	DirectX::XMMATRIX matTranslate_;	// 移動行列
	DirectX::XMMATRIX matRotate_;		// 回転行列
	DirectX::XMMATRIX matScale_;		// 拡大行列
};