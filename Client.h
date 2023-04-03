#pragma once

#include <vector>
#include <string>
#include <SFML\Network.hpp>
#include "Constants1.h"

typedef struct
{
	STREAM_COMMANDS type;
	int clientId;

	union
	{
		struct
		{
			char Name[Constants::MAX_NAME_LENGTH + 1];
			bool isBot;
		} Add;

		struct
		{
			int CurveID;
		} RemoveCurve;

		struct
		{
			int CurveId;
			int AbsDirection;
		} Direction;

		struct
		{
			int ClientId;
		} Kick;
	} Attributes;
} SCommand;

class Client
{
public:
	Client(sf::TcpSocket& ConnectedSoc, int ClientId);
	~Client();

	void AddCurveId(int id);
	bool HasCurveId(int id);
	int GetId();
	void RemoveCurveId(int id);
	const std::vector<int>& GetCurveIds();
	void Send(sf::Packet& packet);
	void Flush();
	bool PollCommand(SCommand& command);
	void SendCPosition(int id, float x, float y, float z);
	void SendCRotAxis(int id, float x, float y, float z);
	void SendCGodMod(bool on, int id);
	void SendCAdd(int id, bool isLocal, int colorIndex, const std::string& name);
	void SendCLog(std::string& text);
	void SendCStart();
	void SendCRemove(int curveId);
	void SendCKill(int curveId);
	void SendCRoundWinner(int curveId);
	void SendCNewRound();
	void SendCWinner(int curveId);
	void SendCPrevAngle(int curveId, float angle);
	void SendCDebug(std::string& debug);

private:
	int m_nId;
	sf::TcpSocket& m_socClientSocket;
	sf::Packet m_pacIn;
	sf::Packet m_pacOut;
	std::vector<int> m_vecCurveIds;
};