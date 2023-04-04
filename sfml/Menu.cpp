#include "Menu.h"
#include "Constants1.h"


Menu::Menu(sf::RenderWindow& window)
	: m_bHost(*this),
	m_bJoin(*this),
	m_ebIp(*this)
{
	setWindow(window);

	setGlobalFont("TGUI-0.6/fonts/DejaVuSans.ttf");

	m_bHost->load("TGUI-0.6/widgets/Black.conf");
	m_bHost->setSize(110, 30);
	m_bHost->setPosition(Constants::GAME_LENGTH / 2 - 60, Constants::GAME_HIGHT / 2 - 100);
	m_bHost->setText("Host");
	m_bHost->setTextColor(sf::Color::White);
	m_bHost->setTextSize(15);
	m_bHost->bindCallback(tgui::Button::LeftMouseClicked);
	m_bHost->setCallbackId(1);

	m_bJoin->load("TGUI-0.6/widgets/Black.conf");
	m_bJoin->setSize(110, 30);
	m_bJoin->setPosition(Constants::GAME_LENGTH / 2 - 60, Constants::GAME_HIGHT / 2 + 50);
	m_bJoin->setText("Join");
	m_bJoin->setTextColor(sf::Color::White);
	m_bJoin->setTextSize(15);
	m_bJoin->bindCallback(tgui::Button::LeftMouseClicked);
	m_bJoin->setCallbackId(0);

	m_ebIp->load("TGUI-0.6/widgets/White.conf");
	m_ebIp->setSize(150, 30);
	m_ebIp->setTextSize(17);
	m_ebIp->setPosition(Constants::GAME_LENGTH / 2 - 250, Constants::GAME_HIGHT / 2 + 50);
}


Menu::~Menu()
{
}

std::string Menu::GetIp()
{
	return m_ebIp->getText();
}