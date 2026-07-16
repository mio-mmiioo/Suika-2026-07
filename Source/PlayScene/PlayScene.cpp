#include "PlayScene.h"
#include "../../Engine/Input.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Data/Image.h"

PlayScene::PlayScene()
{
	hBackground_ = Image::Load("background01.png");
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
