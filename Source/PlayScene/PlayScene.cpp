#include "PlayScene.h"
#include "../../Engine/Input.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Data/Image.h"
#include "Player.h"
#include "../Data.h"

PlayScene::PlayScene()
{
	hBackground_ = Data::image["background"];
	player_ = new Player();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (Input::IsButtonDown("next"))
	{
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	//Image::DrawGraph(0, 0, hBackground_);
}
