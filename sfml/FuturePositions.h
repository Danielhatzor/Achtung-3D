#pragma once
#include <array>
#include "MinMax.h"

class FuturePositions
{
public:
	bool hasNext;
	MinMax length;
	std::array<FuturePositions*, 3> futurePositions;

	FuturePositions();
	~FuturePositions();
};

