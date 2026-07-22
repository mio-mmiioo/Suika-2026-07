#pragma once
#include "../../Engine/ObjectManager/GamaObject.h"
#include "../MyLibrary/MyStruct.h"

/// <summary>
/// フルーツが進化した時のエフェクトクラス
/// </summary>
class Effect : public GameObject
{
public:
	Effect(Point position, int fruitDistanceR);
	~Effect() override;
	void Update() override;
	void Draw() override;

private:
	Point position_;	// 位置
	float timer_;		// 時間
	int hImage_;		// 画像ハンドル
	int anim_;			// アニメーションの状態
	int size_;			// 表示するサイズ
	int imageWidth_;	// 画像の横幅
	int imageHeight_;	// 画像の縦幅
};

