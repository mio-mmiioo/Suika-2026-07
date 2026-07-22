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
	bgm_ = "bgm02";
	Sound::Play(bgm_, TRUE);
}

TitleScene::~TitleScene()
{
	Sound::Stop(bgm_);
	delete newStart_;
}

void TitleScene::Update()
{
	newStart_->Update();
	if (newStart_->GetIsOnArea() == true)
	{
		Sound::Play("select", false); // 選択音
	}

	// ボタンが押された
	if (newStart_->GetIsPushArea() == true)
	{
		Sound::Play("decide", false); // 決定音
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
