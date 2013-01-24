/********************************************************************
	created:	2012/10/22
	filename: 	ScreenManager.h
	author:		reedhong
	
	purpose:	screen, 将游戏的分层，此概念偷学cocos2d， 
						Screen类似于其scene
*********************************************************************/

#ifndef __Screen_H__
#define __Screen_H__

#include <list>
using namespace std;

#include "AbstractLayer.h"

enum eLayerType {
	eUILayer = 0,
	eSceneLayer,
#ifdef LOTUS2D_DEBUG
	eTestLayer,
#endif
	LAYER_COUNT
};

class Screen
{
protected:
	Screen();
public:
	~Screen();
	static Screen* Instance();

	int handleEvent(eActionEventType event, int x, int y);
	void paint();
	void step();

	/*
	 * 把某种类型的screen压栈
	 */
	void pushLayer(eLayerType type);
	/*
	 * 弹出栈
	 */
	void popLayer();

	/*
	 * 清空栈
	 */
	void clearLayers();

	AbstractLayer* getLayer(eLayerType type);

	void setLayerVisible(eLayerType type, BOOL b);

protected:
	list<AbstractLayer *>	m_layerStack;
	AbstractLayer*	m_layers[LAYER_COUNT];
};

#endif