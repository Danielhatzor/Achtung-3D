#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <cassert>
#include <SFML\Network.hpp>
#include "Constants1.h"
#include "Client.h"

Client::Client(sf::TcpSocket& ConnectedSoc, int ClientId) : m_socClientSocket(ConnectedSoc), m_nId(ClientId)
{
	// Check if socket is connected
	assert(m_socClientSocket.getLocalPort());

	m_socClientSocket.setBlocking(false);
}

Client::~Client()
{
}

void Client::AddCurveId(int id)
{
	m_vecCurveIds.push_back(id);
}

bool Client::HasCurveId(int id)
{
	for (int tempId : m_vecCurveIds)
	{
		if (tempId == id)
		{
			return true;
		}
	}
	return false;
}

int Client::GetId()
{
	return m_nId;
}

void Client::RemoveCurveId(int curveId)
{
	// removes all elements with the value curveId
	m_vecCurveIds.erase(std::remove(m_vecCurveIds.begin(), m_vecCurveIds.end(), curveId), m_vecCurveIds.end());
}

const std::vector<int>& Client::GetCurveIds()
{
	return m_vecCurveIds;
}

void Client::Send(sf::Packet& packet)
{
	m_pacOut.append(packet.getData(), packet.getDataSize());
}

void Client::Flush()
{
	if (!m_pacOut.endOfPacket())
	{
		while (m_socClientSocket.send(m_pacOut) == sf::Socket::Status::Partial);
		m_pacOut.clear();
	}
}

bool Client::PollCommand(SCommand& command)
{
	int buffer;

	if (m_pacIn.endOfPacket())
	{
		while(m_socClientSocket.receive(m_pacIn) == sf::Socket::Partial);

		if (m_pacIn.endOfPacket())
		{
			return false;
		}
	}

	m_pacIn >> buffer;

	STREAM_COMMANDS ctCom = static_cast<STREAM_COMMANDS>(buffer);

	switch (ctCom)
	{
	case STREAM_COMMANDS::START:
	{
		command.type = STREAM_COMMANDS::START;
		break;
	}
	case STREAM_COMMANDS::ADD:
	{
		std::string name;
		bool isBot;

		m_pacIn >> name >> isBot;

		command.type = STREAM_COMMANDS::ADD;
		strcpy(command.Attributes.Add.Name, name.c_str());
		command.Attributes.Add.isBot = isBot;

		break;
	}
	case STREAM_COMMANDS::REMOVE_CURVE:
	{
		int curveID;

		m_pacIn >> curveID;;

		command.type = STREAM_COMMANDS::REMOVE_CURVE;
		command.Attributes.RemoveCurve.CurveID = curveID;

		break;
	}
	case STREAM_COMMANDS::DIRECTION:
	{
		int curveId;
		int direction;

		m_pacIn >> curveId >> direction;

		command.type = STREAM_COMMANDS::DIRECTION;
		command.Attributes.Direction.CurveId = curveId;
		command.Attributes.Direction.AbsDirection = direction;

		break;
	}
	case STREAM_COMMANDS::KICK:
	{
		int clientId;

		m_pacIn >> clientId;

		command.type = STREAM_COMMANDS::KICK;
		command.Attributes.Kick.ClientId = clientId;

		break;
	}
	default:
	{
		return PollCommand(command);
	}
	}

	command.clientId = GetId();

	return true;
}

void Client::SendCPosition(int id, float x, float y, float z)
{
	m_pacOut << -1 << (int)STREAM_COMMANDS::POSITION << id << x << y << z << -1;
}


void Client::SendCRotAxis(int id, float x, float y, float z)
{
	m_pacOut << -1 << (int)STREAM_COMMANDS::ROT_AXIS << id << x << y << z << -1;
}

void Client::SendCGodMod(bool on, int id)
{
	m_pacOut << -1 << (int)STREAM_COMMANDS::GOD_MOD << on << id << -1;
}

void Client::SendCAdd(int id, bool isLocal, int colorIndex, const std::string& name)
{
	m_pacOut << -1 << (int)STREAM_COMMANDS::ADD << id << isLocal << colorIndex << name << -1;
}

void Client::SendCLog(std::string& text)
{
	m_pacOut << -1 << (int)STREAM_COMMANDS::LOG << text << -1;
}

void Client::SendCStart()
{
	m_pacOut << -1 << (int)STREAM_COMMANDS::START << -1;
}

void Client::SendCRemove(int curveId)
{
	m_pacOut << -1 << (int)STREAM_COMMANDS::REMOVE_CURVE << curveId << -1;
}

void Client::SendCKill(int curveId)
{
	m_pacOut << -1 << (int)STREAM_COMMANDS::KILL << curveId << -1;
}

void Client::SendCRoundWinner(int curveId)
{
	m_pacOut << -1 << (int)STREAM_COMMANDS::ROUND_WINNER << curveId << -1;
}

void Client::SendCNewRound()
{
	m_pacOut << -1 << (int)STREAM_COMMANDS::RESTART << -1;
}

void Client::SendCWinner(int curveId)
{
	m_pacOut << -1 << (int)STREAM_COMMANDS::WINNER << curveId << -1;
}

void Client::SendCPrevAngle(int curveId, float angle)
{
	m_pacOut << -1 << (int)STREAM_COMMANDS::PREV_ANGLE << curveId << angle << -1;
}

void Client::SendCDebug(std::string& debug)
{
	m_pacOut << -1 << (int)STREAM_COMMANDS::DEBUG << debug << -1;
}