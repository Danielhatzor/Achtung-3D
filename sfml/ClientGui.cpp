#include <string>
#include <TGUI/TGUI.hpp>
#include "Constants1.h"
#include <queue>
#include <string>
#include "ClientGui.h"

ClientGui::ClientGui(sf::RenderWindow& window, bool isHost)
	: tgui::Gui(window),
	m_nPlayers(0),
	m_bIsHost(isHost),
	m_lTitle(*this),
	m_bStartButton(*this),
	//m_bAddPlayerButton(*this),
	m_bAddBotButton(*this)
	//m_ebNameEditBox(*this)
{
	// Load the font (you should check the return value to make sure that it is loaded)
	setGlobalFont("TGUI-0.6/fonts/DejaVuSans.ttf");

	// Create the title label
	m_lTitle->setText("HatzoryFever");
	m_lTitle->setPosition(400, 100);

	// Create the start button
	m_bStartButton->load("TGUI-0.6/widgets/Black.conf");
	m_bStartButton->setSize(110, 30);
	m_bStartButton->setPosition(170, 540);
	m_bStartButton->setText("Start");
	m_bStartButton->bindCallback(tgui::Button::LeftMouseClicked);
	m_bStartButton->setCallbackId(int(GuiCommand::START));

	/*// Create the add-player button
	m_bAddPlayerButton->load("TGUI-0.6/widgets/Black.conf");
	m_bAddPlayerButton->setSize(150, 30);
	m_bAddPlayerButton->setPosition(320, 540);
	m_bAddPlayerButton->setText("Add player");
	m_bAddPlayerButton->bindCallback(tgui::Button::LeftMouseClicked);
	m_bAddPlayerButton->setCallbackId(int(GuiCommand::ADD_PLAYER));
	m_bAddPlayerButton->disable();*/

	// Create the add-bot button
	m_bAddBotButton->load("TGUI-0.6/widgets/Black.conf");
	m_bAddBotButton->setSize(150, 30);
	m_bAddBotButton->setPosition(720, 540);
	m_bAddBotButton->setText("Add bot");
	m_bAddBotButton->bindCallback(tgui::Button::LeftMouseClicked);
	m_bAddBotButton->setCallbackId(int(GuiCommand::ADD_BOT));

	/*// Create player name text box
	m_ebNameEditBox->load("TGUI-0.6/widgets/White.conf");
	m_ebNameEditBox->setSize(150, 30);
	m_ebNameEditBox->setTextSize(17);
	m_ebNameEditBox->setPosition(490, 540);
	m_ebNameEditBox->setMaximumCharacters(Constants::MAX_NAME_LENGTH);*/

	if (!isHost)
	{
		m_bAddBotButton->disable();
		m_bStartButton->disable();
	}
}

ClientGui::~ClientGui()
{
}

void ClientGui::AddPlayerPanel(std::string playerName, sf::Color color, std::string leftKey, std::string rightKey, bool isLocal, int id)
{
	tgui::Panel::Ptr playerPanel(*this);
	playerPanel->setSize(500, 100);
	//playerPanel->setPosition(50, 100 + 30 * m_nPlayers);

	// Create the name label
	tgui::Label::Ptr labelName;// (playerPanel);
	labelName->setText(playerName);
	labelName->setTextSize(17);
	labelName->setTextColor(color);
	labelName->setPosition(0, 0);
	playerPanel->add(labelName);

	/*// Create the keys label
	tgui::Label::Ptr keysLabel;
	keysLabel->setText(leftKey + "  " + rightKey);
	keysLabel->setPosition(80, 3);
	keysLabel->setTextSize(17);
	keysLabel->setSize(50, 20);
	playerPanel->add(keysLabel);*/

	// Create an X button if player is a bot
	if (playerName[0] == 'B')
	{
		tgui::Button::Ptr addPlayerButton;
		addPlayerButton->load("TGUI-0.6/widgets/Black.conf");
		addPlayerButton->setSize(30, 15);
		addPlayerButton->setPosition(170, 0);
		addPlayerButton->setText("X");
		addPlayerButton->bindCallback(tgui::Button::LeftMouseClicked);
		addPlayerButton->setCallbackId(id * PANEL_ID_DIVIDER + int(GuiCommand::REMOVE));
		playerPanel->add(addPlayerButton);
	}
	/*// Create a kick button if this is the host
	else if (m_bIsHost)
	{
		tgui::Button::Ptr kickButton;
		kickButton->load("TGUI-0.6/widgets/Black.conf");
		kickButton->setSize(50, 15);
		kickButton->setTextSize(13);
		kickButton->setPosition(170, 0);
		kickButton->setText("Kick");
		kickButton->bindCallback(tgui::Button::LeftMouseClicked);
		kickButton->setCallbackId(id * PANEL_ID_DIVIDER + int(GuiCommand::KICK));
		playerPanel->add(kickButton);
	}*/

	playerPanel->setSize(220, 15);
	playerPanel->setBackgroundColor(sf::Color::Transparent);

	// Adding to queue
	m_qPanels.push_back(playerPanel);

	// Setting panel name to id
	setWidgetName(playerPanel, std::to_string(id));

	SetPanelPositions();
}

void ClientGui::RemovePlayerPanel(int id)
{
	for (auto panel : m_qPanels)
	{
		std::string panelId;
		getWidgetName(panel, panelId);

		if (panelId == std::to_string(id))
		{
			remove(panel);
			m_qPanels.remove(panel);

			break;
		}
	}

	SetPanelPositions();
}

void ClientGui::SetPanelPositions()
{
	int index = 1;
	for (auto panel : m_qPanels)
	{
		panel->setPosition(50, 100 + 30 * index);
		++index;
	}
}

std::string ClientGui::getTypedName()
{
	return m_ebNameEditBox->getText();
}

void ClientGui::SetAddButton()
{
	/*if (m_ebNameEditBox->getText().isEmpty())
	{
		m_bAddPlayerButton->disable();
		m_bAddBotButton->disable();
	}
	else
	{
		m_bAddPlayerButton->enable();

		if (m_bIsHost)
		{
			m_bAddBotButton->enable();
		}
	}*/
}

int ClientGui::GetEventId(int callBackId)
{
	return callBackId % PANEL_ID_DIVIDER;
}

int ClientGui::GetPlayerId(int callBackId)
{
	return callBackId / PANEL_ID_DIVIDER;
}