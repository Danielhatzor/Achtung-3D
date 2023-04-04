#include <math.h>
#include <vector>
#include <SFML\Graphics.hpp>
#include "Patterns.h"
#include "MyMat.h"
#include "SCurve.h"
#include "CollisionMap.h"
#include "Constants1.h"

/*std::vector<sf::Vector3f> Patterns::LeftTrajectory = std::vector<sf::Vector3f>();
std::vector<sf::Vector3f> Patterns::RightTrajectory = std::vector<sf::Vector3f>();
std::vector<sf::Vector3f> Patterns::StraightTrajectory = std::vector<sf::Vector3f>();

void Patterns::GeneratePatterns()
{
	InitRightTrajectory();
	InitLeftTrajectory();
	InitStraightTrajectory();
}

void Patterns::InitRightTrajectory()
{
	sf::Vector3f step(0, 0, Constants::SPHERE_RADIUS);
	step = MyMat::RotateY(step, atan2(-Constants::CURVE_ROTATING_RADIUS, Constants::SPHERE_RADIUS));

	sf::Vector3f offset = MyMat::RotateZ(step, -90 * Constants::DEG_TO_RADS);
	MyMat mat(offset);

	int steps = 360 / Constants::CURVE_ANGULAR_SPEED;
	RightTrajectory.resize(steps);

	for (int i = 0; i < steps; ++i)
	{
		sf::Vector3f rightStep = MyMat::RotateZ(step, Constants::CURVE_ANGULAR_SPEED * Constants::DEG_TO_RADS * (i + 1));

		RightTrajectory[i] = mat.RotateToSeed(rightStep);
	}
}

void Patterns::InitLeftTrajectory()
{
	sf::Vector3f step(0, 0, Constants::SPHERE_RADIUS);
	step = MyMat::RotateY(step, atan2(-Constants::CURVE_ROTATING_RADIUS, Constants::SPHERE_RADIUS));

	sf::Vector3f offset = MyMat::RotateZ(step, 90 * Constants::DEG_TO_RADS); ///////
	MyMat mat(offset);

	int steps = 360 / Constants::CURVE_ANGULAR_SPEED;
	LeftTrajectory.resize(steps);

	for (int i = 0; i < steps; ++i)
	{
		sf::Vector3f leftStep = MyMat::RotateZ(step, -Constants::CURVE_ANGULAR_SPEED * Constants::DEG_TO_RADS * (i + 1)); //////////////

		LeftTrajectory[i] = mat.RotateToSeed(leftStep);
	}
}

void Patterns::InitStraightTrajectory()
{
	auto& hdsf = StraightTrajectory;
	double angularSpeed = 360 / (Constants::SPHERE_RADIUS * 2 * Constants::PI);

	sf::Vector3f step(0, 0, Constants::SPHERE_RADIUS);
	step = MyMat::RotateY(step, -90 * Constants::DEG_TO_RADS);//atan2(-Constants::SPHERE_RADIUS, Constants::SPHERE_RADIUS));

	sf::Vector3f offset = MyMat::RotateZ(step, -90 * Constants::DEG_TO_RADS);
	MyMat mat(offset);

	int steps = 360 / angularSpeed;
	StraightTrajectory.resize(steps);

	for (int i = 0; i < steps; ++i)
	{
		sf::Vector3f rightStep = MyMat::RotateZ(step, angularSpeed * Constants::DEG_TO_RADS * (i + 1));

		StraightTrajectory[i] = mat.RotateToSeed(rightStep);
	}
}*/

std::array<std::vector<CurvePosition>, 3> Patterns::curvePaths;

std::vector<sf::Vector3f> Patterns::deleteDuplicateVectors(std::vector<sf::Vector3f> positions)
{
	std::vector<sf::Vector3f> meagerPositions;

	for (int i = 0; i < positions.size(); ++i)
	{
		bool duplicate = false;

		for (int j = i + 1; j < positions.size(); ++j)
		{
			if (abs(positions[i].x - positions[j].x) < 0.25 &&
				abs(positions[i].y - positions[j].y) < 0.25 &&
				abs(positions[i].z - positions[j].z) < 0.25)
			{
				duplicate = true;
				break;
			}
		}

		if (!duplicate)
		{
			meagerPositions.push_back(positions[i]);
		}
	}

	return meagerPositions;
}

void Patterns::GeneratePatterns()
{
	for (int i = 0; i < 3; ++i)
	{
		sf::Vector3f position(0, 0, Constants::SPHERE_RADIUS);
		sf::Vector3f rotationAxis(0, Constants::SPHERE_RADIUS, 0);

		for (int j = 0; j < Constants::CURVE_BOT_MAX_RAY_LEN; ++j)
		{
			rotationAxis = SCurve::changeRotAxis(position, rotationAxis, AbsoluteDirection(i - 1));
			position = SCurve::getNextStep(position, rotationAxis);

			std::vector<sf::Vector3f> collisionPoints;
			CollisionMap::GetCollisionPoints(collisionPoints, position, rotationAxis);
			collisionPoints = deleteDuplicateVectors(collisionPoints);
			
			int size = curvePaths[i].size();
			curvePaths[i].resize(size + 1);
			curvePaths[i][size].collisionPoints = collisionPoints;
			curvePaths[i][size].position = position;
		}
			/*// Changing rotation axis
			MyMat mat(position);
			sf::Vector3f propRotAxis = mat.UnRotateFromSeed(rotationAxis);
			propRotAxis = MyMat::RotateZ(propRotAxis, (i - 1) * Constants::CURVE_ANGULAR_SPEED * Constants::DEG_TO_RADS);
			rotationAxis = mat.RotateToSeed(propRotAxis);

			// Changing position
			mat.SeedVector(rotationAxis);
			sf::Vector3f propPosition = mat.UnRotateFromSeed(position);
			propPosition = MyMat::RotateZ(propPosition, (i - 1) * Constants::CURVE_ANGULAR_SPEED * Constants::DEG_TO_RADS);
			position = mat.RotateToSeed(propPosition);

			// Addingposition to path
			curvePaths[i].push_back(position);*/
		//}
	}
}

const std::vector<CurvePosition>& Patterns::GetPath(AbsoluteDirection pathDirection)
{
	return curvePaths[int(pathDirection) + 1];
}