#include <Windows.h>
#include "Scene/SceneManager.h"
#include "ObjectManager/ObjectManager.h"
#include "DirectX/Direct3D.h"
#include "Screen.h"
#include "Camera.h"
#include "Input.h"
#include "Time.h"
#include "Data/Sound.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LPCTSTR WIN_CLASS_NAME = TEXT("SampleGame"); // ウィンドウクラス名

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウクラスを作成
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);				// 構造体のサイズ
	wc.hInstance = hInstance;					// インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;			// ウィンドウクラス名
	wc.lpfnWndProc = WndProc;					// ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;			// スタイル（デフォルト）
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// アイコン
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);	// 小さいアイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// マウスカーソル
	wc.lpszMenuName = NULL;						// メニュー( なし )
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 背景
	RegisterClassEx(&wc); // クラスを登録

	// ウィンドウサイズの計算
	RECT winRect = { 0, 0, Screen::WINDOW_WIDTH, Screen::WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE); // 希望するサイズ、ウィンドウスタイル、メニューバーの有無
	int windowWidth = winRect.right - winRect.left;	// ウィンドウの横幅
	int windowHeight = winRect.bottom - winRect.top;	// ウィンドウの高さ

	// ウィンドウを作成
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,			// ウィンドウクラス名
		L"サンプルゲーム",		// タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW,	// スタイル( 普通のウィンドウ )
		CW_USEDEFAULT,			// 表示位置左( おまかせ )
		CW_USEDEFAULT,			// 表示位置上( おまかせ )
		windowWidth,			// ウィンドウの横幅
		windowHeight,			// ウィンドウの高さ
		NULL,					// 親ウィンドウ( なし )
		NULL,					// メニュー( なし )
		hInstance,				// インスタンス
		NULL					// パラメータ( なし )
	);

	// ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);
	Direct3D::Initialize(windowWidth, windowHeight, hWnd);
	Sound::Initialize();
	Input::Initialize(hWnd);
	Time::Initialize(hWnd);
	ObjectManager::Init();
	SceneManager::Init();
	Camera::Initialize();


	// メッセージループ
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		// メッセージがある
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 処理
			Time::Update();
			Sound::Update();
			Camera::Update();
			Input::Update(); // 入力処理の更新
			// どのシーンでもやめるボタンを押したら閉じる
			if (Input::IsButtonDown("quit") == true)
			{
				PostQuitMessage(0);
			}
			SceneManager::Update();
			ObjectManager::Update();

			Direct3D::BeginDraw();
			// 描画処理
			SceneManager::Draw();
			ObjectManager::Draw();

			Direct3D::EndDraw();
		}
	}

	Input::Release();
	ObjectManager::Release();
	Sound::Release();
	SceneManager::Relase();
	Direct3D::Release();

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0); // プログラム終了
		return 0;

	case WM_MOUSEMOVE: // マウスが動いた
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}