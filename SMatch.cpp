#include <thread>
#include <SFML\System\Time.hpp>
#include "Constants1.h"
#include "SMatch.h"
#include "SConBox.h"
#include "SCurveCollection.h"
#include "Client.h"
#include "BotBrain.h"

SMatch::SMatch() 
	: m_bHasStarted(false),
	m_nNewRoundTimeOut(-1)
{
}

SMatch::~SMatch()
{
}

void SMatch::PreGameCycle()
{

#pragma region New clients
	if (m_sccCurves.GetCurves().size() < Constants::MAX_CURVES)
	{
		int newClientId = m_scbClients.AcceptClient();

		if (newClientId != -1)
		{
			for (SCurve* curve : m_sccCurves.GetCurves())
			{
				m_scbClients.GetClients()[newClientId]->SendCAdd(curve->getID(), false, curve->getColorIndex(), curve->getName());
			}

			m_scbClients.GetClients()[newClientId]->Flush();
		}
	}

#pragma endregion

	SCommand command;

	while (m_scbClients.PollCommand(command))
	{
		switch (command.type)
		{
		case STREAM_COMMANDS::ADD:
		{
			if (m_sccCurves.GetLength() < Constants::MAX_CURVES)
			{
				//std::string name = command.Attributes.Add.Name;
				bool isBot = command.Attributes.Add.isBot;
				int id = m_sccCurves.AddCurve(std::string(), isBot);
				std::string name;

				if (isBot)
				{
					name = "Bot " + std::to_string(id);
				}
				else
				{
					name = "Player " + std::to_string(id);
				}

				int colorIndex = m_sccCurves[id].getColorIndex();

				m_scbClients[command.clientId].AddCurveId(id);

				m_scbClients.SendCAddCurve(id, colorIndex, name);
			}

			break;
		}

		case STREAM_COMMANDS::START:
		{
			if (command.clientId == m_scbClients.GetHost().GetId() &&
				m_sccCurves.GetCurves().size() > 1)
			{
				m_scbClients.SendCStart();
				Start();
			}

			break;
		}

		case STREAM_COMMANDS::KICK:
		{
			if (command.clientId == m_scbClients.GetHost().GetId())
			{
				//Removing the client's curves
				for (int curveId : m_scbClients[command.Attributes.Kick.ClientId].GetCurveIds())
				{
					m_scbClients.SendCRemoveCurve(curveId);
					m_sccCurves.RemoveCurve(curveId);
				}

				//m_scbClients.SendCLog("Sorry, but the host has kicked you.", command.Attributes.Kick.ClientId);
				m_scbClients.RemoveClient(command.Attributes.Kick.ClientId);
			}

			break;
		}

		case STREAM_COMMANDS::REMOVE_CURVE:
		{
			int curveId = command.Attributes.RemoveCurve.CurveID;

			if (m_scbClients[command.clientId].HasCurveId(curveId))
			{
				m_scbClients.SendCRemoveCurve(curveId);
				m_scbClients[command.clientId].RemoveCurveId(curveId);
				m_sccCurves.RemoveCurve(curveId);
			}

			break;
		}
		default:
			break;
		}
	}

	m_scbClients.FlushAll();
}

void SMatch::InGameCycle()
{
#pragma region command handling
	static SCommand command;

	while (m_scbClients.PollCommand(command))
	{
		switch (command.type)
		{
		case STREAM_COMMANDS::DIRECTION:
		{
			int id = command.Attributes.Direction.CurveId;
			AbsoluteDirection direction = (AbsoluteDirection)command.Attributes.Direction.AbsDirection;

			if (m_sccCurves.HasId(id))
			{
				m_sccCurves[id].setMovementDirection(direction);
			}

			break;
		}
		default:
			break;
		}
	}
#pragma endregion

	static sf::Clock stopper;
	static sf::Time remainder;

	sf::Time elasped = stopper.getElapsedTime() + remainder;
	int loops = elasped.asSeconds() * Constants::CURVE_SPEED;

	remainder = elasped % sf::seconds((double)1 / Constants::CURVE_SPEED);

	for (int i = 0; i < loops; ++i)
	{
		
		// steps, apply godmods (randomally)
		m_sccCurves.UpdatePositions();
		m_sccCurves.ApplyGodMods(Constants::CURVE_GOD_MOD_CHANCE);
		m_sccCurves.KillCollidingCurves();

		// send positions, rotation axes, kills
		for (SCurve* curve : m_sccCurves.GetCurves())
		{
			if (!curve->isAlive())
			{
				m_scbClients.SendCKill(curve->getID());
			}
			else
			{
				if (curve->isGodModChanged())
				{
					m_scbClients.SendCGodMod(curve->hasGodMod(), curve->getID());
				}

				sf::Vector3f position = curve->getPosition();
				sf::Vector3f rotAxis = curve->getRotationAxis();

				
				m_scbClients.SendCPosition(curve->getID(), position.x, position.y, position.z);
				m_scbClients.SendCRotAxis(curve->getID(), rotAxis.x, rotAxis.y, rotAxis.z);
				//m_scbClients.SendCPrevAngle(curve->getID(), curve->previewAngle);
			}
		}
		std::string lawa("lawa");
		//m_sccCurves[1].futurePositions.length.min;
		lawa = "left - min:" + std::to_string(m_sccCurves[0].futurePaths[0].realLength.min) +
			" max:" + std::to_string(m_sccCurves[0].futurePaths[0].realLength.max) +
			"     straight - min:" + std::to_string(m_sccCurves[0].futurePaths[1].realLength.min) +
			" max:" + std::to_string(m_sccCurves[0].futurePaths[1].realLength.max) +
			"     right - min:" + std::to_string(m_sccCurves[0].futurePaths[2].realLength.min) +
			"max:" + std::to_string(m_sccCurves[0].futurePaths[2].realLength.max);

		/*CurvePath* k = &(m_sccCurves[1].persistantPath);
		int iii = 0;
		while (k != nullptr)
		{
			k = k->next;
			iii++;
		}*/
			
		//m_scbClients.SendCDebug(std::to_string(iii));
		m_scbClients.SendCDebug(lawa);


		if (m_sccCurves.GetRoundWinner() != nullptr)
		{
			if (m_nNewRoundTimeOut > 0)
			{
				m_scbClients.SendCRoundWinner(m_sccCurves.GetRoundWinner()->getID());
			}
			if (m_nNewRoundTimeOut == 0)
			{
				SCurve* winner = m_sccCurves.GetMatchWinner();

				if (!winner)
				{
					m_scbClients.SendCNewRound();
					m_sccCurves.Reset();
				}
				else
				{
					m_scbClients.SendCWinner(winner->getID());
				}
			}
			else if (m_nNewRoundTimeOut < 0)
			{
				m_nNewRoundTimeOut = Constants::CURVE_SPEED * 5;
			}

			--m_nNewRoundTimeOut;
		}
	}

	m_scbClients.FlushAll();

	stopper.restart();
}

bool SMatch::HasStarted()
{
	return m_bHasStarted;
}

void SMatch::Start()
{
	BotBrain* botBrain = new BotBrain(m_sccCurves);
	std::thread* botThread = new std::thread(&BotBrain::Run, (botBrain));

	m_bHasStarted = true;
}