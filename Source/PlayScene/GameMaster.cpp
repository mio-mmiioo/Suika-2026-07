#include "GameMaster.h"
#include "../MyLibrary/ButtonArea.h"
#include "../MyLibrary/Observer.h"
#include "../Data.h"
#include "Fruit.h"
#include "Effect.h"

#include <list>
#include <algorithm>
#include <fstream>

namespace GameMaster
{
	const int MAX_SCORE_DIGITS = 7; // スコアの桁数

	/// <summary>
	/// フルーツリストから指定したfruitを削除する
	/// </summary>
	/// <param name="fruit">Fruitのインスタンス</param>
	void DeleteFruit(Fruit* fruit);

	/// <summary>
	/// ゲームオーバーになっていないか調べる
	/// </summary>
	/// <returns>ゲームオーバーならtrue</returns>
	bool IsCheckGameOver();

	/// <summary>
	/// 指定したフルーツがゲームオーバーに当てはまっていないか調べる
	/// </summary>
	/// <param name="fruit">確認したいフルーツのインスタンス</param>
	/// <returns>ゲームオーバーならtrue</returns>
	bool IsFruitCheckGameOver(Fruit* fruit);

	/// <summary>
	/// 指定したフルーツの進化した次のフルーツを取得する
	/// </summary>
	/// <param name="type">進化前のフルーツの種類</param>
	/// <returns>進化後のフルーツの種類</returns>
	Data::FRUIT_TYPE GetNextType(Data::FRUIT_TYPE type);

	/// <summary>
	/// 現在のゲームを保存する
	/// </summary>
	void SaveData();

	/// <summary>
	/// 前回のゲームの状態を作成する
	/// </summary>
	void CreatePrevGame();

	std::list<Fruit*> allFruitList;		// 箱内にあるすべてのフルーツのリスト
	std::list<Fruit*> deleteFruitList;	// 削除予定のフルーツリスト

	bool isCheckGameOver;	// ゲームオーバーの可能性があるならtrue
	Area box;				// 果物を入れる箱
	Area sinnkanowa;		// フルーツの進化を表す図
	Area score;				// 得点を表示する範囲
	int scoreNumWidth;		// 得点の1つの数字の横幅
	int scoreNumHeight;		// 得点の一つの数字の縦幅

	std::string hBgm; // BGM
}

void GameMaster::Init()
{
	box = Data::areaList["box"];
	sinnkanowa = Data::areaList["sinnkanowa"];
	score = Data::areaList["score"];
	Area number = Data::areaList["number"];
	scoreNumWidth = number.rightDownX - number.leftTopX;
	scoreNumHeight = number.rightDownY - number.leftTopY;
	hBgm = "bgm01";
	Sound::Play(hBgm, true);
	Sound::ChangeVolume(hBgm, 0.75f);

	isCheckGameOver = false;
	if (Observer::GetIsPrevGameCreated() == false)
	{
		CreatePrevGame();
	}
}

int GameMaster::Update()
{
	// 複数回計算をして、位置を整える
	for (int adjustCount = 0; adjustCount < (int)Data::fruitPhysics["maxAdjustCheckPosition"]; adjustCount++)
	{
		FruitCheckPosition();

		if (isCheckGameOver == true)
		{
			if (IsCheckGameOver() == true)
			{
				// ゲームオーバーなら
				Sound::Stop(hBgm);
				return 1;
			}
		}
	}
	return 0;
}

void GameMaster::Draw()
{
	ButtonArea::DrawArea(score);

	// スコアの表示
	{
		int x = score.rightDownX;
		int y = score.leftTopY;
		int score = Observer::GetScore(); // 現在の得点を取得
		int digit[MAX_SCORE_DIGITS];

		// 桁の数字を代入
		for (int count = MAX_SCORE_DIGITS - 1; count >= 0; count--)
		{
			digit[count] = score % 10;
			score = score / 10;
		}
		std::string name;
		// 数字を確認して、画像を表示
		for (int count = MAX_SCORE_DIGITS - 1; count >= 0; count--)
		{
			name = std::to_string(digit[MAX_SCORE_DIGITS - 1 - count]);
			Image::DrawExtendGraph(x - (count + 1) * scoreNumWidth, y, scoreNumWidth, scoreNumHeight, Data::image[name]);
		}
	}

	ButtonArea::DrawArea(box);
	ButtonArea::DrawArea(sinnkanowa);
}

