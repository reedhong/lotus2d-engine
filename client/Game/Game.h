/********************************************************************
	created:	2012/09/11
	filename: 	Game.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef __Game_H__
#define __Game_H__

#include "RenderSystem/Texture.h"
#include "RenderSystem/Sprite.h"
#include "Base/Types.h"

#include "Framework/Framework.h"
using namespace Lotus2d;

enum eGameState
{
	GS_START,		// 启动，加载，版本验证，资源更新等
	GS_LOGIN,    // 登录注册等
	GS_LOADING,	// 场景加载
	GS_ACTION,	// 主场景
	GS_BATTLE,	// 战斗
};



class Game: public Framework
{
public:
	Game();
	~Game();
	static Game * Instance(void);
public:
	virtual void gameStartup(int width,int height,float scale=1.0);
	virtual void gameFrame();
	virtual void gamePause();
	virtual void gameResume();
	virtual void gameEnd();
	virtual void addAction(eRawActionEventType event, int x, int y);
	virtual void addKey(unsigned char key);

	void setState(eGameState status);
	eGameState getState();
	void setProcess(int process){ m_progress = process;}
protected:
	void logic();
	void paint();
	/*
	 * 游戏启动流程 
	 */
	void gameStartProcedure();
	/*
	 * 游戏中场景的加载流程
	 */
	void loadingSceneProcedure();
	
private:
	uint64 m_tick;
	eGameState	m_state;
	int m_progress;		// 启动和场景加载的时候需要

};

#endif 
