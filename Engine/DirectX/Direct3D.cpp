#include <d3dcompiler.h>
#include "Direct3D.h"
#include <DirectXMath.h>

namespace Direct3D
{
	float BACKGROUND_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // 背景の色　R,G,B,A

	ID3D11Device*			pDevice;			// デバイス
	ID3D11DeviceContext*	pContext;			// デバイスコンテキスト
	IDXGISwapChain*			pSwapChain;			// スワップチェイン
	ID3D11RenderTargetView* pRenderTargetView;	// レンダーターゲットビュー
	ID3D11Texture2D*		pDepthStencil;		// 深度ステンシル
	ID3D11DepthStencilView* pDepthStencilView;	// 深度ステンシルビュー

	struct SHADER_BUNDLE
	{
		ID3D11VertexShader*		pVertexShader;		// 頂点シェーダー
		ID3D11PixelShader*		pPixelShader;		// ピクセルシェーダー
		ID3D11InputLayout*		pVertexLayout;		// 頂点インプットレイアウト
		ID3D11RasterizerState*	pRasterizerState;	// ラスタライザー
	};

	SHADER_BUNDLE shaderBundle[MAX_SHADER_TYPE]; // シェーダー
	ID3D11DepthStencilState* pDepthState2D;
	ID3D11DepthStencilState* pDepthState3D;
}

HRESULT Direct3D::Initialize(int windowWidth, int windowHeight, HWND hWnd)
{
	HRESULT hr;
	// デバイスコンテキストなどをセットするための設定

	DXGI_SWAP_CHAIN_DESC scDesc; // 設定項目をまとめた構造体
	ZeroMemory(&scDesc, sizeof(scDesc)); // 0で初期化

	// 描画先のフォーマット
	scDesc.BufferDesc.Width = windowWidth;		// ウィンドウの横幅
	scDesc.BufferDesc.Height = windowHeight;	// ウィンドウの高さ
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 何色使えるか

	// FPS (1/60秒に1回)
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	// その他
	scDesc.Windowed = TRUE;		// ウィンドウモードかフルスクリーンか
	scDesc.OutputWindow = hWnd; // ウィンドウハンドル
	scDesc.BufferCount = 1;		// バックバッファの枚数
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // バックバッファーの使い道　画面に描画する
	scDesc.SampleDesc.Count = 1;	// MSAA( マルチサンプリング・アンチエイリアシング )の設定
	scDesc.SampleDesc.Quality = 0;	// MSAA( マルチサンプリング・アンチエイリアシング )の設定

	// デバイス、コンテキスト、スワップチェインを作成
	D3D_FEATURE_LEVEL level;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,					// どのビデオアダプタを使用するか、nullptrだと既定
		D3D_DRIVER_TYPE_HARDWARE,	// ドライバのタイプを渡す、通常はHARDWARE
		nullptr,					// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定しない限りnullptr
		0,							// 何らかのフラグを指定する
		nullptr,					// デバイス、コンテキストのレベルを設定、nullptrにすれば問題なし
		0,							// 上の引数でレベルを何個指定したか
		D3D11_SDK_VERSION,			// SDKのバージョン
		&scDesc,					// 上で設定した構造体
		&pSwapChain,				// 完成したSwapChainのアドレスが返ってくる
		&pDevice,					// 完成したDeviceのアドレスが返ってくる
		&level,						// 完成したDevice, Contextのレベルが返ってくる
		&pContext					// 完成したContextのアドレスが返ってくる
	);

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct3D Initialize()：デバイス、コンテキスト、スワップチェインの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	// レンダーターゲットビュー作成
	// スワップチェーンからバックバッファを取得
	ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView); // レンダーターゲットビューを作成
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct3D Initialize()：レンダーターゲットビューの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	pBackBuffer->Release(); // 解放

	// ビューポート( 描画範囲 )設定
	D3D11_VIEWPORT vp;	// レンダリング結果を表示する範囲
	vp.Width = (FLOAT)windowWidth;		// 横幅
	vp.Height = (FLOAT)windowHeight;	// 高さ
	vp.MinDepth = 0.0f; // 手前
	vp.MaxDepth = 1.0f; // 奥
	vp.TopLeftX = 0;	// 左
	vp.TopLeftY = 0;	// 上

	// 深度ステンシルビューの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = windowWidth;
	descDepth.Height = windowHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Direct3D Initialize()：深度ステンシルの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	hr = pDevice->CreateDepthStencilView(pDepthStencil, NULL, &pDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Direct3D Initialize()：深度ステンシルビューの作成に失敗しました", L"エラー", MB_OK);
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthState2D);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Direct3D Initialize()：2Dのデプスステンシルステートの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	depthStencilDesc.DepthEnable = TRUE;
	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthState3D);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Direct3D Initialize()：3Dのデプスステンシルステートの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	// データを画面に描画するための一通りの設定( パイプライン )
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // データの入力種類を指定
	pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView); // 描画先を設定
	pContext->RSSetViewports(1, &vp);

	InitShader(); // シェーダーの初期化

	return S_OK;
}

