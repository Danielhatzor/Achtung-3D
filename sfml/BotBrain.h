#pragma once

#include <vector>
#include "SCurve.h"
#include "Bot.h"
#include "SCurveCollection.h"
#include "MinMax.h"

struct CurveCollision
{
	int CollDist;
	int EnemyCollDist;
	sf::Vector3f CollPoint;
};

class BotBrain
{
public:
	//static CurvePath* tempCurvePath;

	BotBrain(SCurveCollection& collection);
	~BotBrain();

	void CalcPaths();
	int calcPath(sf::Vector3f& pos, sf::Vector3f& rotAxis, AbsoluteDirection direction, int steps, const CollisionMap &collisionMap);
	//MinMax BotBrain::CalcPaths2(sf::Vector3f pos, sf::Vector3f rotAxis, AbsoluteDirection direction, int steps, int depth, const CollisionMap &collisionMap, CurvePath* curvePath);
	MinMax BotBrain::CalcPaths2(sf::Vector3f pos, sf::Vector3f rotAxis, AbsoluteDirection direction, int steps, int depth, const CollisionMap &collisionMap);

	void Run();
	CurveCollision CalcCurveCollisions(Bot& bot, AbsoluteDirection botDir, SCurve& curve, AbsoluteDirection curveDir);

	//void ChooseDirection(Bot& bot);

private:
	SCurveCollection& m_cCollection;
	
};

