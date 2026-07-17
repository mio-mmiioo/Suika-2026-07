#include "Player.h"
#include "../../Engine/Data/Image.h"
#include "../../Engine/Input.h"
#include "../Data.h"

Player::Player()
{
	fruitPosition_ = { 0.0f, 100.0f };
	coolDownTimer_ = 0.0f;
	hFruitImage_ = Data::image["itigo"];
}

Player::~Player()
{
}

void Player::Update()
{
	if (Input::GetMousePosition().x != Input::GetPrevMousePosition().x)
	{
		fruitPosition_.x = Input::GetMousePosition().x;
	}

	if (Input::IsButtonKeepDown("right") == true)
	{
		fruitPosition_.x += 1.0f;
	}
	if (Input::IsButtonKeepDown("left") == true)
	{
		fruitPosition_.x -= 1.0f;
	}

	// 位置補正
	if (fruitPosition_.x < 100.0f)
	{
		fruitPosition_.x = 100.0f;
	}
	else if (fruitPosition_.x > 924.0f)
	{
		fruitPosition_.x = 924.0f;
	}

}

void Player::Draw()
{
	//Image::DrawGraph((int)fruitPosition_.x, (int)fruitPosition_.y, hFruitImage_);
	Image::DrawExtendGraph((int)fruitPosition_.x, (int)fruitPosition_.y, 100, 100, hFruitImage_);
}
