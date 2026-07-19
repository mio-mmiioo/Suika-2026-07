#pragma once
#include "../../Engine/ObjectManager/GamaObject.h"
#include "../MyLibrary/MyStruct.h"
#include "../Data.h"

class Fruit : public GameObject
{
public:
	/// <summary>
	/// 1つのフルーツ
	/// フルーツはGameMasterで管理される
	/// </summary>
	/// <param name="fruitType">フルーツの種類</param>
	/// <param name="position">フルーツの位置</param>
	Fruit(Data::FRUIT_TYPE fruitType, Point position);
	~Fruit() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// 位置を取得する
	/// </summary>
	/// <returns>座標</returns>
	Point GetPosition() const { return position_; }
	
	/// <summary>
	/// 位置をセットする
	/// </summary>
	/// <param name="p">座標</param>
	void SetPosition(Point p) { position_ = p; }

private:
	Data::FruitData fruitData_;
	Point position_;

	float angle_;
};