HRESULT Direct3D::InitShader()
{
	if (FAILED(InitShader3D()))
	{
		return E_FAIL;
	}
	if (FAILED(InitShader2D()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Direct3D::InitShader3D()
{
	HRESULT hr;
	// 頂点シェーダーの作成
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"Engine/Shader/Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &pCompileVS, NULL);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &(shaderBundle[SHADER_3D].pVertexShader));
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct3D.cpp InitShader3D()：頂点シェーダの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	// 頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, // 位置
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0}, // UV座標
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}, // 法線
	};
	hr = pDevice->CreateInputLayout(layout, 3, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderBundle[SHADER_3D].pVertexLayout);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct3D.cpp InitShader3D()：頂点インプットレイアウトの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	pCompileVS->Release();

	// ピクセルシェーダーの作成
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Engine/Shader/Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderBundle[SHADER_3D].pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct3D.cpp InitShader3D()：ピクセルシェーダーの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	// ラスタライザ作成
	D3D11_RASTERIZER_DESC rasterizer = {};
	rasterizer.CullMode = D3D11_CULL_BACK;
	rasterizer.FillMode = D3D11_FILL_SOLID;
	rasterizer.FrontCounterClockwise = FALSE; // ポリゴンのどちらの面を表側にするか　通常は頂点が時計回りに見える面が表
	hr = pDevice->CreateRasterizerState(&rasterizer, &shaderBundle[SHADER_3D].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct3D.cpp InitShader3D()：ラスタライザの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Direct3D::InitShader2D()
{
	HRESULT hr;
	// 頂点シェーダーの作成
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"Engine/Shader/Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &pCompileVS, NULL);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &(shaderBundle[SHADER_2D].pVertexShader));
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct3D.cpp InitShader2D()：頂点シェーダの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	// 頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, // 位置
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0}, // UV座標
	};
	hr = pDevice->CreateInputLayout(layout, 2, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderBundle[SHADER_2D].pVertexLayout);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct3D.cpp InitShader2D()：頂点インプットレイアウトの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	pCompileVS->Release();

	// ピクセルシェーダーの作成
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Engine/Shader/Simple2D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderBundle[SHADER_2D].pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct3D.cpp InitShader2D()：ピクセルシェーダーの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	// ラスタライザ作成
	D3D11_RASTERIZER_DESC rasterizer = {};
	rasterizer.CullMode = D3D11_CULL_BACK;
	rasterizer.FillMode = D3D11_FILL_SOLID;
	rasterizer.FrontCounterClockwise = FALSE; // ポリゴンのどちらの面を表側にするか　通常は頂点が時計回りに見える面が表
	hr = pDevice->CreateRasterizerState(&rasterizer, &shaderBundle[SHADER_2D].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Direct3D.cpp InitShader3D()：ラスタライザの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

void Direct3D::SetShader(SHADER_TYPE type)
{
	// それぞれの情報をセット
	pContext->VSSetShader(shaderBundle[type].pVertexShader, NULL, 0);
	pContext->PSSetShader(shaderBundle[type].pPixelShader, NULL, 0);
	pContext->IASetInputLayout(shaderBundle[type].pVertexLayout);
	pContext->RSSetState(shaderBundle[type].pRasterizerState);

	if (type == SHADER_TYPE::SHADER_2D)
	{
		pContext->OMSetDepthStencilState(pDepthState2D, 0);
	}
	else if (type == SHADER_TYPE::SHADER_3D)
	{
		pContext->OMSetDepthStencilState(pDepthState3D, 0);
	}
}

void Direct3D::BeginDraw()
{
	pContext->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR); // 画面をクリア

	// 深度バッファクリア
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::EndDraw()
{
	// スワップ( バックバッファを表に表示 )
	pSwapChain->Present(0, 0);
}

void Direct3D::Release()
{
	for (int count = 0; count < MAX_SHADER_TYPE; count++)
	{
		SAFE_RELEASE(shaderBundle[count].pRasterizerState);
		SAFE_RELEASE(shaderBundle[count].pVertexLayout);
		SAFE_RELEASE(shaderBundle[count].pPixelShader);
		SAFE_RELEASE(shaderBundle[count].pVertexShader);
	}

	SAFE_RELEASE(pDepthStencilView);
	SAFE_RELEASE(pDepthStencil);
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pSwapChain);
	SAFE_RELEASE(pContext);
	SAFE_RELEASE(pDevice);
}
