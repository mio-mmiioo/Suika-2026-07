#include "Input.h"
#include <map>
#include <vector>
#include <Xinput.h>

#pragma comment(lib, "Xinput.lib")

namespace Input
{
	// 入力するデバイスの種類
	enum INPUT_TYPE
	{
		KEYBOARD,		// キーボード
		GAMEPAD,		// ゲームパッド(Xbox)
		MOUSE,			// マウス
		MAX_INPUT_TYPE	// デバイスの最大数
	};

	struct InputInfo
	{
		INPUT_TYPE type;	// デバイスの種類
		int buttonID;		// ボタンの番号
	};

	using InputActionMap_t = std::map<std::string, std::vector<InputInfo>>;

	std::map<std::string, bool> currentInput;	// 現在の状態
	std::map<std::string, bool> previousInput;	// 前回の状態
	InputActionMap_t inputActionMap;

	const int KEY_MAX = 256; // キーボード入力の最大数
	
	// マウスの状態
	const int MOUSE_LEFT = 0;
	const int MOUSE_RIGHT = 1;
	const int MOUSE_MIDDLE = 2;

	LPDIRECTINPUT8 pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice;	// デバイスオブジェクト・キーボード
	BYTE keyState[KEY_MAX] = { 0 };		// キーボードの状態
	LPDIRECTINPUTDEVICE8 pMouseDevice;	// デバイスオブジェクト・マウス
	DIMOUSESTATE mouseState;			// マウスの状態
	POINT mousePosition;				// マウスカーソルの位置
	POINT prevMousePosition;			// 前回のマウスカーソルの位置
	XINPUT_STATE controllerState;		// コントローラーの状態

	/// <summary>
	/// 使いまわし可能なボタンを設定する
	/// </summary>
	void InitActionMap();

	/// <summary>
	/// 入力値をデッドゾーンを考慮しつつ、-1.0~1.0の値に変換する
	/// コントローラーで使用する
	/// </summary>
	/// <param name="raw">現在の値</param>
	/// <param name="max">入力の最大値</param>
	/// <param name="deadZone">デッドゾーンの大きさ</param>
	/// <returns>-1.0f~1.0fの値</returns>
	float GetAnalogValue(int raw, int max, int deadZone);

}

void Input::InitActionMap()
{
	// ここに入力の処理内容をかく
	inputActionMap["quit"] = { {INPUT_TYPE::KEYBOARD, DIK_ESCAPE} };
	inputActionMap["ok"] = { {INPUT_TYPE::KEYBOARD, DIK_SPACE}, {INPUT_TYPE::MOUSE, MOUSE_LEFT} };
	inputActionMap["next"] = { {INPUT_TYPE::KEYBOARD, DIK_N}, {INPUT_TYPE::MOUSE, MOUSE_RIGHT} };
	inputActionMap["right"] = { {INPUT_TYPE::KEYBOARD, DIK_RIGHT} };
	inputActionMap["left"] = { {INPUT_TYPE::KEYBOARD, DIK_LEFT} };
}

float Input::GetAnalogValue(int raw, int max, int deadZone)
{
	float ret = (float)raw;

	// デッドゾーン内なら値を０にして無視する
	if (ret > 0)
	{
		if (ret < deadZone)
		{
			ret = 0;
		}
		else
		{
			ret = (ret - deadZone) / (max - deadZone);
		}
	}
	else
	{
		if (ret > deadZone)
		{
			ret = 0;
		}
		else
		{
			ret = (ret + deadZone) / (max - deadZone);
		}
	}

	return ret;
}

void Input::Initialize(HWND hWnd)
{
	// DirectInputの本体を使用可能状態にする
	DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
	
	// キーボードの入力情報を取得可能状態にする
	pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
	pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

	// マウスの入力情報を取得可能状態にする
	pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, nullptr);
	pMouseDevice->SetDataFormat(&c_dfDIMouse);
	pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	InitActionMap(); // 使用するボタンをセットする

	// はじめはすべて押されていない状態にする これを書かないとpreviousInputに値が入らず、警告が出る
	for (const auto& mapInfo : inputActionMap)
	{
		currentInput[mapInfo.first] = false;
	}
}

