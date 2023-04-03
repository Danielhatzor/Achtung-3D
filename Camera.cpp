#include <cassert>
#include <array>
#include <thread>
#include <future>
#include <math.h>
#include <SFML\Graphics.hpp>
#include "Constants1.h"
#include "Camera.h"
#include "Sphere.h"

Material Camera::sphereMaterial
{
	{ 0.05f, 0.3f, 0.3f },
	{ 0.05f, 0.3f, 0.3f },
	{ 0.1f, 0.6f, 0.3f }, 64
};

Camera::Camera(sf::RenderTarget& target, std::vector<ClientCurve*>& curves, sf::Vector3f follow, sf::Vector3f rotAxis)
	: m_rtTarget(target),
	m_aCurves(curves),
	m_mMat(follow),
	m_v3fRotAxis(rotAxis),
	m_v3fPosition(follow),
	m_fOffset(0),
	m_fTargetOffset(0),
	m_colLightColor(255, 255, 255)
{
	using namespace Constants;

	for (auto& star : m_aStars)
	{
		star.sides = rand() % 3 + 3;

		// Generating a random vector;
		float x = rand() % (SPHERE_RADIUS * 2) - SPHERE_RADIUS;
		float y = rand() % (SPHERE_RADIUS * 2) - SPHERE_RADIUS;
		float z = rand() % (SPHERE_RADIUS * 2) - SPHERE_RADIUS;

		float wantedLength = rand() % (MAX_GALAXY_RADIUS / 2) + MAX_GALAXY_RADIUS / 2;
		//wantedLength *= abs(wantedLength);
		float length = sqrt(x * x + y * y + z * z);
		float scale = wantedLength / length;

		star.position = sf::Vector3f(x * scale, y * scale, z * scale);
	}
}

Camera::~Camera()
{
}

void Camera::DrawCurve(ClientCurve& curve)
{
	using namespace Constants;

	static float sqrtSphereRad = sqrt(SPHERE_RADIUS);
	float curveRad;
	double sinAngle = sin(m_fRotationAngle);
	double cosAngle = cos(m_fRotationAngle);

	sf::CircleShape circle;

#pragma region curve head
	sf::Vector3f pos = curve.getPosition();
	sf::Vector3f normal;

	pos = m_mMat.UnRotateFromSeed(pos);

	if (pos.z > 0)
	{
		pos = MyMat::RotateZ(pos, sinAngle, cosAngle);
		curveRad = SCALE * CURVE_HEAD_RADIUS * sqrt(pos.z) / sqrtSphereRad;
		circle.setRadius(curveRad);
		circle.setOrigin(curveRad, curveRad);
		circle.setPosition(SCALE * pos.x + GAME_LENGTH / 2, SCALE * pos.y + GAME_HIGHT / 2 + m_fOffset);
	
		normal = sf::Vector3f(pos.x / SPHERE_RADIUS, pos.y / SPHERE_RADIUS, pos.z / SPHERE_RADIUS);
		circle.setFillColor(CURVE_MATERIALS[curve.getColorIndex()].getColor(normal, m_v3fLight, m_colLightColor));
		
		m_rtTarget.draw(circle);
	}
#pragma endregion

	//m_sSphere.update(Constants::CURVE_MATERIALS[curve.getColorIndex()], curve.getPosition());

#pragma region curve path
	std::vector<std::vector<sf::Vector3f>>& borders = curve.m_aav3PathBounders;
	sf::Color mycolor;
	sf::Vector3f mynormal;

	for (auto& arr : borders)
	{
		sf::VertexArray curvef(sf::TrianglesStrip);
		for (auto position : arr)
		{
			position = m_mMat.UnRotateFromSeed(position);

			if (position.z > 0)
			{
				position = MyMat::RotateZ(position, sinAngle, cosAngle);
				normal = sf::Vector3f(position.x / SPHERE_RADIUS, position.y / SPHERE_RADIUS, position.z / SPHERE_RADIUS);
				mycolor = CURVE_MATERIALS[curve.getColorIndex()].getColor(normal, m_v3fLight, m_colLightColor);
				curvef.append(sf::Vertex(sf::Vector2f(SCALE * position.x + GAME_LENGTH / 2, SCALE * position.y + GAME_HIGHT / 2 + m_fOffset), mycolor));
			}
			else if (curvef.getVertexCount() > 0)
			{
				m_rtTarget.draw(curvef);
				curvef.clear();
			}
		}

		if (curvef.getVertexCount() > 0)
		{
			m_rtTarget.draw(curvef);
			curvef.clear();
		}
	}


	/*
	std::vector<sf::Vector3f>& positions = curve.getTrajectory();

	for (auto position : positions)
	{
		position = m_mMat.UnRotateFromSeed(position);

		if (position.z > 0)
		{
			position = MyMat::RotateZ(position, sinAngle, cosAngle);

			curveRad = SCALE * CURVE_HEAD_RADIUS * sqrt(position.z) / sqrtSphereRad;
			circle.setRadius(curveRad);
			circle.setOrigin(curveRad, curveRad);
			circle.setPosition(SCALE * position.x + GAME_LENGTH / 2, SCALE * position.y + GAME_HIGHT / 2 + m_fOffset);
			
			normal = sf::Vector3f(position.x / SPHERE_RADIUS, position.y / SPHERE_RADIUS, position.z / SPHERE_RADIUS);
			circle.setFillColor(CURVE_MATERIALS[curve.getColorIndex()].getColor(normal, m_v3fLight, m_colLightColor));

			m_rtTarget.draw(circle);
		}
	}
	*/
#pragma endregion
}

