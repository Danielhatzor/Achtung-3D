#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <SFML\Network.hpp>
#include "SConBox.h"
#include "Client.h"
#include "Constants1.h"

SConBox::SConBox() : m_NextId(0), m_cHost(nullptr)
{
	m_tlListener.setBlocking(false);

	while (m_tlListener.listen(Constants::LISTEN_PORT) != sf::Socket::Done);
}

SConBox::~SConBox()
{
	for (Client* client : m_vecClients)
	{
		delete client;
	}
}

std::vector<Client*> SConBox::GetClients()
{
	return m_vecClients;
}

Client& SConBox::operator[](int clientId)
{
	for (auto& client : m_vecClients)
	{
		if (client->GetId() == clientId)
		{
			return *client;
		}
	}

	assert(false);
}

int SConBox::AcceptClient()
{
	sf::TcpSocket* client = new sf::TcpSocket();

	if (m_tlListener.accept(*client) != sf::Socket::Done)
	{
		delete client;
		return -1;
	}

	return AddClient(*client);
}

bool SConBox::PollCommand(SCommand& command)
{
	for (Client* client : m_vecClients)
	{
		if ((*client).PollCommand(command))
		{
			return true;
		}
	}

	return false;
}

void SConBox::FlushAll()
{
	for (Client* client : m_vecClients)
	{
		client->Flush();
	}
}

int SConBox::AddClient(sf::TcpSocket& socket)
{
	Client* client = new Client(socket, m_NextId);
	m_vecClients.push_back(client);

	if (!m_cHost)
	{
		m_cHost = client;
	}

	++m_NextId;

	return m_NextId - 1;
}

Client& SConBox::GetHost()
{
	return *m_cHost;
}

void SConBox::RemoveClient(int clientId)
{
	for (int i = 0; i < m_vecClients.size(); ++i)
	{
		if (m_vecClients[i]->GetId() == clientId)
		{
			delete m_vecClients.at(i);
			m_vecClients.erase(m_vecClients.begin() + i);
		}
	}
}

void SConBox::SendCPosition(int curveId, float x, float y, float z)
{
	for (Client* client : m_vecClients)
	{
		client->SendCPosition(curveId, x, y, z);
	}
}


void SConBox::SendCRotAxis(int curveId, float x, float y, float z)
{
	for (Client* client : m_vecClients)
	{
		client->SendCRotAxis(curveId, x, y, z);
	}
}

void SConBox::SendCGodMod(bool on, int curveId)
{
	for (Client* client : m_vecClients)
	{
		client->SendCGodMod(on, curveId);
	}
}

void SConBox::SendCAddCurve(int curveId, int colorIndex, const std::string& name)
{
	for (Client* client : m_vecClients)
	{
		bool isLocal = client->HasCurveId(curveId);
		client->SendCAdd(curveId, isLocal, colorIndex, name);
	}
}

void SConBox::SendCLog(std::string text, int clientId)
{
	for (Client* client : m_vecClients)
	{
		if (client->GetId() == clientId)
		{
			client->SendCLog(text);
		}
	}
}

void SConBox::SendCStart()
{
	for (Client* client : m_vecClients)
	{
		client->SendCStart();
	}
}

void SConBox::SendCRemoveCurve(int id)
{
	for (Client* client : m_vecClients)
	{
		client->SendCRemove(id);
	}
}

void SConBox::SendCKill(int curveId)
{
	for (Client* client : m_vecClients)
	{
		client->SendCKill(curveId);
	}
}

void SConBox::SendCRoundWinner(int curveId)
{
	for (Client* client : m_vecClients)
	{
		client->SendCRoundWinner(curveId);
	}
}

void SConBox::SendCNewRound()
{
	for (Client* client : m_vecClients)
	{
		client->SendCNewRound();
	}
}

void SConBox::SendCWinner(int curveId)
{
	for (Client* client : m_vecClients)
	{
		client->SendCWinner(curveId);
	}
}

void SConBox::SendCPrevAngle(int curveId, float angle)
{
	for (Client* client : m_vecClients)
	{
		client->SendCPrevAngle(curveId, angle);
	}
}

void SConBox::SendCDebug(std::string& debug)
{
	for (Client* client : m_vecClients)
	{
		client->SendCDebug(debug);
	}
}