/********************************************************************
	created:	2012/10/23
	filename: 	ResManager.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/
#include "ResManager.h"
#include "TinyXML/tinyxml.h"
#include "Base/FileStream.h"
#include "RenderSystem/Sprite.h"
#include "RenderSystem/PatchSprite.h"
#include "Constants/GameConfig.h"
using namespace Lotus2d;




ResManager::ResManager()
{
}

ResManager::~ResManager()
{
	clearSprite();
}


ResManager* ResManager::Instance()
{
	static ResManager instance;
	setInstance(&instance);
	return &instance;
}

const char* ResManager::getTarget()
{
	return "480x800";
}



bool ResManager::init(const string& resPath)
{
	loadStrings();
	return true;
}

void ResManager::loadStrings()
{
	string path = string(RESOURCE_PATH) + string("strings");
	FileStream stream(path.c_str(), "rb");
	int count = stream.read16le();
	for(int i = 0; i < count ; i++){
		UString str = stream.readUString();
		m_strings.push_back(str);
	}
}

const UString& ResManager::getString(size_t index) const
{
	ASSERT(index < m_strings.size());
	return  m_strings[index];
}


void ResManager::clearSprite()
{
	map<string, Sprite*>::iterator iter = mSpritesCache.begin();
	for(; iter != mSpritesCache.end(); ++iter){
		SAFE_RELEASE(iter->second);
	}
	mSpritesCache.clear();
}

Page*  ResManager::getPage(unsigned int key)
{
	return NULL;
}


Resource* ResManager::loadResouce(unsigned int key, eResType type)
{
	return NULL;
}

Resource* ResManager::loadResouce(const string& path, eResType type)
{
	return NULL;
}

void ResManager::releaseResouce(Resource*  resource)
{

}