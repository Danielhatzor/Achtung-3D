#pragma once

#include <SFML\Graphics.hpp>
#include "Material.h"
#include "MinMax.h"

enum class AbsoluteDirection
{
	Left = -1,
	Straight,
	Right
};

enum class STREAM_COMMANDS
{
	ADD,
	DIRECTION,
	POSITION,
	ROT_AXIS,
	GOD_MOD,
	KILL,
	START,
	REMOVE_CURVE,
	WINNER,
	ROUND_WINNER,
	CLEAN,
	KICK,
	LOG,
	RESTART,
	PREV_ANGLE,
	LENGTH,
	DEBUG
};

enum class GuiCommand
{
	START = 1,
	ADD_PLAYER,
	ADD_BOT,
	REMOVE,
	KICK
};

struct CurveKeys
{
	sf::Keyboard::Key RightKey;
	sf::Keyboard::Key LeftKey;
	std::string RightName;
	std::string LeftName;
};

struct Star
{
	int sides;
	sf::Vector3f position;
};

struct Grid
{
	sf::Vector3f representedVec;
	bool isOnSphere;
};

namespace Constants
{
	const int			GAME_HIGHT = sf::VideoMode::getDesktopMode().height; //768;
	const int			GAME_LENGTH = sf::VideoMode::getDesktopMode().width; //1366;
	const float			SCALE = 2.2 * sqrt(Constants::GAME_HIGHT * Constants::GAME_LENGTH) / sqrt(768 * 1366); // * 2.2

#pragma region sphere constants
	const int			SPHERE_RADIUS = 150;
	const float			SPHERE_GRID = 4;// 2.5f;
#pragma endregion

	const sf::Color		LIGHT_COLOR = sf::Color::White;
	const int			STARS = 15000;
	const int			MAX_GALAXY_RADIUS = 15000;// 1000 * SPHERE_RADIUS;
	const double		PI = 3.14159f;
	const float			CURVE_HEAD_RADIUS = 2.3;
	const float			FAR_CURVE_RADIUS = 0.5;
	const int			CURVE_SPEED = 70; // Pixels per second
	const float			CURVE_ANGULAR_SPEED = 2.5f; // Degrees per step
	const int			CURVE_ROTATING_RADIUS = (360 / CURVE_ANGULAR_SPEED) / (2 * PI);
	const int			CURVE_GOD_MOD_POINTS = CURVE_HEAD_RADIUS * 7; // Pixels
	const int			CURVE_BOT_MIN_RAY_LEN = 3 * CURVE_ROTATING_RADIUS;
	const int			CURVE_BOT_MAX_RAY_LEN = (2 * PI * CURVE_ROTATING_RADIUS - 4 * CURVE_HEAD_RADIUS) / 2.7f;
	const int			CURVE_BOT_MAX_RAY_DEPTH = CURVE_BOT_MAX_RAY_LEN * 2.7f;
	const double		CURVE_GOD_MOD_CHANCE = 1.2f;
	const int			COLLISION_CHECKS_PER_STEP = 21;
	const double		DEG_TO_RADS = PI / 180;
	const int			MAX_CURVES = 6;
	const int			MAX_NAME_LENGTH = 15;
	const int			LISTEN_PORT = 53000;
	const int			GAME_LOOPS = 60;
	const int			CAMERA_PAUSE_TIME = GAME_LOOPS * 3;
	const int			MIN_ROUNDS_TO_WIN = 10;

	const sf::Color CURVE_COLORS[MAX_CURVES]
	{
		sf::Color::Blue,
		sf::Color::Cyan,
		sf::Color::Green,
		sf::Color::Magenta,
		sf::Color::Red,
		sf::Color::Yellow
	};

	const Material CURVE_MATERIALS[MAX_CURVES]
	{
		// Blue
		{
			{ 0.0f, 0.0f, 0.6f },
			{ 0.0f, 0.0f, 0.6f },
			{ 0.5f, 0.5f, 0.0f }, 16
		},

		// Cyan
		{
			{ 0.0f, 0.0f, 0.6f },
			{ 0.5f, 0.5f, 0.0f },
			{ 0.5f, 0.5f, 0.0f }, 16
		},

		// Green
		{
			{ 0.0f, 0.0f, 0.6f },
			{ 0.5f, 0.5f, 0.0f },
			{ 0.0f, 0.0f, 0.6f }, 16
		},

		// Magenta
		{ 
			{ 0.5f, 0.5f, 0.0f },
			{ 0.0f, 0.0f, 0.6f },
			{ 0.5f, 0.5f, 0.0f }, 16
		},

		// Red
		{
			{ 0.5f, 0.5f, 0.0f },
			{ 0.0f, 0.0f, 0.6f },
			{ 0.0f, 0.0f, 0.6f }, 16
		},

		// Yellow
		{
			{ 0.5f, 0.5f, 0.0f },
			{ 0.5f, 0.5f, 0.0f },
			{ 0.0f, 0.0f, 0.6f }, 16
		}
	};

	const Material SPHERE_MATERIAL
	{
		{ 0.05f, 0.3f, 0.3f },
		{ 0.05f, 0.3f, 0.3f },
		{ 0.1f, 0.6f, 0.3f }, 64
	};
	
	const CurveKeys PLAYER_KEYS[MAX_CURVES]
	{
		{ sf::Keyboard::W, sf::Keyboard::Q, "W", "Q" },
		{ sf::Keyboard::N, sf::Keyboard::B, "N", "B" },
		{ sf::Keyboard::P, sf::Keyboard::O, "P", "O" },
		{ sf::Keyboard::X, sf::Keyboard::Z, "X", "Z" },
		{ sf::Keyboard::Y, sf::Keyboard::T, "Y", "T" },
		{ sf::Keyboard::Right, sf::Keyboard::Left, "->", "<-" }
	};
}