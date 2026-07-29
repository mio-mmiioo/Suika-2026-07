#include "Time.h"


namespace Time
{
	LARGE_INTEGER freq;
	LARGE_INTEGER current;
	float deltaTime;
	float refreshRate;
	const int BUF_SIZE = 30;
	float timeBuf[BUF_SIZE];
	int readP;
	int writeP;
}

void Time::Initialize(HWND hWnd)
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&current);

	HDC hdc = GetDC(hWnd);
	refreshRate = (float)GetDeviceCaps(GetDC(hWnd), VREFRESH); // リフレッシュレートの取得
	ReleaseDC(hWnd, hdc);

	readP = 0;
	writeP = 0;
}

void Time::Update()
{
	LARGE_INTEGER last = current;
	QueryPerformanceCounter(&current);
	float dt = static_cast<float>(current.QuadPart - last.QuadPart) / freq.QuadPart;
	timeBuf[writeP] = dt;
	writeP = (writeP + 1) % BUF_SIZE;
	if (writeP == readP)
	{
		readP = (readP + 1) % BUF_SIZE;
	}
	float sum = 0;
	int num = 0;
	for (int i = readP; i != writeP; i = (i + 1) % BUF_SIZE)
	{
		sum += timeBuf[i];
		num += 1;
	}
	if (num > BUF_SIZE / 2)
	{
		deltaTime = sum / num;
	}
	else
	{
		deltaTime = dt;
	}
}

float Time::GetDeltaTime()
{
	return deltaTime;
}
