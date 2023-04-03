#include <SFML\Graphics.hpp>
#include "Patterns.h"
#include "MyMat.h"
#include "BotBrain.h"
#include "Constants1.h"
#include <assert.h>

BotBrain::BotBrain(SCurveCollection& collection)
	: m_cCollection(collection)
{
}


BotBrain::~BotBrain()
{
}


int BotBrain::calcPath(sf::Vector3f& pos, sf::Vector3f& rotAxis, AbsoluteDirection direction, int steps, const CollisionMap &collisionMap)
{
	sf::Vector3f futureRotAxis = rotAxis;
	sf::Vector3f futurePos = pos;

	/*if (direction == AbsoluteDirection::Straight)
	{
		steps = steps - Constants::CURVE_HEAD_RADIUS - 1;
	}*/

	for (int j = 0; j < steps; ++j)
	{
		futureRotAxis = SCurve::changeRotAxis(futurePos, futureRotAxis, direction);
		futurePos = SCurve::getNextStep(futurePos, futureRotAxis);

		if (collisionMap.IsColliding(futurePos, futureRotAxis))
		{
			return j;
		}

		pos.x = futurePos.x;
		pos.y = futurePos.y;
		pos.z = futurePos.z;
		rotAxis.x = futureRotAxis.x;
		rotAxis.y = futureRotAxis.y;
		rotAxis.z = futureRotAxis.z;


	}

	return steps;
}

/*
MinMax BotBrain::CalcPaths2(sf::Vector3f pos, sf::Vector3f rotAxis, AbsoluteDirection direction, int steps, int depth, const CollisionMap &collisionMap, CurvePath* curvePath)
{
	MinMax minmax, best, branch;
	AbsoluteDirection bestDirection = AbsoluteDirection::Straight;
	int check;

	if (depth <= 0 || steps <= 0)
	{
		curvePath->hasNext = false;
		curvePath->realLength = MinMax(0,0);
		return minmax;
	}
	
	//if (curvePath == nullptr)
	//{
	//	//wtf
	//	curvePath = new CurvePath();
	//}
	assert(curvePath != nullptr);

	check = calcPath(pos, rotAxis, direction, std::min(steps, depth), collisionMap);
	depth -= check;

	if (check == steps)
	{
		if (curvePath->next == nullptr)
		{
			curvePath->next = new CurvePath();
		}

		for (int i = 0; i < 3; ++i)
		{
			branch = CalcPaths2(pos, rotAxis, AbsoluteDirection(i - 1), Constants::CURVE_BOT_MAX_RAY_LEN, depth, collisionMap, curvePath->next);
			//branch = CalcPaths2(pos, rotAxis, AbsoluteDirection(i - 1), Constants::CURVE_BOT_MAX_RAY_LEN, depth, collisionMap, tempCurvePath->next);
			if (best < branch)
			{
				best = branch;
				bestDirection = AbsoluteDirection(i - 1);
			}
			if (minmax.max < branch.max)
			{
				minmax.max = branch.max;
			}
			if ((minmax.min == 0 || minmax.min > branch.min) && branch.min != 0)
			{
				minmax.min = branch.min;
			}
		}
	}
	
	curvePath->hasNext = (minmax.max > 0);

	minmax.max += check;
	minmax.min += check;

	curvePath->realLength = minmax;
	curvePath->direction = bestDirection;

	return minmax;
}
*/

MinMax BotBrain::CalcPaths2(sf::Vector3f pos, sf::Vector3f rotAxis, AbsoluteDirection direction, int steps, int depth, const CollisionMap &collisionMap)
{
	MinMax minmax, best, branch;
	AbsoluteDirection bestDirection = AbsoluteDirection::Straight;
	int check;

	if (depth <= 0 || steps <= 0)
	{
		return minmax;
	}

	check = calcPath(pos, rotAxis, direction, std::min(steps, depth), collisionMap);
	depth -= check;

	if (check == steps)
	{
		for (int i = 0; i < 3; ++i)
		{
			branch = CalcPaths2(pos, rotAxis, AbsoluteDirection(i - 1), Constants::CURVE_BOT_MAX_RAY_LEN, depth, collisionMap);
			//branch = CalcPaths2(pos, rotAxis, AbsoluteDirection(i - 1), Constants::CURVE_BOT_MAX_RAY_LEN, depth, collisionMap, tempCurvePath->next);
			if (best < branch)
			{
				best = branch;
				bestDirection = AbsoluteDirection(i - 1);
			}
			if (minmax.max < branch.max)
			{
				minmax.max = branch.max;
			}
			if ((minmax.min == 0 || minmax.min > branch.min) && branch.min != 0)
			{
				minmax.min = branch.min;
			}
		}
	}

	minmax.max += check;
	minmax.min += check;

	return minmax;
}