void Camera::DrawStars()
{
	sf::CircleShape starShape(2);
	starShape.setFillColor(sf::Color::White);
	
	double sinAngle = sin(m_fRotationAngle);
	double cosAngle = cos(m_fRotationAngle);

	sf::VertexArray mystars(sf::Triangles);
	sf::Vertex tmpvertex;

	for (auto star : m_aStars)
	{
		sf::Vector3f pos = star.position;

		pos = m_mMat.UnRotateFromSeed(pos);

		pos = MyMat::RotateZ(pos, sinAngle, cosAngle);

		if (pos.z < 0)
		{
			float distance = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
			const float avg = sqrt(Constants::GAME_LENGTH * Constants::GAME_HIGHT);
			pos.x *= Constants::GAME_LENGTH / distance;
			pos.y *= Constants::GAME_LENGTH / distance;

			if (abs(pos.y) <= Constants::GAME_HIGHT / 2)
			{
				starShape.setFillColor(sf::Color(255, 255, 255, 255 * (1.5 * Constants::MAX_GALAXY_RADIUS - distance) / (Constants::MAX_GALAXY_RADIUS)));
				starShape.setPosition(pos.x + Constants::GAME_LENGTH / 2, pos.y + Constants::GAME_HIGHT / 2 + m_fOffset);
				starShape.setPointCount(star.sides);
				//m_rtTarget.draw(starShape);

				//proof of concept
				tmpvertex.color == sf::Color::White;
				tmpvertex.position = starShape.getPosition();
				mystars.append(tmpvertex);
				tmpvertex.position.x += 2;
				tmpvertex.position.y -= 2;
				mystars.append(tmpvertex);
				tmpvertex.position.x += 2;
				tmpvertex.position.y += 2;
				mystars.append(tmpvertex);
			}
		}
	}

	m_rtTarget.draw(mystars);
}

void Camera::DrawSphere()
{
	sf::Sprite sphere = Sphere::getSphere(m_v3fLight);
	m_rtTarget.draw(sphere);
}

void Camera::Draw()
{
	DrawStars();
	DrawSphere();

	// Creating new threads for each curve
	for (auto curve : m_aCurves)
	{
		DrawCurve(*curve);
	}

	// Changing camera offset
	if (abs(m_fOffset - m_fTargetOffset) < 1)
	{
		m_fTargetOffset = Lerp(m_fTargetOffset, 0, 1.96);
	}

	m_fOffset = Lerp(m_fOffset, m_fTargetOffset, 0.9);
}

void Camera::Follow(sf::Vector3f vector, sf::Vector3f rotAxis, float rotationAngle)
{
	/*m_v3fPosition = vector;
	m_v3fRotAxis = rotAxis;

	m_mMat.SeedVector2(m_v3fPosition);
	sf::Vector3f proportionalAxis = m_mMat.UnRotateFromSeed2(m_v3fRotAxis);
	m_fRotationAngle = -atan2(proportionalAxis.y, proportionalAxis.z);

	m_v3fLight = m_mMat.UnRotateFromSeed2(sf::Vector3f(1, 0, 0));
	m_v3fLight = m_mMat.RotateX(m_v3fLight, m_fRotationAngle);
	*/


	m_v3fPosition = MyMat(vector).LerpToSeed(m_v3fPosition, 0.2);
	m_v3fRotAxis = MyMat(rotAxis).LerpToSeed(m_v3fRotAxis, 0.05);
	//m_v3fRotAxis = rotAxis;

	m_mMat.SeedVector(m_v3fPosition);
	sf::Vector3f proportionalAxis = m_mMat.UnRotateFromSeed(m_v3fRotAxis);
	m_fRotationAngle = -atan2(proportionalAxis.y, proportionalAxis.x);; // +rotationAngle * Constants::DEG_TO_RADS;

	m_v3fLight = m_mMat.UnRotateFromSeed(sf::Vector3f(0, 0, 1));
	m_v3fLight = m_mMat.RotateZ(m_v3fLight, m_fRotationAngle);
}

float Camera::Lerp(float val1, float val2, float amount)
{
	return val1 + (val2 - val1) * amount;
}

void Camera::Bang()
{
	m_fTargetOffset = 0;// 50;
}