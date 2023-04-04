#include <string>
#include <SFML\Graphics.hpp>
#include "Constants1.h"
#include "CollisionMap.h"
#include "Bot.h"


Bot::Bot(int id, std::string& name, int colorIndex)
	: SCurve(id, name, colorIndex),
	m_nRequiredRayLen(Constants::CURVE_BOT_MAX_RAY_LEN)
{
}


Bot::~Bot()
{
}


AbsoluteDirection Bot::ChooseDirection()
{
	std::vector<AbsoluteDirection> bestDirections;
	AbsoluteDirection bestDirection = AbsoluteDirection::Straight;
	MinMax maxLen;

	for (int i = 0; i < 3; ++i)
	{
		if (futurePaths[i].realLength > maxLen)
		{
			bestDirections.clear();
			bestDirections.push_back(AbsoluteDirection(i - 1));
			//bestDirection = AbsoluteDirection(i - 1);
			maxLen = futurePaths[i].realLength;

		}
		else if (futurePaths[i].realLength == maxLen)
		{
			bestDirections.push_back(AbsoluteDirection(i - 1));
			//bestDirection = AbsoluteDirection(i - 1);
		}
	}
//}
	for (auto& x : bestDirections)
	{
		if (x == m_nMovingTo)
			return x;
	}

	int randomBest = std::rand() % bestDirections.size();
	return bestDirections[randomBest];
	//return bestDirections.at(int(bestDirections.size()) - 1);
}

void Bot::step()
{
	AbsoluteDirection newDirection = ChooseDirection();
	if (newDirection != m_nMovingTo)
	{
		stepsTillNewDirection = Constants::CURVE_BOT_MAX_RAY_LEN;
	}

	setMovementDirection(ChooseDirection());
	SCurve::step();
}


	/*
	using namespace Constants;
	int rightRay, leftRay, straightRay;

	straightRay = GetRayDistance(getPosition().x, getPosition().y, getAngle(), AbsoluteDirection::Straight);

	if (straightRay > m_nCurrMaxRayLen)
	{
		m_nCurrMaxRayLen = CURVE_BOT_MIN_RAY_LEN;//(m_nCurrMaxRayLen + CURVE_BOT_MIN_RAY_LEN) / 2;
		return AbsoluteDirection::Straight;
	}

	m_nCurrMaxRayLen = (m_nCurrMaxRayLen * 5) / 10 +  5 * CURVE_BOT_MAX_RAY_LEN / 10;

	rightRay = GetRayDistance(getPosition().x, getPosition().y, getAngle(), AbsoluteDirection::Right);
	leftRay = GetRayDistance(getPosition().x, getPosition().y, getAngle(), AbsoluteDirection::Left);

	if (straightRay >= leftRay && straightRay >= rightRay)
	{
		return AbsoluteDirection::Straight;
	}
	else if (rightRay > leftRay)
	{
		return AbsoluteDirection::Right;
	}
	else
	{
		return AbsoluteDirection::Left;
	}
	
	
	int rightDist, leftDist, sumDist;
	int tempAngle;
	double tempX, tempY;

	rightDist = GetRayDistance(getPosition().x, getPosition().y, getAngle(), AbsoluteDirection::Right);
	leftDist = GetRayDistance(getPosition().x, getPosition().y, getAngle(), AbsoluteDirection::Left);
	sumDist = rightDist + leftDist;

	int maxRayDest(0), currRayDest(0);
	AbsoluteDirection bestDir = AbsoluteDirection::Straight;

	tempX = getPosition().x;
	tempY = getPosition().y;
	tempAngle = getAngle();

#pragma region right calcs
	for (int i = 0; i < rightDist; ++i)
	{
		if (i % (sumDist / Constants::CURVE_BOT_RAYS) == 0)
		{
			currRayDest = i + GetRayDistance(tempX, tempY, tempAngle, AbsoluteDirection::Straight);

			if (currRayDest > maxRayDest)
			{
				maxRayDest = currRayDest;
				bestDir = AbsoluteDirection::Right;
			}
		}

		sf::Vector2f newPos = GetNextStep(tempX, tempY, tempAngle);

		tempX = newPos.x;
		tempY = newPos.y;
		tempAngle += Constants::CURVE_ANGULAR_SPEED * (int)AbsoluteDirection::Left;
	}
#pragma endregion
#pragma region left calcs
	tempX = getPosition().x;
	tempY = getPosition().y;
	tempAngle = getAngle();

	for (int i = 0; i < leftDist; ++i)
	{
		if (i % (sumDist / Constants::CURVE_BOT_RAYS) == 0)
		{
			currRayDest = i + GetRayDistance(tempX, tempY, tempAngle, AbsoluteDirection::Straight);

			if (currRayDest > maxRayDest)
			{
				maxRayDest = currRayDest;
				bestDir = AbsoluteDirection::Left;
			}
		}

		sf::Vector2f newPos = GetNextStep(tempX, tempY, tempAngle);

		tempX = newPos.x;
		tempY = newPos.y;
		tempAngle += Constants::CURVE_ANGULAR_SPEED * (int)AbsoluteDirection::Left;
	}
#pragma endregion

	return bestDir;
	
}*/

