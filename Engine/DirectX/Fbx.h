#pragma once
#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include <vector>
#include "../ObjectManager/Transform.h"
#include "Texture.h"

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

class Fbx
{
	// マテリアル
	struct MATERIAL
	{
		Texture* pTexture;			// テクスチャ
		DirectX::XMFLOAT4 diffuse;	// 拡散反射
	};

	struct CONSTANT_BUFFER
	{
		DirectX::XMMATRIX matWVP;		// ワールド・ビュー・プロジェクションの行列
		DirectX::XMMATRIX matNormal;	// 法線変換用の行列
		DirectX::XMFLOAT4 diffuse;		// ワールド行列
		BOOL isUseMaterial;				// マテリアルを使用するならtrue
	};

	struct VERTEX
	{
		DirectX::XMVECTOR position; // 位置
		DirectX::XMVECTOR uv;		// UV座標
		DirectX::XMVECTOR normal;	// 法線
	};

public:
	Fbx();
	~Fbx();

	/// <summary>
	/// Fbxファイルの読み込み
	/// </summary>
	/// <param name="fileName">読み込むファイルのパス</param>
	/// <returns>成功したか失敗したか</returns>
	HRESULT Load(std::string fileName);

	/// <summary>
	/// 読み込んだファイルの描画
	/// </summary>
	/// <param name="transform">( 位置, 回転, 拡縮 )</param>
	void Draw(Transform& transform);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

private:
	/// <summary>
	/// 頂点バッファの準備 Loadで呼ばれる
	/// </summary>
	/// <param name="mesh">3Dモデルの形状(頂点座標、法線、UV座標、、、、)</param>
	/// <returns>成功したか失敗したか( 失敗した場合、原因を返す )</returns>
	HRESULT InitVertex(fbxsdk::FbxMesh* mesh);

	/// <summary>
	/// インデックスバッファの準備 Loadで呼ばれる
	/// </summary>
	/// <param name="mesh">3Dモデルの形状</param>
	/// <returns>成功したか失敗したか( 失敗した場合、原因を返す )</returns>
	HRESULT InitIndex(fbxsdk::FbxMesh* mesh);

	/// <summary>
	/// コンスタントバッファの準備 Loadで呼ばれる
	/// </summary>
	/// <returns>成功したか失敗したか( 失敗した場合、原因を返す )</returns>
	HRESULT InitConstantBuffer();

	/// <summary>
	/// マテリアルの準備
	/// </summary>
	/// <param name="pNode">オブジェクト全体の情報</param>
	/// <returns>成功したかどうか</returns>
	HRESULT InitMaterial(fbxsdk::FbxNode* pNode);

	int vertexCount_;	// 頂点数
	int polygonCount_;	// ポリゴン数
	int materialCount_; // マテリアルの数

	std::vector<int> indexCount_;

	ID3D11Buffer* pVertexBuffer_;			// 頂点バッファ
	ID3D11Buffer** pIndexBuffer_;			// インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;			// コンスタントバッファ
	std::vector<MATERIAL> pMaterialList_;	// マテリアルのリスト

	std::vector<VERTEX> vertices_; // 頂点配列
	std::vector<std::vector<int>> indicesPerMat_; // [material][index]
};

