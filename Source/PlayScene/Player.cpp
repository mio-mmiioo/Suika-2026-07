#include "Player.h"
#include "../../Engine/Data/Image.h"
#include "../../Engine/Input.h"

Player::Player()
{
	fruitPosition_ = { 0.0f, 100.0f };
	coolDownTimer_ = 0.0f;
	hFruitImage_ = Image::Load("itigo.png");
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

}

void Player::Draw()
{
	Image::DrawGraph((int)fruitPosition_.x, (int)fruitPosition_.y, hFruitImage_);
}
