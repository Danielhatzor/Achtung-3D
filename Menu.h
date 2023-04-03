#pragma once

#include <string>
#include <TGUI\TGUI.hpp>

class Menu : private tgui::Gui
{
public:
	tgui::Gui::pollCallback;
	tgui::Gui::draw;
	tgui::Gui::handleEvent;

	Menu(sf::RenderWindow& window);
	~Menu();

	std::string GetIp();

private:
	tgui::Button::Ptr m_bHost;
	tgui::Button::Ptr m_bJoin;
	tgui::EditBox::Ptr m_ebIp;
};

