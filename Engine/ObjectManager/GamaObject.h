#pragma once
#include "ObjectManager.h"

/// <summary>
/// 他のオブジェクトの基底クラス
/// これを継承したクラスは、ObjectManagerで管理されて、自動でUpdate()とDraw()が呼ばれる
/// </summary>
class GameObject
{
public:
	GameObject() : isDestroy_(false), isDontDestroy_(false), drawOrder_(0) { ObjectManager::Push(this); }
	virtual ~GameObject() { ObjectManager::Pop(this); }

	/// <summary>
	/// マイフレーム呼ばれる更新処理
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// マイフレーム呼ばれる描画処理
	/// </summary>
	virtual void Draw() {};

	/// <summary>
	/// 自身のインスタンスを削除する
	/// 次回のUpdateの前に削除される
	/// </summary>
	virtual void DestroyMe() { isDestroy_ = true; }

	/// <summary>
	/// DestroyMeが呼ばれているか調べる
	/// </summary>
	/// <returns>DestoryMeが呼ばれているとtrue</returns>
	inline bool DestroyReauested() const { return isDestroy_; }

	/// <summary>
	/// シーンチェンジするときに、削除されなくする
	/// </summary>
	/// <param name="sw">書かないとtrue,falseにすると削除される</param>
	void DontDestroyOnSceneChange(bool sw = true) { isDontDestroy_ = sw; }

	/// <summary>
	/// DontDestroyが設定されているかを調べる
	/// </summary>
	/// <returns>DontDestroyが設定されているならtrue</returns>
	bool IsDontDestroy() const { return isDontDestroy_; }

	/// <summary>
	/// 描画の優先順位を指定する
	/// 数値が高いほど先に描画され、指定しない場合は0
	/// </summary>
	/// <param name="order">描画順位</param>
	void SetDrawOrder(int order)
	{
		drawOrder_ = order;
		ObjectManager::SortByDrawOrder();
	}

	/// <summary>
	/// 描画の優先順位を取得する
	/// </summary>
	/// <returns>描画の優先順位</returns>
	inline int GetDrawOrder() const { return drawOrder_; }

private:
	bool isDestroy_;
	bool isDontDestroy_;
	int drawOrder_;
};