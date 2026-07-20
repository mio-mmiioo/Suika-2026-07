#include "Fruit.h"
#include "../../Engine/Data/Image.h"
#include "../../Engine/Input.h"
#include "../../Engine/Time.h"
#include "GameMaster.h"

namespace FRUIT
{
	const float GRAVITY = 0.98f;
}

Fruit::Fruit(Data::FRUIT_TYPE fruitType, Point position)
{
	fruitData_ = Data::fruitDataList[fruitType];
	position_ = position;
	isOnGround_ = false;
	velocity_ = { 0.0f, 0.0f };

	angle_ = 0.0f;

	GameMaster::AddFruit(this);
}

Fruit::~Fruit()
{
}

void Fruit::Update()
{
	if (Input::IsButtonDown("right"))
	{
		angle_ += 0.1f;
	}

	position_.x += velocity_.x * Time::GetDeltaTime();
	position_.y += velocity_.y * Time::GetDeltaTime();

	velocity_.y += FRUIT::GRAVITY;
	if (isOnGround_ == true)
	{
		velocity_.y = 0.0f;
	}
}

void Fruit::Draw()
{
	float width = fruitData_.distanceR * 2;
	float height = fruitData_.distanceR * 2;
	Image::DrawExtendRotateGraph((int)(position_.x - fruitData_.distanceR), (int)(position_.y - fruitData_.distanceR), width, height, angle_, fruitData_.image);
}
