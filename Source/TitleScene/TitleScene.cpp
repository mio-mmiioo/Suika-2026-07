#include "TitleScene.h"
#include "../../Engine/Data/Image.h"

TitleScene::TitleScene()
{
	backgroundImage_ = "background01.png";
	hBackground_ = Image::Load(backgroundImage_);
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
}

void TitleScene::Draw()
{
	Image::DrawGraph(0, 0, hBackground_);
}
