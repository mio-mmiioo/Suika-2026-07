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
	// フルーツの出現率
	enum APPEARANCE_RATE
	{
		APPEARANCE_RATE_NAME, // フルーツの名前
		APPEARANCE_RATE_RATE, // 出現率
		MAX_FRUIT_APPEARANCE_RATE
	};

	/// <summary>
	/// 次に落とすフルーツの種類を作成
	/// </summary>
	void CreateFruitType();

	MY_POINT fruitPosition_;	// 次に落とす予定の果物の場所
	float coolDownTimer_;	// 次に落とせるようになるまでの時間

	Data::FRUIT_TYPE fruitType_;			// 置く予定のフルーツの種類
	Data::FRUIT_DATA fruitData_;				// 置く予定のフルーツのデータ
	std::vector<int> fruitAppearanceRate_;	// フルーツの出現率
};