/********************************************************************
	created:	2012/10/23
	filename: 	ResManager.h
	author:		reedhong
	
	purpose:	暂时直接读取资源
*********************************************************************/

#include <string>
#include <vector>
#include <map>
using namespace std;

#include "RenderSystem/Sprite.h"
#include "RenderSystem/Texture.h"
#include "Resource/ResManagerBase.h"
#include "UIEngine/Page.h"
using namespace Lotus2d;


class ResManager: public ResourceManagerBase
{
public:
	static ResManager* Instance();
	/*
	 * @binaryPath: 资源的二进制数据
	 * @xmlPath: 资源数据的路径  
	 */
	bool init(const string& resPath);
	void clearSprite();


	// 根据资源的key，加载资源
	virtual Resource* loadResouce(unsigned int key, eResType type);
	// 根据资源的路径，加载资源
	virtual Resource* loadResouce(const string& path, eResType type);
	virtual void releaseResouce(Resource*  resource);

	Sprite* loadSprite(const string& name);
	string getScenePath(const string& name);

	const UString& getString(size_t index) const;
	
	Page* getPage(unsigned int key);
protected:
	void loadStrings();
	const char* getTarget();

protected:
	ResManager();
	~ResManager();

public:

	map<string, Sprite*>	mSpritesCache;	// action map, 用于分配和回收
	vector<UString >			m_strings;
};
