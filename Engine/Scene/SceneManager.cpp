#include "SceneManager.h"
#include "Scene.h"

#include "../../Source/TitleScene/TitleScene.h"
#include "../../Source/PlayScene/PlayScene.h"

namespace SceneManager
{
	/// <summary>
	/// 最初のシーンを作成する
	/// </summary>
	void CreateFirst();

	Scene* current; // 現在のシーン
}

void SceneManager::CreateFirst()
{
	current = new TitleScene();
}

void SceneManager::ChangeScene(const std::string& name)
{
	Scene* prev = current;
	if (name == "TITLE")
	{
		current = new TitleScene();
	}
	else if (name == "PLAY")
	{
		current = new PlayScene();
	}
	else
	{
		// シーンを切り替えられない
	}
	prev->~Scene();
}

void SceneManager::Init()
{
	CreateFirst();
}

void SceneManager::Update()
{
	current->Update();
}

void SceneManager::Draw()
{
	current->Draw();
}

void SceneManager::Relase()
{
	current->~Scene();
}
