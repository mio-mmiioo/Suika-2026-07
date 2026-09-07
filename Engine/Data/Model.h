#pragma once

#include <string>
#include "../DirectX/Fbx.h"

namespace Model
{
	struct MODEL_DATA
	{
		Fbx* pFbx; // モデルを管理するのに必要なクラス
		Transform transform; // 位置
		std::string fileName; // ファイルの名前
	};

	/// <summary>
	/// 3Dモデルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>モデルの番号</returns>
	int Load(std::string fileName);

	/// <summary>
	/// モデルの位置をセットする
	/// </summary>
	/// <param name="hModel">セットするモデルの番号</param>
	/// <param name="transform">セットする位置</param>
	void SetTransform(int hModel, Transform transform);

	/// <summary>
	/// モデルを描画する
	/// </summary>
	/// <param name="hModel">描画するモデルの名前</param>
	void Draw(int hModel);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Relase();
}
