#include "TitleScene.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Input.h"

#include "../MyLibrary/Observer.h"
#include "../Data.h"

#include "../../ImGui/imgui.h"

TitleScene::TitleScene()
{
	AREA newStartNormal = Data::areaList["newStartNormal"];
	AREA newStartSelect = Data::areaList["newStartSelect"];
	newStart_ = new Button(newStartNormal, newStartSelect); // 新しく始めるボタン

	AREA continueStartNormal = Data::areaList["continueStartNormal"];
	AREA continueStartSelect = Data::areaList["continueStartSelect"];
	continueStart_ = new Button(continueStartNormal, continueStartSelect);

	title_ = Data::areaList["title"];
	hBackground_ = Data::image["background"];
	bgm_ = "bgm02";
	Sound::Play(bgm_, TRUE, false);
	volume_ = 0.75f;
	Sound::ChangeVolume(bgm_, volume_);
	
	int score = 0;
	Data::InitSaveFruitData(&score);
}

TitleScene::~TitleScene()
{
	Sound::Stop(bgm_);
	delete newStart_;
	delete continueStart_;
}

void TitleScene::Update()
{
	newStart_->Update();
	if (Data::saveFruitData.size() > 0)
	{
		continueStart_->Update();
	}

	if (newStart_->GetIsOnArea() == true ||
		continueStart_->GetIsOnArea() == true && Data::saveFruitData.size() > 0)
	{
		Sound::Play("select", false, true); // 選択音
	}

	// ボタンが押された
	if (newStart_->GetIsPushArea() == true)
	{
		Observer::SetIsNewGame(true);
		Observer::SetIsPrevGameCreated(true);
		Observer::SetScore(0); // 新しくゲームを始めるため、得点も0にする
		Sound::Play("decide", false, true); // 決定音
		SceneManager::ChangeScene("PLAY");
	}
	if (continueStart_->GetIsPushArea() == true && Data::saveFruitData.size() > 0)
	{
		Observer::SetIsNewGame(false);
		Observer::SetIsPrevGameCreated(false);
		Sound::Play("decide", false, true);
		SceneManager::ChangeScene("PLAY");
	}

	if (Input::IsButtonDown("next"))
	{
		SceneManager::ChangeScene("PLAY");
	}

#if _DEBUG
	ImGui::Begin("Title");
	ImGui::InputFloat("volume:", &volume_);
	Sound::ChangeVolume(bgm_, volume_);
	ImGui::End();

#endif
}

void TitleScene::Draw()
{
	Image::DrawGraph(0, 0, hBackground_);
	ButtonArea::DrawArea(title_);
	newStart_->Draw();
	if (Data::saveFruitData.size() > 0)
	{
		continueStart_->Draw();
	}
}
