#include "GameMaster.h"
#include "../MyLibrary/ButtonArea.h"
#include "../Data.h"
#include "Fruit.h"

namespace
{
	Area box; // 果物を入れる箱
	Fruit* fruit;
}

void GameMaster::Init()
{
	box = Data::areaList["box"];
	Point p = { 100.0f, 100.0f };
	fruit = new Fruit(Data::FRUIT_TYPE::ITIGO, p);
}

void GameMaster::Update()
{
	// ここで果物を管理する計算を行う
}

void GameMaster::Draw()
{
	ButtonArea::DrawArea(box);
}

void GameMaster::Release()
{
	delete fruit;
}
