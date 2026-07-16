#include "Fbx.h"
#include "Direct3D.h"
#include "../Camera.h"
#include <filesystem>

Fbx::Fbx()
	:pVertexBuffer_(nullptr),
	pIndexBuffer_(nullptr),
	pConstantBuffer_(nullptr),
	pMaterialList_(0),
	vertexCount_(0),
	polygonCount_(0),
	materialCount_(0)
{
}

Fbx::~Fbx()
{
}

HRESULT Fbx::Load(std::string fileName)
{
	std::string subDir("Assets");
	std::filesystem::path currentPath;
	std::filesystem::path basePath;
	currentPath = std::filesystem::current_path();
	basePath = currentPath;
	currentPath = currentPath / subDir;
	std::filesystem::current_path(currentPath);

	FbxManager* pFbxManager = FbxManager::Create(); // マネージャーを作成
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp"); // インポーターを作成
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	// シーンオブジェクトにFBXファイルの情報を流し込む
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);
	fbxImporter->Destroy();

	// メッシュ情報を取得
	FbxNode* rootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	FbxMesh* mesh = pNode->GetMesh();

	// 各情報の個数を取得
	vertexCount_ = mesh->GetControlPointsCount();	// 頂点の数
	polygonCount_ = mesh->GetPolygonCount();		// ポリゴンの数
	materialCount_ = pNode->GetMaterialCount();		// マテリアルの数

	if (FAILED(InitVertex(mesh)))
	{
		return E_FAIL;
	}
	if (FAILED(InitIndex(mesh)))
	{
		return E_FAIL;
	}
	if (FAILED(InitConstantBuffer()))
	{
		return E_FAIL;
	}
	if (FAILED(InitMaterial(pNode)))
	{
		return E_FAIL;
	}
	
	std::filesystem::current_path(basePath);
	pFbxManager->Destroy();
	return S_OK;
}

void Fbx::Draw(Transform& transform)
{
	Direct3D::SetShader(Direct3D::SHADER_3D);
	transform.Calclation();

	CONSTANT_BUFFER constantBuffer;
	constantBuffer.matWVP = DirectX::XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	constantBuffer.matNormal = transform.GetNormalMatrix();

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);

	for (int count = 0; count < materialCount_; count++)
	{
		if (pMaterialList_[count].pTexture)
		{
			constantBuffer.isUseMaterial = TRUE;
			constantBuffer.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); // なくても問題はないが、万が一の時のため
		}
		else
		{
			constantBuffer.isUseMaterial = FALSE;
			constantBuffer.diffuse = pMaterialList_[count].diffuse;
		}

		D3D11_MAPPED_SUBRESOURCE pData;
		Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&constantBuffer), sizeof(constantBuffer));

		Direct3D::pContext->Unmap(pIndexBuffer_[count], 0);

		// インデックスバッファをセット
		stride = sizeof(int);
		offset = 0;
		Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_[count], DXGI_FORMAT_R32_UINT, 0);

		if (pMaterialList_[count].pTexture)
		{
			ID3D11SamplerState* pSampler = pMaterialList_[count].pTexture->GetSampler();
			Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
			ID3D11ShaderResourceView* pSRV = pMaterialList_[count].pTexture->GetSRV();
			Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
		}

		// 描画
		Direct3D::pContext->DrawIndexed(indexCount_[count], 0, 0);
	}
}