void GameMaster::Release()
{
	Sound::Stop(hBgm);
	SaveData(); // データを保存する

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
	bool isCheck = false; // ゲームオーバーか確認すべきならtrue
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
			isCheck = true;
			IsFruitCheckGameOver(fruitA);
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
						new Effect(p, Data::fruitDataList[type].distanceR);
						Observer::AddScore(Data::fruitDataList[type].score); // 得点の加算
						Sound::Play("createFruit", false); // 生成音を鳴らす

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
					if (fruitA->GetIsSleep() == true || fruitB->GetIsSleep() == true)
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
				if (fruitA->GetIsSleep() == false)
				{
					fruitA->AddVelocity(Point(-n.x * pushPower, 0.0f));
				}
				if (fruitB->GetIsSleep() == false)
				{
					fruitB->AddVelocity(Point(n.x* pushPower, 0.0f));
				}

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

		if (isCheck == true)
		{
			break;
		}
	}

	isCheckGameOver = isCheck;

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
	if (p.y + distanceR >= box.rightDownY)
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

bool GameMaster::IsCheckGameOver()
{
	bool ret = false;
	Fruit* fruit = nullptr;
	//Fruit* overFruit = nullptr;

	// 箱の外に出ていないかを確認する
	for (auto itr = allFruitList.begin(); itr != allFruitList.end(); itr++)
	{
		fruit = *itr;
		// フルーツがゲームオーバーなら
		if (IsFruitCheckGameOver(fruit) == true)
		{
			ret = true;
			Point p = fruit->GetPosition();

			// 箱のふちと確認し、横にずらす
			// これがないと、箱に重なりながら落ちてしまう
			if (p.x <= box.leftTopX)
			{
				p.x = box.leftTopX - fruit->GetFruitData().distanceR; // 左側に落とす
			}
			else
			{
				p.x = box.rightDownX - fruit->GetFruitData().distanceR; // 右側に落とす
			}
		}
		else
		{
			FruitCheckBoxPosition(fruit);
		}
	}

	// ゲームオーバーの場合
	if (ret == true)
	{
		for (auto itr = allFruitList.begin(); itr != allFruitList.end(); itr++)
		{
			// ゲームオーバーの原因になったフルーツ以外はスリープさせる
			if (!(fruit == (*itr)))
			{
				(*itr)->SetSleep();
			}
		}
	}
	return ret;
}

bool GameMaster::IsFruitCheckGameOver(Fruit* fruit)
{
	bool ret = false;
	Point p = fruit->GetPosition();
	// 箱の外に出ているかを確認
	if (p.x < box.leftTopX || p.x > box.rightDownX)
	{
		if (p.y - fruit->GetFruitData().distanceR < box.leftTopY)
		{
			// 箱の外に出ているため、ゲームオーバーの処理
			ret = true;
			fruit->SetIsGameOverFruit();
		}
	}
	return ret;
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

void GameMaster::SaveData()
{
	std::ofstream file("Assets/data/saveData.csv");

	// ゲームオーバーじゃない場合、現在のフルーツの情報を書き込む
	if (isCheckGameOver == false)
	{
		file << Observer::GetScore() << std::endl;
		for (auto& fruit : allFruitList)
		{
			Point pos = fruit->GetPosition();
			Data::FRUIT_TYPE type = fruit->GetFruitData().type;
			
			// 置く前のフルーツなら考慮しない
			if (pos.y <= box.leftTopY)
			{
				continue;
			}
			// データに書き込み
			file << type << ",";
			file << pos.x << "," << pos.y;
			file << std::endl;
		}
	}
	// ゲームオーバーの場合、特に書き込まない
	else
	{
		file << "";
	}
	file.close();

	int score = 0;
	Data::InitSaveFruitData(&score);
}

void GameMaster::CreatePrevGame()
{
	int score = 0;
	Data::InitSaveFruitData(&score); // 前回までのゲームの状況を読み込む
	Data::SaveFruitData current;
	for (int count = 0; count < Data::saveFruitData.size(); count++)
	{
		current = Data::saveFruitData[count];
		new Fruit(current.type, Point(current.x, current.y));
	}
	Observer::SetScore(score); // スコアをセット
	Observer::SetIsPrevGameCreated(true); // 前回のゲームを作成した報告
}
