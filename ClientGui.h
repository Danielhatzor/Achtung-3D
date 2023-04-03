#pragma once

#include <string>
#include <TGUI/TGUI.hpp>
#include "Constants1.h"
#include <queue>
#include"ClientCurve.h"

class ClientGui : private tgui::Gui
{
public:
	tgui::Gui::pollCallback;
	tgui::Gui::draw;
	tgui::Gui::handleEvent;

	ClientGui(sf::RenderWindow& window, bool isServer = false);
	~ClientGui();

	static int GetEventId(int callBackId);
	static int GetPlayerId(int callBackId);
	void AddPlayerPanel(std::string playerName, sf::Color color, std::string leftKey, std::string rightKey, bool isLocal, int id);
	void RemovePlayerPanel(int id);
	void SetAddButton();
	std::string getTypedName();

private:
	tgui::Label::Ptr m_lTitle;
	tgui::Button::Ptr m_bStartButton;
	tgui::Button::Ptr m_bAddPlayerButton;
	tgui::Button::Ptr m_bAddBotButton;
	tgui::EditBox::Ptr m_ebNameEditBox;

	static const int PANEL_ID_DIVIDER = 1000;
	int m_nPlayers;
	bool m_bIsHost;
	std::list<tgui::Panel::Ptr> m_qPanels;

	void SetPanelPositions();
};