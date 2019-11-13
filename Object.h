#pragma once
#include "GameEngine.h"

class Object
{
public:
	Object();
	~Object();

	float x;//tọa độ x
	float y;//tọa độ y
	float vx;//véc tơ vận tốc x
	float vy;//véc tơ vận tốc y
	int width;//chiều rộng
	int height;//chiều dài

};

