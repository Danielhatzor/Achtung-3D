#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <TGUI\TGUI.hpp>
#include "Match.h"
#include "CommunicationBox.h"
#include "CurveCollection.h"
#include "ClientGui.h"

Match::Match(sf::TcpSocket& connectedSocket, sf::RenderWindow& window, bool isHost)
	: m_cbServer(connectedSocket),
	m_bHasStarted(false),
	m_bIsHost(isHost),
	m_bHasEnded(false),
	m_cgGui(window, isHost),
	m_rwWindow(window),
	m_cCamera(window, m_ccCurves.getCurves(), sf::Vector3f(0, 0, Constants::SPHERE_RADIUS), sf::Vector3f(0, Constants::SPHERE_RADIUS, 0)),
	m_cFollowedCurve(nullptr),
	m_nCameraPauseTime(0),
	m_gScores(window, m_ccCurves)
{
	m_cbServer.SendCAdd(m_cgGui.getTypedName(), false);
}

Match::~Match()
{
}

bool Match::HasEnded()
{
	return m_bHasEnded;
}

void Match::GeneralGameCycle()
{
	sf::Event event;

	while (m_rwWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_rwWindow.close();
		}

		if (!HasStarted())
		{
			// Pass the event to all the widgets
			m_cgGui.handleEvent(event);
		}
	}

	m_cbServer.Flush();
}

void Match::PreGameCycle()
{
#pragma region server -> gui

	Command serverCommand;

	while (m_cbServer.PollCommand(serverCommand))
	{
		switch (serverCommand.type)
		{
		case STREAM_COMMANDS::ADD:
		{
			int curveID = serverCommand.Attributes.Add.CurveID;
			int colorIndex = serverCommand.Attributes.Add.ColorIndex;
			bool local = serverCommand.Attributes.Add.Local;
			std::string name = serverCommand.Attributes.Add.Name;

			m_ccCurves.AddCurve(curveID, name, colorIndex, local);

			std::string rightKey = "";
			std::string leftKey = "";
			auto localCurves = m_ccCurves.getLocals();

			for (auto curve : localCurves)
			{
				if (curve->getID() == curveID)
				{
					auto keyPair = curve->GetKeys();

					rightKey = keyPair.RightName;
					leftKey = keyPair.LeftName;
				}
			}

			m_cgGui.AddPlayerPanel(name, Constants::CURVE_COLORS[colorIndex], leftKey, rightKey, local, curveID);

			break;
		}

		case STREAM_COMMANDS::REMOVE_CURVE:
		{
			int curveID = serverCommand.Attributes.Remove.CurveID;

			m_ccCurves.RemoveCurve(curveID);
			m_cgGui.RemovePlayerPanel(curveID);

			break;
		}
		case STREAM_COMMANDS::START:
		{
			Start();
			m_gScores.setPtsToReach(Constants::MIN_ROUNDS_TO_WIN * (m_ccCurves.getCurves().size() - 1));
			
			break;

		}
		/*case CommandType::KICK:
		{
		int curveID = serverCommand.Attributes.Kick.CurveID;

		m_ccCurves.RemoveCurve(curveID);
		m_cgGui.RemovePlayerPanel(curveID);

		break;
		}*/
		default:
			break;
		}
	}
#pragma endregion

#pragma region gui -> server
	tgui::Callback callback;
	while (m_cgGui.pollCallback(callback))
	{
		switch (ClientGui::GetEventId(callback.id))
		{
		case (int)GuiCommand::ADD_PLAYER:
		{
			m_cbServer.SendCAdd(m_cgGui.getTypedName(), false);

			break;
		}
		case (int)GuiCommand::ADD_BOT:
		{
			m_cbServer.SendCAdd(m_cgGui.getTypedName(), true);

			break;
		}
		case (int)GuiCommand::START:
		{
			m_cbServer.SendCStart();

			break;
		}
		case (int)GuiCommand::REMOVE:
		{
			m_cbServer.SendCRemove(ClientGui::GetPlayerId(callback.id));

			break;
		}
		case (int)GuiCommand::KICK:
		{
			m_cbServer.SendCKick(ClientGui::GetPlayerId(callback.id));

			break;
		}
		}
	}

#pragma endregion

	m_cgGui.SetAddButton();
}

