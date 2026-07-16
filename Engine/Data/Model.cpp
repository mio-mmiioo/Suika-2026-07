#include "Model.h"
#include "../DirectX/Direct3D.h"

namespace Model
{
	std::vector<ModelData*> modelList;
}

int Model::Load(std::string fileName)
{
	ModelData* pModelData = new ModelData;
	pModelData->fileName = fileName;
	pModelData->pFbx = nullptr;

	for (auto& itr : modelList)
	{
		if (itr->fileName == fileName)
		{
			pModelData->pFbx = itr->pFbx;
			break;
		}
	}

	if (pModelData->pFbx == nullptr)
	{
		pModelData->pFbx = new Fbx();
		pModelData->pFbx->Load(fileName.c_str());
	}
	modelList.push_back(pModelData);
	return((int)(modelList.size() - 1));
}

void Model::SetTransform(int hModel, Transform transform)
{
	modelList[hModel]->transform = transform;
}

void Model::Draw(int hModel)
{
	modelList[hModel]->pFbx->Draw(modelList[hModel]->transform);
}

void Model::Relase()
{
	bool isReffered = false; // 参照されているか
	for (int i = 0; i < modelList.size(); i++)
	{
		isReffered = false;
		for (int j = i + 1; j < modelList.size(); j++)
		{
			if (modelList[i]->pFbx == modelList[j]->pFbx)
			{
				isReffered = true;
				break;
			}
		}
		if (isReffered == false)
		{
			SAFE_DELETE(modelList[i]->pFbx);
		}
		SAFE_DELETE(modelList[i]);
	}
	modelList.clear(); // 配列を空にする
}
