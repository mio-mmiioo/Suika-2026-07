#include "PlayScene.h"
#include "../../Engine/Input.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../Data.h"
#include "../MyLibrary/ButtonArea.h"
#include "../../Engine/Time.h"
#include "Player.h"
#include "GameMaster.h"

#include "../../ImGui/imgui.h"

namespace PLAY_SCENE
{
	const float WAIT_CHANGE_SCENE_TIME = 60.0f;
}

PlayScene::PlayScene()
{
	GameMaster::Init();
	hBackground_ = Data::image["background"];
	player_ = new Player();

	Area endNormal = Data::areaList["endNormal"];
	Area endSelect = Data::areaList["endSelect"];
	end_ = new Button(endNormal, endSelect);

	isGameOver_ = false;
	timer_ = PLAY_SCENE::WAIT_CHANGE_SCENE_TIME;
}

PlayScene::~PlayScene()
{
	if (player_ != nullptr)
	{
		player_->DestroyMe();
	}
	GameMaster::Release();
}

void PlayScene::Update()
{
	end_->Update();

	if (isGameOver_ == false)
	{
		if (GameMaster::Update() == 1)
		{
			Sound::Play("gameOverFruit", false);
			isGameOver_ = true;
			player_->DestroyMe();
		}
	}
	else
	{
		timer_ -= Time::GetDeltaTime();
		if (timer_ <= 0.0f)
		{
			SceneManager::ChangeScene("TITLE");
		}
	}

	if (end_->GetIsPushArea() == true)
	{
		SceneManager::ChangeScene("TITLE");
		Sound::Play("decide", FALSE);
		if (isGameOver_ == false)
		{
			player_->DestroyMe();
		}
		return;
	}

	if (end_->GetIsOnArea() == true)
	{
		Sound::Play("select", false);
	}

	ImGui::Begin("PlayScene");
	ImGui::Text("time:%f", Time::GetDeltaTime());
	ImGui::End();
}

void PlayScene::Draw()
{
	Image::DrawGraph(0, 0, hBackground_);
	GameMaster::Draw();
	
	end_->Draw();
}