void Input::Update()
{
	// それぞれの機種の状態を取得する
	pKeyDevice->Acquire();
	pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
	pMouseDevice->Acquire();
	pMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
	XInputGetState(0, &controllerState); // コントローラーは1つのみ使用する想定

	previousInput = currentInput;

	for (const auto& mapInfo : inputActionMap)
	{
		bool isDown = false;
		for (const auto& inputInfo : mapInfo.second)
		{
			// それぞれの状態のフラグを確認
			isDown = ((inputInfo.type == INPUT_TYPE::KEYBOARD) && (keyState[inputInfo.buttonID] & 0x80)) ||
				((inputInfo.type == INPUT_TYPE::MOUSE) && (mouseState.rgbButtons[inputInfo.buttonID] & 0x80)) ||
				((inputInfo.type == INPUT_TYPE::GAMEPAD) && (controllerState.Gamepad.wButtons & inputInfo.buttonID));

			if (isDown == true)
			{
				break;
			}
		}
		currentInput[mapInfo.first] = isDown;
	}
}

void Input::Release()
{
	pDInput->Release();
	pKeyDevice->Release();
}

bool Input::IsButtonDown(const std::string& action)
{
	auto it = currentInput.find(action);
	// actionが見つからない場合false
	if (it == currentInput.end())
	{
		return false;
	}
	auto prevIt = previousInput.find(action);
	// 押した瞬間 = 今は押していて、前回は押していない
	if (it->second == true && prevIt->second == false)
	{
		return true;
	}
	return false;
}

bool Input::IsButtonKeepDown(const std::string& action)
{
	auto it = currentInput.find(action);
	if (it == currentInput.end())
	{
		return false;
	}
	return it->second;
}

bool Input::IsButtonUp(const std::string& action)
{
	auto it = currentInput.find(action);
	if (it == currentInput.end())
	{
		return false;
	}
	auto prevIt = previousInput.find(action);
	// 離された瞬間 = 現在は離していて、前回は押している
	if (it->second == false && prevIt->second == true)
	{
		return true;
	}
	return false;
}

DirectX::XMFLOAT3 Input::GetMousePosition()
{
	return DirectX::XMFLOAT3((float)mousePosition.x, (float)mousePosition.y, 0.0f);
}

DirectX::XMFLOAT3 Input::GetPrevMousePosition()
{
	return DirectX::XMFLOAT3((float)prevMousePosition.x, (float)prevMousePosition.y, 0.0f);
}

void Input::SetMousePosition(int x, int y)
{
	prevMousePosition = mousePosition;
	mousePosition.x = (float)x;
	mousePosition.y = (float)y;
}

DirectX::XMFLOAT3 Input::GetMouseMove()
{
	return DirectX::XMFLOAT3((float)mouseState.lX, (float)mouseState.lY, (float)mouseState.lZ);
}

DirectX::XMFLOAT3 Input::GetPadStickLeft()
{
	float x = GetAnalogValue(controllerState.Gamepad.sThumbLX, 32767, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	float y = GetAnalogValue(controllerState.Gamepad.sThumbLY, 32767, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	return DirectX::XMFLOAT3(x, y, 0);
}

DirectX::XMFLOAT3 Input::GetPadStickRight()
{
	float x = GetAnalogValue(controllerState.Gamepad.sThumbRX, 32767, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	float y = GetAnalogValue(controllerState.Gamepad.sThumbRY, 32767, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	return DirectX::XMFLOAT3(x, y, 0);
}

float Input::GetPadTrrigerLeft()
{
	return GetAnalogValue(controllerState.Gamepad.bLeftTrigger, 255, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

float Input::GetPadTrrigerRight()
{
	return GetAnalogValue(controllerState.Gamepad.bRightTrigger, 255, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

void Input::SetPadVibration(int left, int right)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = left;
	vibration.wRightMotorSpeed = right;
	XInputSetState(0, &vibration);
}