void Fbx::Release()
{
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

HRESULT Fbx::InitVertex(fbxsdk::FbxMesh* mesh)
{
	vertices_.resize(vertexCount_);
	// 全ポリゴン
	for (long poly = 0; poly < polygonCount_; poly++)
	{
		// 3頂点分
		for (int vertex = 0; vertex < 3; vertex++)
		{
			// 調べる頂点の番号
			int index = mesh->GetPolygonVertex(poly, vertex);

			// 頂点の位置
			FbxVector4 pos = mesh->GetControlPointAt(index);
			vertices_[index].position = DirectX::XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);

			// 頂点のUV
			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2 uv = pUV->GetDirectArray().GetAt(uvIndex);

			vertices_[index].uv = DirectX::XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 1.0f);

			// 頂点の法線
			FbxVector4 normal;
			mesh->GetPolygonVertexNormal(poly, vertex, normal);
			vertices_[index].normal = DirectX::XMVectorSet((float)normal[0], (float)normal[1], (float)normal[2], 0.0f);
		}
	}

	// 頂点バッファ作成
	HRESULT hr;
	D3D11_BUFFER_DESC bdVertex;
	bdVertex.ByteWidth = sizeof(VERTEX) * vertexCount_;
	bdVertex.Usage = D3D11_USAGE_DEFAULT;
	bdVertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bdVertex.CPUAccessFlags = 0;
	bdVertex.MiscFlags = 0;
	bdVertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA dataVertex;
	dataVertex.pSysMem = vertices_.data();
	hr = Direct3D::pDevice->CreateBuffer(&bdVertex, &dataVertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Fbx.cpp InitVertex()：頂点バッファの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Fbx::InitIndex(fbxsdk::FbxMesh* mesh)
{
	pIndexBuffer_ = new ID3D11Buffer * [materialCount_];
	indicesPerMat_.resize(materialCount_);
	indexCount_ = std::vector<int>(materialCount_);

	for (int count = 0; count < materialCount_; count++)
	{
		auto& indices = indicesPerMat_[count];

		// 全ポリゴン
		for (long poly = 0; poly < polygonCount_; poly++)
		{
			FbxLayerElementMaterial* mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);

			if (mtlId == count)
			{
				for (long vertex = 0; vertex < 3; vertex++)
				{
					indices.push_back(mesh->GetPolygonVertex(poly, vertex));
				}
			}
		}

		indexCount_[count] = (int)indices.size();
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(int) * indexCount_[count];
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = indices.data();
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		HRESULT hr;
		hr = Direct3D::pDevice->CreateBuffer(&bufferDesc, &initData, &pIndexBuffer_[count]);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"Fbx.cpp InitIndex()：インデックスバッファの作成に失敗しました", L"エラー", MB_OK);
			return hr;
		}
	}
	return S_OK;
}

HRESULT Fbx::InitConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(CONSTANT_BUFFER);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// コンスタントバッファ
	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&bufferDesc, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Fbx.cpp InitConstantBuffer()：コンスタントバッファの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Fbx::InitMaterial(fbxsdk::FbxNode* pNode)
{
	pMaterialList_.resize(materialCount_);
	for (int count = 0; count < materialCount_; count++)
	{
		// count番目のマテリアル情報を取得
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(count);
		FbxProperty ImageProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		int fileTextureCount = ImageProperty.GetSrcObjectCount<FbxFileTexture>();

		// テクスチャあり
		if (fileTextureCount > 0)
		{
			FbxFileTexture* textureInfo = ImageProperty.GetSrcObject<FbxFileTexture>(0);
			const char* textureFilePath = textureInfo->GetRelativeFileName();

			std::filesystem::path tPath(textureFilePath);
			if (std::filesystem::is_regular_file(tPath))
			{
				pMaterialList_[count].pTexture = new Texture();
				pMaterialList_[count].pTexture->Load(tPath.string());
			}
			else
			{
				MessageBox(NULL, L"Fbx.cpp InitMaterial()：テクスチャファイルが見つかりませんでした", L"エラー", MB_OK);
			}
		}
		// テクスチャなし
		else
		{
			pMaterialList_[count].pTexture = nullptr;

			// マテリアルの色
			FbxSurfaceLambert* pMaterial = (FbxSurfaceLambert*)pNode->GetMaterial(count);
			FbxDouble3 diffuse = pMaterial->Diffuse;
			pMaterialList_[count].diffuse = DirectX::XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
		}
	}
	return S_OK;
}
