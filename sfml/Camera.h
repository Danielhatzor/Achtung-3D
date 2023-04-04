#pragma once

#include <array>
#include <vector>
#include <SFML\Graphics.hpp>
#include "ClientCurve.h"
#include "MyMat.h"
#include "Sphere.h"

class Camera
{
public:
	Camera(sf::RenderTarget& target, std::vector<ClientCurve*>& curves, sf::Vector3f follow, sf::Vector3f rotAxis);
	~Camera();

	void Draw();
	void Follow(sf::Vector3f vector, sf::Vector3f rotAxis, float rotationAngle);
	void Bang();

private:
	static Material						sphereMaterial;

	sf::RenderTarget&					m_rtTarget;
	std::vector<ClientCurve*>&			m_aCurves;
	MyMat								m_mMat;
	float								m_fRotationAngle;
	sf::Vector3f						m_v3fPosition;
	sf::Vector3f						m_v3fRotAxis;
	sf::Vector3f						m_v3fLight;
	sf::Color							m_colLightColor;
	std::array<Star, Constants::STARS>	m_aStars;
	std::vector<std::vector<Grid>>		m_aSphereGrids;



	float								m_fOffset;
	float								m_fTargetOffset;

	static float Lerp(float val1, float val2, float amount);

	void DrawCurve(ClientCurve& curve);
	void DrawStars();
	void DrawSphere();
};