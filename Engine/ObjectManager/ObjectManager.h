#pragma once
#include <list>

class GameObject;

/// <summary>
/// GameObjectを管理する
/// </summary>
namespace ObjectManager
{
	void Init();
	void Update();
	void Draw();
	void Release();

	/// <summary>
	/// GameObjectの追加
	/// </summary>
	/// <param name="obj">GameObjectのインスタンス</param>
	void Push(GameObject* obj);

	/// <summary>
	/// 描画順を作り直す
	/// </summary>
	void SortByDrawOrder();

	/// <summary>
	/// GameObjectを削除する
	/// </summary>
	/// <param name="obj">GameObjectのインスタンス</param>
	void Pop(GameObject* obj);

	/// <summary>
	/// 全てのGameObjectを削除する
	/// </summary>
	void DeleteAllGameObject();

	const std::list<GameObject*>& GetAllObject();

	/// <summary>
	/// クラス名でオブジェクトを探す
	/// </summary>
	/// <typeparam name="C">クラス</typeparam>
	/// <returns>オブジェクトの実態( 存在しないならnullptr )</returns>
	template<class C> C* FindGameObject()
	{
		const std::list<GameObject*>& objs = GetAllObject();
		for (GameObject* obj : objs)
		{
			C* inst = dynamic_cast<C*>(obj);
			if (inst != nullptr)
			{
				return inst;
			}
			return nullptr;
		}
	}

	/// <summary>
	/// クラスのオブジェクトをすべて探す
	/// </summary>
	/// <typeparam name="C">クラス名</typeparam>
	/// <returns>オブジェクトの実態</returns>
	template<class C> std::list<C*> FindGameObjects()
	{
		std::list<C*> ret;
		ret.clear();
		const std::list<GameObject>& objs = ObjectManager::GetAllObject();

		for (GameObject* obj : objs)
		{
			C* inst = dynamic_cast<C*>(obj);
			if (inst != nullptr)
			{
				ret.emplace_back(inst);
			}
		}
		return ret;
	}
}