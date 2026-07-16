#include "TitleScene.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Data/Image.h"
#include "../../Engine/Input.h"

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
	if (Input::IsButtonDown("next"))
	{
		SceneManager::ChangeScene("PLAY");
	}
}

void TitleScene::Draw()
{
	Image::DrawGraph(0, 0, hBackground_);
}
