#pragma once
#include "../../Engine/ObjectManager/GamaObject.h"
#include "../MyLibrary/MyStruct.h"
#include "../Data.h"

class Player : public GameObject
{
public:
	Player();
	~Player() override;
	void Update() override;
	void Draw() override;

private:
	/// <summary>
	/// 次に落とすフルーツの種類を作成
	/// </summary>
	void CreateFruitType();

	Point fruitPosition_;	// 次に落とす予定の果物の場所
	float coolDownTimer_;	// 次に落とせるようになるまでの時間

	Data::FRUIT_TYPE fruitType_; // 置く予定のフルーツの種類
	Data::FruitData fruitData_; // 置く予定のフルーツのデータ
};