#include "CurvePath.h"


CurvePath::CurvePath() :	direction(AbsoluteDirection::Straight),
							next(nullptr),
							hasNext(false)
{
}

CurvePath::~CurvePath()
{
	if (next != nullptr)
		delete next;
}

void CurvePath::operator=(CurvePath& other)
{
	direction = other.direction;

	// ---------------------

	if (other.next != nullptr && other.hasNext)
	{
		if (next == nullptr)
		{
			next = new CurvePath();
		}

		*next = *other.next;
		hasNext = true;
	}
	else
	{
		hasNext = false;
	}

	// ---------------------

	/*if (other.futurePaths.size() > 0 && other.hasNext)
	{
		if (futurePaths.size() == 0)
		{
			futurePaths.resize(3);
		}

		for (int i = 0; i < 3; i++)
		{
			futurePaths[i] = other.futurePaths[i];
		}

		hasNext = true;
	}
	else
	{
		hasNext = false;
	}
	*/
}