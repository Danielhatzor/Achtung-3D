#include <future>
#include <SFML\System\Time.hpp>
#include <SFML\Main.hpp>
#include <TGUI\TGUI.hpp>
#include "SMatch.h"
#include "Sphere.h"
#include "Match.h"
#include "Constants1.h"
#include "Menu.h"

void hostGame(sf::RenderWindow& window);
void hostGame1();
void joinGame(sf::RenderWindow& window, sf::TcpSocket& socket, bool isHost);
void loadContent(sf::RenderWindow& window);

void main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(Constants::GAME_LENGTH, Constants::GAME_HIGHT), "Achtung 3D", sf::Style::Fullscreen, settings);
	Menu menu(window);
	srand(time(NULL));

	loadContent(window);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			menu.handleEvent(event);
		}

		tgui::Callback callback;

		while (menu.pollCallback(callback))
		{
			// host
			if (callback.id == 1)
			{
				std::thread* hostThread = new std::thread(hostGame1);
				//hostGame1(window);

				sf::TcpSocket socket;
				auto success = socket.connect(sf::IpAddress::LocalHost, Constants::LISTEN_PORT);
				while (success != sf::Socket::Status::Done);
				joinGame(window, socket, true);
			}
			// join
			else
			{
				sf::IpAddress ip(menu.GetIp());
				sf::TcpSocket socket;
				auto success = socket.connect(ip, Constants::LISTEN_PORT);
				
				if (success == sf::Socket::Status::Done)
				{
					joinGame(window, socket, false);
				}
			}
		}

		menu.draw();
		window.display();
		window.clear();
	}

	Sphere::unload();
}

void hostGame(sf::RenderWindow& window)
{
	sf::Clock timer;
	sf::Time elasped;
	sf::Time loopTime = sf::seconds(1) / (float)Constants::GAME_LOOPS;

	SMatch serverMatch;

	sf::TcpSocket socket;
	socket.connect(sf::IpAddress::LocalHost, Constants::LISTEN_PORT);

	Match clientMatch(socket, window, true);

	while (window.isOpen())
	{
		clientMatch.GeneralGameCycle();

		if (!clientMatch.HasStarted())
		{
			serverMatch.PreGameCycle();
			clientMatch.PreGameCycle();
		}
		else
		{
			serverMatch.InGameCycle();
			clientMatch.InGameCycle();
		}

		clientMatch.Display();

		elasped = timer.getElapsedTime();
		sf::sleep(loopTime - elasped);
		timer.restart();
	}
}

void hostGame1()
{
	sf::Clock timer;
	sf::Time elasped;
	sf::Time loopTime = sf::seconds(1) / (float)Constants::GAME_LOOPS;

	SMatch serverMatch;

	while (true)
	{
		if (!serverMatch.HasStarted())
		{
			serverMatch.PreGameCycle();
		}
		else
		{
			serverMatch.InGameCycle();
		}

		elasped = timer.getElapsedTime();
		sf::sleep(loopTime - elasped);
		timer.restart();
	}
}


void joinGame(sf::RenderWindow& window, sf::TcpSocket& socket, bool isHost)
{
	sf::Clock timer;
	sf::Time elasped;
	sf::Time loopTime = sf::seconds(1) / (float)Constants::GAME_LOOPS;

	Match clientMatch(socket, window, isHost);

	while (window.isOpen())
	{
		clientMatch.GeneralGameCycle();

		if (!clientMatch.HasStarted())
		{
			clientMatch.PreGameCycle();
		}
		else
		{
			clientMatch.InGameCycle();
		}

		clientMatch.Display();

		elasped = timer.getElapsedTime();
		sf::sleep(loopTime - elasped);
		timer.restart();
	}
}

void loadContent(sf::RenderWindow& window)
{
	tgui::Gui gui(window);
	tgui::Label::Ptr label(gui);
	tgui::Label::Ptr percent(gui);

	gui.setGlobalFont("TGUI-0.6/fonts/DejaVuSans.ttf");
	label->setTextSize(60);
	label->setTextColor(sf::Color::White);
	label->setText("Loading...");
	label->setPosition(Constants::GAME_LENGTH / 2 - label->getSize().x / 2, Constants::GAME_HIGHT / 2 - label->getSize().y / 2);
	percent->setTextSize(40);
	percent->setTextColor(sf::Color::Red);

	auto f = std::async(Sphere::load);
	auto status = f.wait_for(std::chrono::milliseconds(100));

	while (window.isOpen() && status != std::future_status::ready)
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		percent->setText(std::to_string(int(100 * Sphere::texturesLoaded / (Constants::PI * Constants::SPHERE_RADIUS))) + "%");
		percent->setPosition(Constants::GAME_LENGTH / 2 - percent->getSize().x / 2, Constants::GAME_HIGHT - percent->getSize().y / 2 - 50);

		gui.draw();
		window.display();
		window.clear();
		status = f.wait_for(std::chrono::milliseconds(100));
	}
}