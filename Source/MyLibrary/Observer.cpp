#include "Observer.h"

namespace Observer
{
	int score; // 得点
	bool isPrevGameCreated; // すでに前回のフルーツを作成しているならtrue
	bool isNewGame; // 新しくゲームを始めるならtrue
}

void Observer::Init()
{
	score = 0;
	isNewGame = true;
}

void Observer::SetScore(int s)
{
	score = s;
}

void Observer::AddScore(int addScore)
{
	score += addScore;
}

int Observer::GetScore()
{
	return score;
}

void Observer::SetIsNewGame(bool flag)
{
	isNewGame = flag;
}

bool Observer::GetIsNewGame()
{
	return isNewGame;
}

void Observer::SetIsPrevGameCreated(bool flag)
{
	isPrevGameCreated = flag;
}

bool Observer::GetIsPrevGameCreated()
{
	return isPrevGameCreated;
}
