#pragma once

#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <TGUI\TGUI.hpp>
#include "IMatch.h"
#include "CommunicationBox.h"
#include "CurveCollection.h"
#include "ClientGui.h"
#include "Camera.h"
#include "ScoreGui.h"

class Match : public IMatch
{
public:
	Match(sf::TcpSocket& connectedSocket, sf::RenderWindow& window, bool isHost);
	~Match();

	virtual bool HasStarted() override;
	virtual void Start() override;
	virtual void InGameCycle() override;
	virtual void PreGameCycle() override;
	bool HasEnded();
	void GeneralGameCycle();
	void Display();
private:
	sf::Music			music;
	CommunicationBox	m_cbServer;
	CurveCollection		m_ccCurves;
	ClientGui			m_cgGui;
	Camera				m_cCamera;
	sf::RenderTexture	m_rtGameImage;
	sf::RenderWindow&	m_rwWindow;
	bool				m_bHasStarted;
	bool				m_bIsHost;
	bool				m_bHasEnded;
	ClientCurve*		m_cFollowedCurve;
	int					m_nCameraPauseTime;
	ScoreGui			m_gScores;
};