void Match::InGameCycle()
{
	// Update absolute directions of local curves
	m_ccCurves.updateDirections();

	// Update game by commands of the server
	Command serverCommand;

	while (m_cbServer.PollCommand(serverCommand))
	{
		switch (serverCommand.type)
		{
		case STREAM_COMMANDS::POSITION:
		{
			int curveID = serverCommand.Attributes.Position.CurveID;
			float x = serverCommand.Attributes.Position.X;
			float y = serverCommand.Attributes.Position.Y;
			float z = serverCommand.Attributes.Position.Z;

			m_ccCurves[curveID].setPosition(sf::Vector3f(x, y, z));

			break;
		}

		case STREAM_COMMANDS::ROT_AXIS:
		{
			int curveID = serverCommand.Attributes.RotAxis.CurveID;
			float x = serverCommand.Attributes.RotAxis.X;
			float y = serverCommand.Attributes.RotAxis.Y;
			float z = serverCommand.Attributes.RotAxis.Z;

			m_ccCurves[curveID].setRotationAxis(sf::Vector3f(x, y, z));

			break;
		}

		case STREAM_COMMANDS::KILL:
		{
			int curveID = serverCommand.Attributes.Kill.CurveID;

			if (m_ccCurves[curveID].isAlive())
			{
				m_ccCurves[curveID].setAlive(false);

				for (ClientCurve* curve : m_ccCurves.getCurves())
				{
					if (curve->isAlive())
					{
						++(curve->points);
					}
				}
			}

			break;
		}

		case STREAM_COMMANDS::GOD_MOD:
		{
			int curveId = serverCommand.Attributes.GodMod.id;
			bool godmod = serverCommand.Attributes.GodMod.on;

			m_ccCurves[curveId].setGodMod(godmod);

			break;
		}
		case STREAM_COMMANDS::ROUND_WINNER:
		{
			int curveId = serverCommand.Attributes.RoundWinner.id;

			//m_ccCurves[curveId].setGodMod(godmod);

			break;
		}
		case STREAM_COMMANDS::RESTART:
		{
			m_cFollowedCurve = m_ccCurves.getLocals()[0];
			m_nCameraPauseTime = 0;
			m_ccCurves.Reset();

			int curves = m_ccCurves.getCurves().size();
			int min_points = Constants::MIN_ROUNDS_TO_WIN * (curves - 1);
			
			if (m_ccCurves.GetCurvesByScores()[0]->points < min_points)
			{
				m_gScores.setPtsToReach(Constants::MIN_ROUNDS_TO_WIN * (curves - 1));
			}
			else
			{
				m_gScores.setPtsToReach(m_ccCurves.GetCurvesByScores()[0]->points + 2);
			}

			break;
		}
		case STREAM_COMMANDS::WINNER:
		{
			int curveId = serverCommand.Attributes.Winner.id;

			m_gScores.setWinner(m_ccCurves[curveId]);
			m_bHasEnded = true;

			break;
		}
		case STREAM_COMMANDS::PREV_ANGLE:
		{
			int curveId = serverCommand.Attributes.PrevAngle.id;
			float angle = serverCommand.Attributes.PrevAngle.angle;

			m_ccCurves[curveId].previewAngle = angle;

			break;
		}
		case STREAM_COMMANDS::DEBUG:
		{
			/*int a = serverCommand.Attributes.Debug.a;
			int b = serverCommand.Attributes.Debug.b;
			int c = serverCommand.Attributes.Debug.c;
			int d = serverCommand.Attributes.Debug.d;
			int e = serverCommand.Attributes.Debug.e;
			int f = serverCommand.Attributes.Debug.f;*/

			/*m_gScores.m_strDebug =	std::to_string(a) +
							"   " + std::to_string(b) +
							"   " + std::to_string(c) +
							"   " + std::to_string(d) +
							"   " + std::to_string(e) +
							"   " + std::to_string(f);*/
			std::string debug = serverCommand.Attributes.Debug.debug;
			m_gScores.m_strDebug = debug;

			break;
		}
		default:
			break;
		}
	}

	// Send the server the absolute directions of local curves
	auto localCurves = m_ccCurves.getLocals();

	for (auto curve : localCurves)
	{
		if (curve->isDirectionChanged())
		{
			auto direction = curve->getAbsDirection();
			m_cbServer.SendCDirection(curve->getID(), direction);
		}
	}

	// Camera
	m_cCamera.Follow(m_cFollowedCurve->getPosition(), m_cFollowedCurve->getRotationAxis(), m_cFollowedCurve->previewAngle);

	if (m_nCameraPauseTime == 0)
	{
		if (!m_cFollowedCurve->isAlive())
		{
			m_cCamera.Bang();
			m_nCameraPauseTime = Constants::CAMERA_PAUSE_TIME;
		}
	}
	else if (m_nCameraPauseTime == 1)
	{
		for (auto* curve : m_ccCurves.getCurves())
		{
			if (curve->isAlive())
			{
				m_cFollowedCurve = curve;
				--m_nCameraPauseTime;

				break;
			}
		}
	}
	else
	{
		--m_nCameraPauseTime;
	}

	m_gScores.update();
}

bool Match::HasStarted()
{
	return m_bHasStarted;
}

void Match::Start()
{
	m_cFollowedCurve = m_ccCurves.getLocals()[0];

	m_gScores.init();
	m_bHasStarted = true;

	music.openFromFile("music.wav");
	music.setLoop(true);
	music.play();
}

void Match::Display()
{
	if (m_bHasStarted)
	{
		m_cCamera.Draw();
		m_gScores.draw();
	}
	else
	{
		m_cgGui.draw();
	}

	m_rwWindow.display();
	m_rwWindow.clear();
}