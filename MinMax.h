#pragma once

class MinMax
{
public:
	int min;
	int max;
	
	bool operator>(MinMax &other);
	bool operator<(MinMax &other);
	bool operator==(MinMax &other);
	void operator=(MinMax& other);

	MinMax();
	MinMax::MinMax(int _min, int _max);
	~MinMax();
};

