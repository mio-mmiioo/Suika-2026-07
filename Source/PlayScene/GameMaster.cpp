#include "GameMaster.h"
#include "../MyLibrary/ButtonArea.h"
#include "../Data.h"
#include "Fruit.h"

#include <list>

namespace GameMaster
{
	/// <summary>
	/// フルーツリストから指定したfruitを削除する
	/// </summary>
	/// <param name="fruit">Fruitのインスタンス</param>
	void DeleteFruit(Fruit* fruit);

	std::list<Fruit*> allFruitList; // 箱内にあるすべてのフルーツのリスト
	std::list<Fruit*> deleteFruitList; // 削除予定のフルーツリスト
	Area box; // 果物を入れる箱
}

void GameMaster::Init()
{
	box = Data::areaList["box"];
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
	for (auto itr = allFruitList.begin(); itr != allFruitList.end(); itr++)
	{
		(*itr)->DestroyMe();
		*itr = nullptr;
	}
	allFruitList.clear();
}

void GameMaster::AddFruit(Fruit* fruit)
{
	if (fruit != nullptr)
	{
		allFruitList.push_back(fruit);
	}
}

void GameMaster::FruitCheckPosition()
{
}

bool GameMaster::FruitCheckBoxPosition(Fruit* fruit)
{
	return false;
}

void GameMaster::DeleteFruit(Fruit* fruit)
{
	for (auto itr = allFruitList.begin(); itr != allFruitList.end(); itr++)
	{
		if (*itr == fruit)
		{
			*itr = nullptr;
		}
		allFruitList.remove(nullptr);
	}
}
