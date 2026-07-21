#include "GameMaster.h"
#include "../MyLibrary/ButtonArea.h"
#include "../Data.h"
#include "Fruit.h"

#include <list>
#include <algorithm>

namespace GameMaster
{
	/// <summary>
	/// フルーツリストから指定したfruitを削除する
	/// </summary>
	/// <param name="fruit">Fruitのインスタンス</param>
	void DeleteFruit(Fruit* fruit);

	/// <summary>
	/// 指定したフルーツの進化した次のフルーツを取得する
	/// </summary>
	/// <param name="type">進化前のフルーツの種類</param>
	/// <returns>進化後のフルーツの種類</returns>
	Data::FRUIT_TYPE GetNextType(Data::FRUIT_TYPE type);

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
	// 複数回計算をして、位置を整える
	for (int adjustCount = 0; adjustCount < (int)Data::fruitPhysics["maxAdjustCheckPosition"]; adjustCount++)
	{
		FruitCheckPosition();
	}
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

	// y座標をもとに、並び変える
	allFruitList.sort([](const Fruit* a, const Fruit* b)
		{
			return a->GetPosition().y < b->GetPosition().y;
		}
	);
}

void GameMaster::FruitCheckPosition()
{
	Fruit* fruitA = nullptr;
	Fruit* fruitB = nullptr;

	for (auto itr1 = allFruitList.begin(); itr1 != allFruitList.end(); itr1++)
	{
		fruitA = *itr1;
		if (fruitA == nullptr)
		{
			continue;
		}

		// すでに進化に使用され、削除リストに入っている場合、計算をスキップする
		if (std::find(deleteFruitList.begin(), deleteFruitList.end(), fruitA) != deleteFruitList.end())
		{
			continue;
		}

		int supportCount = 0;
		Point pos1 = fruitA->GetPosition();

		// 箱外に出ている可能性がある場合
		if (box.leftTopX >= pos1.x || box.rightDownX <= pos1.x)
		{
			// ゲームオーバーのフラグを立てる
		}

		for (auto itr2 = std::next(itr1); itr2 != allFruitList.end(); itr2++)
		{
			fruitB = *itr2;
			if (fruitB == nullptr)
			{
				continue;
			}
			if (std::find(deleteFruitList.begin(), deleteFruitList.end(), fruitB) != deleteFruitList.end())
			{
				continue;
			}

			float d = fruitB->GetFruitData().distanceR + fruitA->GetFruitData().distanceR;
			Point pos2 = fruitB->GetPosition();
			float x = pos2.x - pos1.x;
			float y = pos2.y - pos1.y;
			float distSq = x * x + y * y;

			// 衝突しているかを確認
			if ((distSq <= d * d) && (distSq > 0.0f))
			{
				// 同じ種類の場合
				if (fruitA->GetFruitData().type == fruitB->GetFruitData().type)
				{
					// 衝突箇所に進化したフルーツを生成する
					Data::FRUIT_TYPE type = GetNextType(fruitA->GetFruitData().type);
					// 進化できるフルーツの場合
					if (!(type == Data::FRUIT_TYPE::MAX_FRUIT_TYPE))
					{
						Point p = { (pos1.x + pos2.x) / 2 + Data::fruitPhysics["positionOffset"], (pos1.y + pos2.y) / 2 };
						new Fruit(type, p);

						// ここで削除するとアクセスエラーなど多々問題があるため、削除リストに追加
						deleteFruitList.push_back(fruitA);
						deleteFruitList.push_back(fruitB);
						break;
					}
				}

				supportCount += 1;

				// スリープ中のフルーツを起こす
				float velocityAY = fruitA->GetVelocity().y;
				float velocityBY = fruitB->GetVelocity().y;
				float wakeUpVelocity = Data::fruitPhysics["wakeUpVelocity"];
				if (velocityAY > wakeUpVelocity || velocityBY > wakeUpVelocity)
				{
					if (fruitA->GetIsSleep() == false || fruitB->GetIsSleep() == false)
					{
						fruitA->WakeUp();
						fruitB->WakeUp();
					}
				}

				// めり込んでいる量
				float distance = sqrtf(distSq);
				float overlap = d - distance;
				Point n = { x / distance, y / distance }; // 法線ベクトル

				// 押し戻す　overlapが小さいなら押し戻さない
				if (overlap >= Data::fruitPhysics["minOverLap"])
				{
					float collisionRaitio = Data::fruitPhysics["collisionRaitio"];
					pos1.x -= n.x * overlap * collisionRaitio;
					pos1.y -= n.y * overlap * collisionRaitio;
					pos2.x += n.x * overlap * collisionRaitio;
					pos2.y += n.y * overlap * collisionRaitio;
				}

				// 横方向に転がす
				float pushPower = overlap * Data::fruitPhysics["fruitRollPower"];
				fruitA->AddVelocity(Point(-n.x * pushPower, 0.0f));
				fruitB->AddVelocity(Point(n.x * pushPower, 0.0f));

				// 位置をセットする
				fruitA->SetPosition(pos1);
				fruitB->SetPosition(pos2);
			}
			else if (distSq == 0.0f)
			{
				// 完全に中心が重なってしまった場合のためのゼロ除算回避
				pos2.x += Data::fruitPhysics["posiitonOffset"]; // 若干ずらす
				fruitB->SetPosition(pos2);
			}
		}
		fruitA->SetSupportCount(supportCount);
		if (supportCount < (int)Data::fruitPhysics["supportCountMin"])
		{
			if (pos1.y < box.rightDownY - fruitA->GetFruitData().distanceR)
			{
				fruitA->WakeUp();
			}
		}
	}

	// 最後にもう一度、箱内に収める
	for (Fruit* fruit : allFruitList)
	{
		FruitCheckBoxPosition(fruit);
	}

	// 進化に使用された、削除予定のフルーツを削除する
	for (auto itr = deleteFruitList.begin(); itr != deleteFruitList.end(); itr++)
	{
		(*itr)->DestroyMe();
		DeleteFruit(*itr);
		*itr = nullptr;
	}
	deleteFruitList.clear();
	allFruitList.remove(nullptr);
}

