#pragma once
#include "../../Engine/ObjectManager/GamaObject.h"
#include "../MyLibrary/MyStruct.h"

/// <summary>
/// フルーツが進化した時のエフェクトクラス
/// </summary>
class Effect : public GameObject
{
public:
	/// <summary>
	/// Effectのコンストラクタ
	/// </summary>
	/// <param name="position">位置</param>
	/// <param name="fruitDistanceR">半径</param>
	Effect(Point position, int fruitDistanceR);
	~Effect() override;		// デストラクタ
	void Update() override; // 更新処理
	void Draw() override;	// 描画処理

private:
	Point position_;	// 位置
	float timer_;		// 時間
	int hImage_;		// 画像ハンドル
	int anim_;			// アニメーションの状態
	int size_;			// 表示するサイズ
	int imageWidth_;	// 画像の横幅
	int imageHeight_;	// 画像の縦幅
};

