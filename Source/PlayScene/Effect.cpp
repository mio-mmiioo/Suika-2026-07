#include "Effect.h"
#include "../Data.h"
#include "../../Engine/Time.h"

namespace EFFECT
{
	const float TIME = 0.06f;	// エフェクトをする時間
	const int MAX_ANIM = 4;		// アニメーションの最大数
}

Effect::Effect(MY_POINT position, int fruitDistanceR)
{
	position_ = position;
	position_.x -= (float)fruitDistanceR;
	position_.y -= (float)fruitDistanceR;
	timer_ = EFFECT::TIME;
	hImage_ = Data::image["effect"];
	anim_ = 0;
	size_ = fruitDistanceR * 2;
	Image::GetGraphSize(hImage_, &imageWidth_, &imageHeight_);
}

Effect::~Effect()
{
	hImage_ = -1;
}

void Effect::Update()
{
	timer_ -= Time::GetDeltaTime();
	// timer_が0以下になったら、切り抜く画像の位置をずらす
	if (timer_ <= 0.0f)
	{
		anim_ += 1;
		timer_ += EFFECT::TIME;
	}
	// すべてのアニメーションを再生したら削除
	if (anim_ >= EFFECT::MAX_ANIM)
	{
		DestroyMe();
	}
}

void Effect::Draw()
{
	int x = position_.x;
	int y = position_.y;
	int width = size_;
	int height = size_;
	int rectX = anim_ * imageWidth_ / EFFECT::MAX_ANIM;
	int rectY = 0;
	int rectWidth = imageWidth_ / EFFECT::MAX_ANIM;
	int rectHeight = imageHeight_;
	Image::DrawRectExtendGraph(x, y, width, height, rectX, rectY, rectWidth, rectHeight, hImage_);
}
