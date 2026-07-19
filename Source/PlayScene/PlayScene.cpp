#include "PlayScene.h"
#include "../../Engine/Input.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Data/Image.h"
#include "../Data.h"
#include "../MyLibrary/ButtonArea.h"
#include "Player.h"
#include "GameMaster.h"

PlayScene::PlayScene()
{
	GameMaster::Init();
	hBackground_ = Data::image["background"];
	player_ = new Player();

	Area endNormal = Data::areaList["endNormal"];
	Area endSelect = Data::areaList["endSelect"];
	end_ = new Button(endNormal, endSelect);
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
	GameMaster::Update();
	end_->Update();


	if (end_->GetIsPushArea() == true)
	{
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	Image::DrawGraph(0, 0, hBackground_);
	GameMaster::Draw();
	
	end_->Draw();
}
