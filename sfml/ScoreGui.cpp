#include <vector>
#include <TGUI\TGUI.hpp>
#include "ScoreGui.h"
#include "CurveCollection.h"
#include "ClientCurve.h"

ScoreGui::ScoreGui(sf::RenderWindow& window, CurveCollection& curves)
	: tgui::Gui(window),
	m_ccCurves(curves),
	m_lPtsToReach(*this),
	m_lPtsDebug(*this)
{
	setGlobalFont("TGUI-0.6/fonts/DejaVuSans.ttf");

	m_lPtsToReach->setTextSize(25);
	m_lPtsToReach->setPosition(50, 50);

	m_lPtsDebug->setTextSize(25);
	m_lPtsDebug->setPosition(50, 300);
}

ScoreGui::~ScoreGui()
{
}

void ScoreGui::init()
{
	m_aLabels.resize(m_ccCurves.getCurves().size());

	for (int i = 0; i < m_ccCurves.getCurves().size(); ++i)
	{
		add(m_aLabels[i]);
		m_aLabels[i]->setTextSize(20);
		m_aLabels[i]->setPosition(50, 100 + i * 20);
	}
}

void ScoreGui::update()
{
	for (int i = 0; i < m_ccCurves.getCurves().size(); ++i)
	{
		ClientCurve* curve = m_ccCurves.GetCurvesByScores()[i];
		m_aLabels[i]->setTextColor(Constants::CURVE_COLORS[curve->getColorIndex()]);

		std::string spaces;
		int n = 15 - curve->getName().size();
		for (int t = 0; t < n; ++t)
		{
			spaces += ' ';
		}

		m_aLabels[i]->setText(curve->getName() + spaces + std::to_string(curve->points));
	}

	m_lPtsToReach->setTextColor(Constants::CURVE_COLORS[m_ccCurves.GetCurvesByScores()[0]->getColorIndex()]);
	m_lPtsDebug->setText(m_strDebug);
}

void ScoreGui::setPtsToReach(int points)
{
	m_lPtsToReach->setText("First to reach " + std::to_string(points));
}

void ScoreGui::setWinner(ClientCurve& winner)
{
	m_lPtsToReach->setText(winner.getName() + " won the match!");
}