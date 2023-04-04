#pragma once

#include <vector>
#include <TGUI\TGUI.hpp>
#include "ClientCurve.h"
#include "CurveCollection.h"

class ScoreGui : private tgui::Gui
{
public:
	tgui::Gui::draw;
	std::string m_strDebug;

	ScoreGui(sf::RenderWindow& window, CurveCollection& curves);
	~ScoreGui();

	void init();
	void update();
	void setPtsToReach(int points);
	void setWinner(ClientCurve& winner);

private:
	CurveCollection& m_ccCurves;
	std::vector < tgui::Label::Ptr > m_aLabels;
	tgui::Label::Ptr m_lPtsToReach;
	tgui::Label::Ptr m_lPtsDebug;
};