#include "ButtonArea.h"
#include "../../Engine/Input.h"
#include "../../Engine/Data/Image.h"

bool ButtonArea::IsMouseKeepInArea(Area area)
{
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;

	if (IsMouseInArea(area, x, y) == true)
	{
		return true;
	}
	return false;
}

bool ButtonArea::IsMouseInArea(Area area)
{
	int prevX = (int)Input::GetPrevMousePosition().x;
	int prevY = (int)Input::GetPrevMousePosition().y;
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;
	if (IsMouseInArea(area, prevX, prevY) == false && IsMouseInArea(area, x, y) == true)
	{
		return true;
	}
	return false;
}

bool ButtonArea::IsMouseInArea(Area area, int mouseX, int mouseY)
{
	if (area.leftTopX < mouseX && mouseX < area.rightDownX)
	{
		if (area.leftTopY < mouseY && mouseY < area.rightDownY)
		{
			return true;
		}
	}
	return false;
}

void ButtonArea::DrawArea(Area area)
{
	int x = (int)area.leftTopX;
	int y = (int)area.leftTopY;
	int width = (int)(area.rightDownX - area.leftTopX);
	int height = (int)(area.rightDownY - area.leftTopY);
	Image::DrawExtendGraph(x, y, width, height, area.hImage);
}

Button::Button(Area normal, Area select)
{
	normal_ = normal;
	select_ = select;
	isPushArea_ = false;
	isOnArea_ = false;
}

Button::~Button()
{
}

void Button::Update()
{
	isOnArea_ = false;

	if (ButtonArea::IsMouseKeepInArea(normal_) == true)
	{
		isOnArea_ = true;
	}
	if (Input::IsButtonDown("ok"))
	{
		if (isOnArea_ == true)
		{
			isPushArea_ = !isPushArea_;
		}
	}
}

void Button::Draw() const
{
	if (isOnArea_ == true)
	{
		ButtonArea::DrawArea(select_);
	}
	else
	{
		ButtonArea::DrawArea(normal_);
	}
}

bool Button::GetIsOnArea() const
{
	if (ButtonArea::IsMouseInArea(normal_) == true)
	{
		return true;
	}
	return false;
}
