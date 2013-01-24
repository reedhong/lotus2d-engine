/********************************************************************
	created:	2012/09/11
	filename: 	Game.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/
#include "RenderSystem/Render.h"
#include "Base/Timer.h"
#include "Math/MD5.h"
#include "Png/Png.h"
#include "Base/FileStream.h"
#include "Font/FontManager.h"
#include "Audio/AudioEngine.h"
#include "Scene/Screen.h"
#include "EventManager.h"
#include "ResManager/ResManager.h"
#include "Constants/GameConfig.h"
#include "Network/NetworkManager.h"
#include "Network/GameProtocol.h"
#include "UIEngine/UIEngine.h"
#include "Platform/textfield.h"
#include "EventManager.h"

#include "Generate/UI.h"
#include "Generate/R.h"
#include "Game.h"
#include "Scene/AllLayer.h"
#include "UI/UIManager.h"


Game::Game():m_tick(0)
{
	m_progress = 0;
}


Game::~Game()
{

}

Game * Game::Instance(void)
{
	static Game s_game;
	return &s_game;
}


void Game::gameStartup(int width,int height,float scale)
{
	Render::Instance()->init(width, height, scale);	
	//AudioEngine::Instance()->setResource("");
	string fontPath = string(RESOURCE_PATH)+string("font");
	LOGD("in Game startup, fontPath %s", fontPath.c_str());
	FontManager::Instance()->load(fontPath.c_str());
	FontManager::Instance()->createFont(MIDDLE_FONT, FONT_CACHE_IMAGE_256x256, 0, FALSE);
	FontManager::Instance()->setFont(MIDDLE_FONT);
	ResManager::Instance()->init(RESOURCE_PATH);
	NetworkManager::Instance()->init();

	// 设置到UIEngine中，免得把应用层逻辑耦合到引擎层
	UIEngine::Instance()->setCallback(UIManager::s_handleCommandCallback);
	setState(GS_START);
}

void Game::gameFrame()
{
	logic();
	paint();
}

void Game::gamePause()
{

}

void Game::gameResume()
{

}

void Game::gameEnd()
{
	exit(0);
}


void Game::addAction(eRawActionEventType event, int x, int y)
{
	EventManager::Instance()->addAction(event, x, y);
}

void Game::addKey(unsigned char key)
{
	lotus2d_inputkey(key);
}


void Game::setState(eGameState status)
{
	// 根据游戏状态的变迁，修改ScreenManager的状态
	switch(status){
	case GS_START:
		{
			UILayer* bl = (UILayer*)Screen::Instance()->getLayer(eUILayer);
			bl->setCurrentPage(UI::login::PGAE_ID);
			Screen::Instance()->pushLayer(eUILayer);
			//NetworkManager::Instance()->connectLoginServer(LOGIN_SERVER_ADDRESS, LOGIN_SERVER_PORT);
		}
		break;
	case GS_LOGIN:
		{
			UILayer* bl = (UILayer*)Screen::Instance()->getLayer(eUILayer);
			bl->setCurrentPage(UI::login::PGAE_ID);
			//Screen::Instance()->pushLayer(eUILayer);
		}
		break;
	case GS_LOADING:
		{
			UILayer* bl = (UILayer*)Screen::Instance()->getLayer(eUILayer);
			bl->setCurrentPage(UI::loading::PGAE_ID);
		}
		break;
	case GS_ACTION:
		{
			Screen::Instance()->clearLayers();
			Screen::Instance()->pushLayer(eSceneLayer);
		}
		break;
	case GS_BATTLE:
		{

		}
	default:
		break;
	}
	m_state = status;
}

eGameState Game::getState()
{
	return m_state;
}

void Game::logic()
{
	eGameState state = getState();
	if(state == GS_START){
		gameStartProcedure();
	}else if( state == GS_LOADING){
		loadingSceneProcedure();
	}
	m_tick++;	
	NetworkManager::Instance()->step();
	EventManager::Instance()->step();
	Screen::Instance()->step();
}


void Game::gameStartProcedure()
{
	switch(m_progress){
	case 0:
		m_progress = 10;
		break;
	case 10:
		{
	
		}
		break;
	case 30:
		{

		}
	case 80:
		m_progress = 99;
		break;
	case 99:
		{
			m_progress = 99;
			setState(GS_LOADING);
		}
		break;
	default:
		break;
		
	}
}


void Game::loadingSceneProcedure()
{
	switch(m_progress){
	case 0:
		m_progress = 10;
		break;
	case 10:
		m_progress = 30;
		break;
	case 30:
		m_progress = 80;
	case 80:
		m_progress = 99;
		break;
	case 99:
		m_progress = 0;
		setState(GS_ACTION);
		break;
	default:
		break;
	}
}


void Game::paint()
{
	Render::Instance()->clearScreen(GL_COLOR_WHITE);
	Screen::Instance()->paint();

	// 这个很关键，保证此帧中的数据均提交给显卡
	Render::Instance()->end();
}