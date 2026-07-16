#pragma once
#include <string>

namespace SceneManager
{
	/// <summary>
	/// シーンを切り替える
	/// </summary>
	/// <param name="name">次のシーン</param>
	void ChangeScene(const std::string& name);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
	/// <summary>
	/// 解放
	/// </summary>
	void Relase();
}