void BotBrain::CalcPaths()
{
	auto& curves = m_cCollection.GetCurves();
	auto& collisionMap = m_cCollection.GetCollisionMap();

	for (SCurve* curve : curves)
	{
		if (curve->isAlive())
		{
			sf::Vector3f futurePos = curve->getPosition();
			sf::Vector3f futureRotAxis = curve->getRotationAxis();
			//MinMax a = CalcPaths2(futurePos, futureRotAxis, curve->m_nMovingTo, curve->stepsTillNewDirection, Constants::CURVE_BOT_MAX_RAY_DEPTH, collisionMap, (curve->futurePaths)[int(curve->m_nMovingTo) + 1].next);
			//MinMax a = CalcPaths2(futurePos, futureRotAxis, curve->m_nMovingTo, curve->stepsTillNewDirection, Constants::CURVE_BOT_MAX_RAY_DEPTH, collisionMap, &(curve->persistantPath));
			MinMax a = CalcPaths2(futurePos, futureRotAxis, curve->m_nMovingTo, curve->stepsTillNewDirection, Constants::CURVE_BOT_MAX_RAY_DEPTH, collisionMap);


			for (int i = 0; i < 3; ++i)
			{
				futurePos = curve->getPosition();
				futureRotAxis = curve->getRotationAxis();

				//MinMax j = CalcPaths2(futurePos, futureRotAxis, AbsoluteDirection(i - 1), Constants::CURVE_BOT_MAX_RAY_LEN, Constants::CURVE_BOT_MAX_RAY_DEPTH, collisionMap, &((curve->futurePaths)[i]));
				MinMax j = CalcPaths2(futurePos, futureRotAxis, AbsoluteDirection(i - 1), Constants::CURVE_BOT_MAX_RAY_LEN, Constants::CURVE_BOT_MAX_RAY_DEPTH, collisionMap);


				if (i == int(curve->m_nMovingTo) + 1)
				{
					if (j > a)
					{
						curve->stepsTillNewDirection = std::min(j.max, Constants::CURVE_BOT_MAX_RAY_LEN);
					}
					else
					{
						j = a;
					}
				}
				curve->futurePaths[i].realLength = j;
			}
		}
	}
		/*MyMat mat(curve->getPosition());
		sf::Vector3f propRotAxis = mat.UnRotateFromSeed(curve->getRotationAxis());
		float rotationAngle = atan2(propRotAxis.x, propRotAxis.y);
		float cosAngle = cos(rotationAngle);
		float sinAngle = sin(rotationAngle);*/

		/*for (int i = 0; i < 3; ++i)
		{
			auto& pattern = Patterns::GetPath(AbsoluteDirection(i - 1));

			int length = Constants::CURVE_BOT_MAX_RAY_LEN;

			for (int j = 0; j < pattern.size(); ++j)
			{
				const CurvePosition& position = pattern[j];
				bool collides = false;

				for (sf::Vector3f collision : position.collisionPoints)
				{
					collision = MyMat::RotateZ(collision, sinAngle, cosAngle);
					collision = mat.RotateToSeed(collision);


					sf::CircleShape circle(4);
					circle.setFillColor(sf::Color::Red);
					circle.setOrigin(2, 2);
					circle.setPosition(collision.x + Constants::GAME_LENGTH / 2, collision.y + Constants::GAME_HIGHT / 2);
					window1.draw(circle);

					if (collisionMap.At(collision))
					{
						collides = true;

						break;
					}
				}

				if (collides)
				{
					length = j;

					break;
				}
			}

			window1.display();
			curve->futurePaths[i].realLength = length;
		}*/

	/*for (SCurve* curve : curves)
	{
		for (SCurve* enemy : curves)
		{
			if (curve != enemy)
			{

			}
		}
	}*/
}

CurveCollision BotBrain::CalcCurveCollisions(Bot& bot, AbsoluteDirection botDir, SCurve& curve, AbsoluteDirection curveDir)
{
	MyMat mat(bot.getRotationAxis());
	sf::Vector3f proportionalVec = mat.UnRotateFromSeed(curve.getRotationAxis());
	float angleToYZ = atan2(proportionalVec.x, proportionalVec.y);

	// Rotating about z to yz plane
	proportionalVec = MyMat::RotateZ(proportionalVec, angleToYZ);

	/*// Getting angle between vectors
	float angleBetweenVectors = atan2(proportionalVec.y, proportionalVec.z);
	*/

	sf::Vector3f collision(Constants::SPHERE_RADIUS, 0, 0);
	collision = MyMat::RotateZ(collision, -angleToYZ);
	collision = mat.RotateToSeed(collision);

	return(CurveCollision());
}

void BotBrain::Run()
{
	while (true)
	{
		CalcPaths();
	}
}

/**
void BotBrain::ChooseDirection(Bot& bot)
{
	AbsoluteDirection bestDirection = AbsoluteDirection::Straight;
	MinMax maxLen;
	std::vector<AbsoluteDirection> bestDirections;

	for (int i = 0; i < 3; ++i)
	{
		if (bot.futurePath.futurePaths[i].realLength > maxLen)
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

	for (auto& x : bestDirections)
	{
		if (x == m_nMovingTo)
			return x;
	}

	int randomBest = std::rand() % bestDirections.size();
	return bestDirections[randomBest];


	//AbsoluteDirection newDirection = ChooseDirection();
	//if (newDirection != m_nMovingTo)
	//{
	//	stepsTillNewDirection = Constants::CURVE_BOT_MAX_RAY_LEN;
	//}
	//
	setMovementDirection(ChooseDirection());
}
*/