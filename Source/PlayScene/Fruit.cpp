#include "Fruit.h"
#include "../../Engine/Data/Image.h"

#include "../../Engine/Input.h"

Fruit::Fruit(Data::FRUIT_TYPE fruitType, Point position)
{
	fruitData_ = Data::fruitDataList[fruitType];
	position_ = position;

	angle_ = 0.0f;
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
}

void Fruit::Draw()
{
	float width = fruitData_.distanceR * 2;
	float height = fruitData_.distanceR * 2;
	Image::DrawExtendRotateGraph((int)position_.x, (int)position_.y, width, height, angle_, fruitData_.image);
}
