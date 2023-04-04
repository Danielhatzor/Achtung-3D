#pragma once

#include <vector>
#include <array>
#include <SFML\Graphics.hpp>
#include "Constants1.h"

struct CurvePosition
{
	sf::Vector3f position;
	std::vector<sf::Vector3f> collisionPoints;
};
class Patterns
{
public:
	static void GeneratePatterns();
	static const std::vector<CurvePosition>& GetPath(AbsoluteDirection pathDirection);

private:
	static std::array<std::vector<CurvePosition>, 3> curvePaths;

	static std::vector<sf::Vector3f> deleteDuplicateVectors(std::vector<sf::Vector3f> positions);
};