#include "ObjectManager.h"
#include "GamaObject.h"

namespace
{
	std::list<GameObject*>* objects; // 全オブジェクトのリスト
	bool isNeedSortDraw; // 描画順を考慮するならtrue
	GameObject* running;
}

void ObjectManager::Init()
{
	objects = new std::list<GameObject*>;
	objects->clear();
	isNeedSortDraw = false;
	running = nullptr;
}

void ObjectManager::Update()
{
	for (auto itr = objects->begin(); itr != objects->end(); itr++)
	{
		GameObject* obj = *itr;
		if (obj == nullptr)
		{
			continue;
		}
		if (obj->DestroyReauested())
		{
			delete obj;
			*itr = nullptr;
		}
		else
		{
			running = obj;
			obj->Update();
			running = nullptr;
		}

	}
	for (auto itr = objects->begin(); itr != objects->end();)
	{
		if (*itr == nullptr)
		{
			itr = objects->erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void ObjectManager::Draw()
{
	if (isNeedSortDraw == true)
	{
		objects->sort([](GameObject* a, GameObject* b) {return a->GetDrawOrder() > b->GetDrawOrder(); });
		isNeedSortDraw = false;
	}
	for (GameObject* obj : *objects)
	{
		if (obj == nullptr || obj->DestroyReauested())
		{
			continue;
		}
		obj->Draw();
	}
}

void ObjectManager::Release()
{
	while (objects->size() > 0)
	{
		auto itr = objects->begin();
		if (*itr != nullptr)
		{
			delete* itr;
		}
		objects->erase(itr);
	}
	objects->clear();
	delete objects;
	objects = nullptr;
}

void ObjectManager::Push(GameObject* obj)
{
	objects->push_back(obj);
	isNeedSortDraw = true;
}

void ObjectManager::SortByDrawOrder()
{
	isNeedSortDraw = true;
}

void ObjectManager::Pop(GameObject* obj)
{
	for (auto itr = objects->begin(); itr != objects->end(); itr++)
	{
		if (*itr == obj)
		{
			*itr = nullptr;
		}
	}
}

void ObjectManager::DeleteAllGameObject()
{
	for (auto itr = objects->begin(); itr != objects->end(); itr++)
	{
		GameObject* obj = *itr;
		if (not obj->IsDontDestroy())
		{
			delete obj;
			*itr = nullptr;
		}
		for (auto itr = objects->begin(); itr != objects->end();)
		{
			if (*itr == nullptr)
			{
				itr = objects->erase(itr);
			}
			else
			{
				itr++;
			}
		}
	}
}

const std::list<GameObject*>& ObjectManager::GetAllObject()
{
	return *objects;
}
