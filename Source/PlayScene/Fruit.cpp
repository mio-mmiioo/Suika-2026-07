#include "Fruit.h"
#include "../../Engine/Data/Image.h"
#include "../../Engine/Time.h"
#include "GameMaster.h"

Fruit::Fruit(Data::FRUIT_TYPE fruitType, Point position)
{
	fruitData_ = Data::fruitDataList[fruitType];
	position_ = position;
	isOnGround_ = false;
	velocity_ = { 0.0f, 0.0f };

	isSleep_ = false;
	supportCount_ = 0;
	supportCountMin_ = (int)Data::fruitPhysics["supportCountMin"];
	sleepCount_ = 0;

	isGameOverFruit_ = false;

	angle_ = 0.0f;

	GameMaster::AddFruit(this);
}

Fruit::~Fruit()
{
}

void Fruit::Update()
{
	if (isSleep_ == true)
	{
		return;
	}

	// 力の加算　重力など
	position_.x += velocity_.x * Time::GetDeltaTime();
	position_.y += velocity_.y * Time::GetDeltaTime();

	if (supportCount_ < supportCountMin_)
	{
		velocity_.y += Data::fruitPhysics["gravity"];
	}

	if (isOnGround_ == true)
	{
		velocity_.x = velocity_.x * Data::fruitPhysics["groundFrection"]; // 地面では転がりにくくする
	}
	else
	{
		velocity_.x = velocity_.x * Data::fruitPhysics["airResistance"]; // 空中での抵抗
	}

	// x方向の力の向きによって回転角度を変える
	if (velocity_.x > 0)
	{
		angle_ += 0.1f;
		if (angle_ >= 360.0f)
		{
			angle_ = 0.0f;
		}
	}
	else
	{
		angle_ -= 0.1f;
		if (angle_ <= 0.0f)
		{
			angle_ = 360.0f;
		}
	}

	isOnGround_ = false;

	// めり込み確認
	{
		// フルーツ同士はGameMasterがUpdate内で行っている
		if (isGameOverFruit_ == false)
		{
			if (GameMaster::FruitCheckBoxPosition(this) == true)
			{
				isOnGround_ = true;
				velocity_.y = 0.0f;
			}
		}
	}

	// しばらく動いていないかの判定
	float sleepSq = velocity_.x * velocity_.x + velocity_.y * velocity_.y;
	if (sleepSq < Data::fruitPhysics["sleepLimitSq"] || supportCount_ >= supportCountMin_)
	{
		sleepCount_ += 1;
	}
	else
	{
		sleepCount_ = 0;
	}

	if (supportCount_ >= supportCountMin_)
	{
		isSleep_ = true;
		velocity_ = { 0.0f, 0.0f };
	}
	if (sleepCount_ > Data::fruitPhysics["maxSleepCount"])
	{
		isSleep_ = true;
		velocity_ = { 0.0f, 0.0f };
	}
}

void Fruit::Draw()
{
	float width = fruitData_.distanceR * 2;
	float height = fruitData_.distanceR * 2;
	Image::DrawExtendRotateGraph((int)(position_.x - fruitData_.distanceR), (int)(position_.y - fruitData_.distanceR), width, height, angle_, fruitData_.image);
}

void Fruit::WakeUp()
{
	isSleep_ = false;
	sleepCount_ = 0;
}

void Fruit::SetIsGameOverFruit()
{
	isGameOverFruit_ = true;
}
