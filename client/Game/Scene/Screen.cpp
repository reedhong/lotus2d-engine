/********************************************************************
	created:	2012/10/22
	filename: 	Scene.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Base/Assert.h"

#include  "Screen.h"
#include "AllLayer.h"
#include "Camera.h"

Screen::Screen()
{
	for(int i = 0; i < LAYER_COUNT; i++){
		m_layers[i] = NULL;
	}
	m_layers[eUILayer] = new UILayer();
	m_layers[eSceneLayer] = new SceneLayer();
#ifdef LOTUS2D_DEBUG
	m_layers[eTestLayer] = new TestLayer();
#endif

}

Screen::~Screen()
{
	for(int i = 0; i < LAYER_COUNT; i++){
		delete m_layers[i];
	}
}

Screen* Screen::Instance()
{
	static Screen s_instance;
	return &s_instance;
}

void Screen::paint()
{
	list<AbstractLayer *>::iterator iter = m_layerStack.begin();
	for(;iter != m_layerStack.end(); ++iter){
		(*iter)->draw();
	}

}

void Screen::step()
{
	Camera::Instance()->step();

	list<AbstractLayer *>::iterator iter = m_layerStack.begin();
	for(;iter != m_layerStack.end(); ++iter){
		(*iter)->step();
	}
}

int Screen::handleEvent(eActionEventType event, int x, int y)
{
	// ∑¥œÚ∑√Œ 
	list<AbstractLayer *>::reverse_iterator  iter = m_layerStack.rbegin();
	int iRet=-1;
	for(;iter != m_layerStack.rend(); ++iter){
		iRet = (*iter)->handleEvent(event, x, y);
		if(!iRet) break;
	}

	return iRet;
}

void Screen::pushLayer(eLayerType type)
{
	AbstractLayer* layer = getLayer(type);
	layer->enter();
	m_layerStack.push_back(layer);
}

void Screen::popLayer()
{
	AbstractLayer* layer = m_layerStack.back();
	layer->leave();
	m_layerStack.pop_back();
}


void Screen::clearLayers()
{
	list<AbstractLayer *>::iterator iter = m_layerStack.begin();
	for(;iter != m_layerStack.end(); ++iter){
		(*iter)->leave();
	}
	// todo:
	//m_layerStack.clear();
}

AbstractLayer* Screen::getLayer(eLayerType type)
{
	ASSERT(0 <= type && type < LAYER_COUNT);
	return m_layers[type];
}

void Screen::setLayerVisible(eLayerType type, BOOL b)
{
	AbstractLayer* screen = getLayer(type);
	screen->setVisible(b);
}