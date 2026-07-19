#include "TitleScene.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Data/Image.h"
#include "../../Engine/Input.h"

#include "../Data.h"

TitleScene::TitleScene()
{
	Area newStartNormal = Data::areaList["newStartNormal"];
	Area newStartSelect = Data::areaList["newStartSelect"];
	newStart_ = new Button(newStartNormal, newStartSelect); // 新しく始めるボタン

	title_ = Data::areaList["title"];
	hBackground_ = Data::image["background"];
	hItigo_ = Data::image["itigo"];
}

TitleScene::~TitleScene()
{
	delete newStart_;
}

void TitleScene::Update()
{
	newStart_->Update();
	if (newStart_->GetIsOnArea() == true)
	{
		// 選択音
	}

	// ボタンが押された
	if (newStart_->GetIsPushArea() == true)
	{
		// 決定音
		SceneManager::ChangeScene("PLAY");
	}

	if (Input::IsButtonDown("next"))
	{
		SceneManager::ChangeScene("PLAY");
	}
}

void TitleScene::Draw()
{
	Image::DrawGraph(0, 0, hBackground_);
	ButtonArea::DrawArea(title_);
	newStart_->Draw();
}
