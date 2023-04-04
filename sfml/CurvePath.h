#pragma once
#include "Constants1.h"

class CurvePath
{
public:
	MinMax realLength;
	AbsoluteDirection direction;
	CurvePath* next;
	//std::vector<CurvePath> futurePaths;
	bool hasNext;

	//int lenToPotentialCollison;
	//int enemyLenToCollision;

	CurvePath();
	~CurvePath();

	void operator=(CurvePath& other);
};

