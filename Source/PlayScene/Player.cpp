#include "Player.h"
#include "../../Engine/Data/Image.h"
#include "../../Engine/Input.h"
#include "../../Engine/Time.h"
#include "Fruit.h"

namespace PLAYER
{
	const Point INIT_FRUIT_POSITION = { 512.0f, 50.0f }; // 次に置くフルーツの初期位置
	const float COOL_DOWN_TIME = 0.5f; // 連打を防ぐ時間
	const float MOVE_SPEED = 100.0f;
}

Player::Player()
{
	fruitPosition_ = PLAYER::INIT_FRUIT_POSITION;
	coolDownTimer_ = 0.0f;
	fruitType_ = Data::FRUIT_TYPE::MAX_FRUIT_TYPE;
	CreateFruitType(); // fruitType_に最初に置くフルーツの種類をセット
	fruitData_ = Data::fruitDataList[fruitType_];
	coolDownTimer_ = PLAYER::COOL_DOWN_TIME;
}

Player::~Player()
{
}

void Player::Update()
{
	if (coolDownTimer_ > 0.0f)
	{
		coolDownTimer_ -= Time::GetDeltaTime();
	}

	if (ButtonArea::IsMouseKeepInArea(Data::areaList["endNormal"]))
	{
		return;
	}

	if (Input::GetMousePosition().x != Input::GetPrevMousePosition().x)
	{
		fruitPosition_.x = Input::GetMousePosition().x;
	}

	if (Input::IsButtonKeepDown("right") == true)
	{
		fruitPosition_.x += Time::GetDeltaTime() * PLAYER::MOVE_SPEED;
	}
	if (Input::IsButtonKeepDown("left") == true)
	{
		fruitPosition_.x -= Time::GetDeltaTime() * PLAYER::MOVE_SPEED;
	}

	// 位置補正
	if (fruitPosition_.x < Data::areaList["box"].leftTopX + fruitData_.distanceR)
	{
		fruitPosition_.x = Data::areaList["box"].leftTopX + fruitData_.distanceR;
	}
	else if (fruitPosition_.x > Data::areaList["box"].rightDownX - fruitData_.distanceR)
	{
		fruitPosition_.x = Data::areaList["box"].rightDownX - fruitData_.distanceR;
	}

	if (Input::IsButtonDown("put") && coolDownTimer_ <= 0.0f)
	{
		new Fruit(fruitType_, fruitPosition_);
		CreateFruitType();
		fruitData_ = Data::fruitDataList[fruitType_];

		coolDownTimer_ = PLAYER::COOL_DOWN_TIME; // 続けておけないようにする
	}

}

void Player::Draw()
{
	int width = (int)fruitData_.distanceR * 2;
	int height = (int)fruitData_.distanceR * 2;
	Image::DrawExtendGraph((int)(fruitPosition_.x - fruitData_.distanceR), (int)(fruitPosition_.y - fruitData_.distanceR), width, height, fruitData_.image);
	//Image::DrawExtendGraph((int)fruitPosition_.x, (int)fruitPosition_.y, width, height, fruitData_.image);
}

void Player::CreateFruitType()
{
	// 確率が単純
	// 出現率に偏りをだすのは後
	int number = rand() % 4;
	fruitType_ = Data::NumberToFruitType(number);
}
