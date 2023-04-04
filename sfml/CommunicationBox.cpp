#define _CRT_SECURE_NO_WARNINGS

#include <SFML\Network.hpp>
#include "CommunicationBox.h"
#include "Constants1.h"
#include <cassert>
#include <iostream>

/*CommunicationBox::CommunicationBox(sf::IpAddress ip, int port)
{
	auto status = m_socServer.connect(ip, port);
	assert(status == sf::Socket::Status::Done);
	m_socServer.setBlocking(false);
}*/

CommunicationBox::CommunicationBox(sf::TcpSocket& connectedSocket) : m_socServer(connectedSocket)
{
	m_socServer.setBlocking(false);
}

CommunicationBox::~CommunicationBox()
{

}

void CommunicationBox::Flush()
{
	if (!m_pacOut.endOfPacket())
	{
		while (m_socServer.send(m_pacOut) == sf::Socket::Status::Partial);
		m_pacOut.clear();
	}
}

void CommunicationBox::SendCAdd(std::string name, bool isBot)//int nColorIndex)
{
	m_pacOut << -1 << static_cast<int>(STREAM_COMMANDS::ADD) << name << isBot << -1;
}

void CommunicationBox::SendCDirection(int nCurveID, AbsoluteDirection adDirection)
{
	m_pacOut << -1 << static_cast<int>(STREAM_COMMANDS::DIRECTION) << nCurveID << static_cast<int>(adDirection) << -1;
}

void CommunicationBox::SendCRemove(int id)
{
	m_pacOut << -1 << static_cast<int>(STREAM_COMMANDS::REMOVE_CURVE) << id << -1;
}

void CommunicationBox::SendCStart()
{
	m_pacOut << -1 << static_cast<int>(STREAM_COMMANDS::START) << -1;
}

void CommunicationBox::SendCKick(int id)
{
	m_pacOut << -1 << static_cast<int>(STREAM_COMMANDS::KICK) << id << -1;
}

bool CommunicationBox::PollCommand(Command& command)
{
	int nBuffer;

	if (m_pacIn.endOfPacket())
	{
		while(m_socServer.receive(m_pacIn) == sf::Socket::Status::Partial);

		if (m_pacIn.endOfPacket())
		{
			return false;
		}
	}

	m_pacIn >> nBuffer;

	STREAM_COMMANDS ctCom = static_cast<STREAM_COMMANDS>(nBuffer);

	switch (ctCom)
	{
	case STREAM_COMMANDS::START:
	{
		command.type = STREAM_COMMANDS::START;

		break;
	}

	case STREAM_COMMANDS::ADD:
	{
		bool bIsLocal;
		int nCurveID;
		int nColorIndex;
		std::string name;

		m_pacIn >> nCurveID >> bIsLocal >> nColorIndex >> name;

		command.type = STREAM_COMMANDS::ADD;
		command.Attributes.Add.Local = bIsLocal;
		command.Attributes.Add.CurveID = nCurveID;
		command.Attributes.Add.ColorIndex = nColorIndex;
		strcpy(command.Attributes.Add.Name, name.c_str());

		break;
	}
	case STREAM_COMMANDS::POSITION:
	{
		int nCurveID;
		float nX;
		float nY;
		float nZ;

		m_pacIn >> nCurveID >> nX >> nY >> nZ;

		command.type = STREAM_COMMANDS::POSITION;
		command.Attributes.Position.CurveID = nCurveID;
		command.Attributes.Position.X = nX;
		command.Attributes.Position.Y = nY;
		command.Attributes.Position.Z = nZ;

		break;
	}
	case STREAM_COMMANDS::ROT_AXIS:
	{
		int nCurveID;
		float nX;
		float nY;
		float nZ;

		m_pacIn >> nCurveID >> nX >> nY >> nZ;

		command.type = STREAM_COMMANDS::ROT_AXIS;
		command.Attributes.RotAxis.CurveID = nCurveID;
		command.Attributes.RotAxis.X = nX;
		command.Attributes.RotAxis.Y = nY;
		command.Attributes.RotAxis.Z = nZ;

		break;
	}
	case STREAM_COMMANDS::REMOVE_CURVE:
	{
		int nCurveID;

		m_pacIn >> nCurveID;;

		command.type = STREAM_COMMANDS::REMOVE_CURVE;
		command.Attributes.Remove.CurveID = nCurveID;

		break;
	}
	case STREAM_COMMANDS::KILL:
	{
		int nCurveID;

		m_pacIn >> nCurveID;

		command.type = STREAM_COMMANDS::KILL;
		command.Attributes.Kill.CurveID = nCurveID;

		break;
	}
	case STREAM_COMMANDS::GOD_MOD:
	{
		int id;
		bool godMod;

		m_pacIn >> godMod >> id;

		command.type = STREAM_COMMANDS::GOD_MOD;
		command.Attributes.GodMod.on = godMod;
		command.Attributes.GodMod.id = id;

		break;
	}
	case STREAM_COMMANDS::RESTART:
	{
		command.type = STREAM_COMMANDS::RESTART;

		break;
	}
	case STREAM_COMMANDS::ROUND_WINNER:
	{
		int id;

		m_pacIn >> id;

		command.type = STREAM_COMMANDS::ROUND_WINNER;
		command.Attributes.RoundWinner.id = id;

		break;
	}
	case STREAM_COMMANDS::WINNER:
	{
		int id;

		m_pacIn >> id;

		command.type = STREAM_COMMANDS::WINNER;
		command.Attributes.Winner.id = id;

		break;
	}
	case STREAM_COMMANDS::PREV_ANGLE:
	{
		int id;
		float angle;

		m_pacIn >> id >> angle;

		command.type = STREAM_COMMANDS::PREV_ANGLE;
		command.Attributes.PrevAngle.id = id;
		command.Attributes.PrevAngle.angle = angle;

		break;
	}
	case STREAM_COMMANDS::DEBUG:
	{
		std::string debug;

		m_pacIn >> debug;

		command.type = STREAM_COMMANDS::DEBUG;
		strcpy(command.Attributes.Debug.debug, debug.c_str());
		break;
	}
	default:
	{
		return PollCommand(command);
	}
	}

	return true;
}