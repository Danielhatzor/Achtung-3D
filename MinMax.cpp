#include "MinMax.h"


MinMax::MinMax()
{
	max = 0;
	min = 0;
}
MinMax::MinMax(int _min, int _max)
{
	this->max = _max;
	this->min = _min;
}


MinMax::~MinMax()
{
}

bool MinMax::operator>(MinMax &other)
{
	return max*max*min > other.max * other.max * other.min;
	/*if (max > other.max)
		return true;
	else if (max < other.max)
		return false;
	else if (min > other.min)
		return true;
	return false;*/
}

bool MinMax::operator<(MinMax &other)
{
	return max*max*min < other.max * other.max * other.min;
	/*if (max < other.max)
		return true;
	/*else if (max > other.max)
		return false;
	else if (min < other.min)
		return true;
	return false;*/
}

bool MinMax::operator==(MinMax &other)
{
	return max*max*min == other.max * other.max * other.min;
	//return min == other.min && max == other.max;
	//return max == other.max;
}
void MinMax::operator=(MinMax& other)
{
	max = other.max;
	min = other.min;
}