#pragma once

struct Point
{
	double x;
	double y;

	Point() { x = 0.0; y = 0.0; }

	Point(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	void operator *= (const Point& pos2)
	{
		this->x *= pos2.x;
		this->y *= pos2.y;
	}

	void operator += (const Point& pos2)
	{
		this->x += pos2.x;
		this->y += pos2.y;
	}

	void operator -= (const Point& pos2)
	{
		this->x -= pos2.x;
		this->y -= pos2.y;
	}
	
	void SetPosition(const Point& pos2)
	{
		this->x = pos2.x;
		this->y = pos2.y;
	}
	void SetPosition(const int x, const int y)
	{
		this->x = x;
		this->y = y;
	}
};