#pragma once

#include <vector>
#include <string>
#include <SFML\Network.hpp>
#include "Client.h"

class SConBox
{
public:
	SConBox();
	~SConBox();

	std::vector<Client*> GetClients();
	Client& operator[](int clientId);
	int AcceptClient();
	bool PollCommand(SCommand& command);
	void FlushAll();
	int AddClient(sf::TcpSocket& socket);
	Client& GetHost();
	void RemoveClient(int clientId);
	void SendCPosition(int id, float x, float y, float z);
	void SendCRotAxis(int id, float x, float y, float z);
	void SendCGodMod(bool on, int curveId);
	void SendCAddCurve(int curveId, int colorIndex, const std::string& name);
	void SendCLog(std::string text, int clientId);
	void SendCStart();
	void SendCRemoveCurve(int id);
	void SendCKill(int curveId);
	void SendCRoundWinner(int curveId);
	void SendCNewRound();
	void SendCWinner(int curveId);
	void SendCPrevAngle(int curveId, float angle);
	void SConBox::SendCDebug(std::string& debug);
private:
	int						m_NextId;
	sf::TcpListener			m_tlListener;
	std::vector<Client*>	m_vecClients;
	Client*					m_cHost;
};