bool GameMaster::FruitCheckBoxPosition(Fruit* fruit)
{
	Point p = fruit->GetPosition();
	float distanceR = fruit->GetFruitData().distanceR;
	bool ret = false;

	// 上からはみ出ないフルーツは戻す
	if (p.y > box.leftTopY)
	{
		if (p.x - distanceR < box.leftTopX)
		{
			p.x = box.leftTopX + distanceR;
		}
		else if (p.x + distanceR > box.rightDownX)
		{
			p.x = box.rightDownX - distanceR;
		}
	}
	if (p.y + distanceR > box.rightDownY)
	{
		p.y = box.rightDownY - distanceR;
		ret = true;
	}

	fruit->SetPosition(p);
	return ret;
}

void GameMaster::DeleteFruit(Fruit* fruit)
{
	for (auto itr = allFruitList.begin(); itr != allFruitList.end(); itr++)
	{
		if (*itr == fruit)
		{
			*itr = nullptr;
		}
	}
}

Data::FRUIT_TYPE GameMaster::GetNextType(Data::FRUIT_TYPE type)
{
	Data::FRUIT_TYPE ret;

	switch (type)
	{
	case Data::FRUIT_TYPE::SAKURANNBO:
		ret = Data::FRUIT_TYPE::ITIGO;
		break;
	case Data::FRUIT_TYPE::ITIGO:
		ret = Data::FRUIT_TYPE::BUDOU;
		break;
	case Data::FRUIT_TYPE::BUDOU:
		ret = Data::FRUIT_TYPE::ORENNJI;
		break;
	case Data::FRUIT_TYPE::ORENNJI:
		ret = Data::FRUIT_TYPE::MIKANN;
		break;
	case Data::FRUIT_TYPE::MIKANN:
		ret = Data::FRUIT_TYPE::RINNGO;
		break;
	case Data::FRUIT_TYPE::RINNGO:
		ret = Data::FRUIT_TYPE::NASI;
		break;
	case Data::FRUIT_TYPE::NASI:
		ret = Data::FRUIT_TYPE::MOMO;
		break;
	case Data::FRUIT_TYPE::MOMO:
		ret = Data::FRUIT_TYPE::PAINAPPURU;
		break;
	case Data::FRUIT_TYPE::PAINAPPURU:
		ret = Data::FRUIT_TYPE::MERONN;
		break;
	case Data::FRUIT_TYPE::MERONN:
		ret = Data::FRUIT_TYPE::SUIKA;
		break;
	default:
		ret = Data::FRUIT_TYPE::MAX_FRUIT_TYPE;
	}
	return ret;
}
