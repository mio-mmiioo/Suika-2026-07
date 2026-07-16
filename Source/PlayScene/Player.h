#pragma once
#include "../../Engine/ObjectManager/GamaObject.h"
#include "../MyLibrary/MyStruct.h"

class Player : public GameObject
{
public:
	Player();
	~Player() override;
	void Update() override;
	void Draw() override;

private:
	Point fruitPosition_;	// 次に落とす予定の果物の場所
	float coolDownTimer_;	// 次に落とせるようになるまでの時間
	int hFruitImage_;		// 落とす予定の果物の画像
};