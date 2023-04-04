#pragma once

#include <SFML\Network.hpp>
#include "Constants1.h"

struct Command
{
	STREAM_COMMANDS type;

	union
	{
		struct
		{
			int		CurveID;
			int		ColorIndex;
			char	Name[Constants::MAX_NAME_LENGTH + 1];
			bool	Local;
		} Add;

		struct
		{
			int CurveID;
			float X;
			float Y;
			float Z;
		} Position;

		struct
		{
			int CurveID;
			float X;
			float Y;
			float Z;
		} RotAxis;

		struct
		{
			int CurveID;
		} Remove;

		struct
		{
			int CurveID;
		} Kill;

		struct
		{
			int CurveID;
		} Kick;

		struct
		{
			bool on;
			int id;
		} GodMod;

		struct
		{
			int id;
		} RoundWinner;

		struct
		{
			int id;
		} Winner;

		struct
		{
			int id;
			float angle;
		} PrevAngle;

		struct
		{
			char debug[255];
		} Debug;
	} Attributes;
};

class CommunicationBox
{
public:
	CommunicationBox(sf::TcpSocket& connectedSocket);
	~CommunicationBox();

	void Flush();

	void SendCAdd(std::string name, bool isBool);

	void SendCDirection(int nCurveID, AbsoluteDirection adDirection);

	void SendCRemove(int id);

	void SendCKick(int id);

	void SendCStart();

	bool PollCommand(Command& command);

private:
	sf::TcpSocket&	m_socServer;
	sf::Packet		m_pacIn;
	sf::Packet		m_pacOut;
	bool			m_bIsConnected;
};

