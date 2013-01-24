/********************************************************************
	created:	2012/09/18
	filename: 	Sprite.h
	author:		reedhong
	
	purpose:	TODO: 还可以做两件事情：
						1.内存池分配；
						2.不保留状态把状态的维护放在关联的对象上。暂时如此
*********************************************************************/

#ifndef __Lotus2d_Sprite_H__
#define  __Lotus2d_Sprite_H__

#include "Base/Prerequisites.h"
#include "Base/ObjectBase.h"
#include "RenderSystem/Texture.h"
#include "RenderSystem/Image.h"
#include "Resource/Resource.h"

namespace Lotus2d {
	
	struct SpriteSModule: public ObjectBase
	{
		short	m_x;
		short	m_y;
		short	m_width;
		short	m_height;
		short	m_imageIndex;
	};

	struct SpriteFModule: public ObjectBase
	{
		short	m_ox;
		short	m_oy;
		short	m_moduleIndex;
		short	m_transFlag; // 翻转模式
	};

	class SpriteSFrame: public ObjectBase
	{
	public:
		SpriteSFrame(string name);
		~SpriteSFrame();
	public:
		string	m_name;
		vector<SpriteFModule *> m_fmodules;
	};


	struct SpriteAFrame: public ObjectBase
	{
		short m_frameIndex;
		short m_fx;
		short m_fy;
		short m_times;
		short m_flipFlag;
		bool m_isVisible;
	};

	class SpriteAction: public ObjectBase
	{
	public:
		SpriteAction();
		~SpriteAction();
	public:
		int m_id;
		string	m_name;
		string m_key;
		vector<SpriteAFrame *> m_aframes;
	};

	/** 
	* Active zone
	*/
	struct ActiveZone{
		short    x;
		short    y;
		short    w;
		short    h;
	};

	class Sprite: public Resource
	{
	protected:
		Sprite(void);
		virtual ~Sprite(void);
	public:
		static Sprite*  loadFromXML(const string& path);
		static Sprite* loadFromBinary(Stream* steam);
	protected:
		bool parseXML(const string& path);
		bool parseBinary(Stream* steam);
	public:
		int getActionCount();
		int getSFrameCount();
		string getActionName(unsigned int index);
	public: // 绘制
		/*
		 * @frameIndex: 静态帧的索引
		 */
		void drawSFrame(size_t frameIndex,  int x, int y, BOOL flipX=FALSE, BOOL flipY=FALSE, int anchor=0,
			 int aframe_fx=0, int aframe_fy=0);

		/*
		 * @actionIndex: action索引
		 * @frameIndex:  某个动作中的动态帧索引号
		 */
		void drawAction(size_t actionIndex, size_t frameIndex, int x, int y, BOOL flipX=FALSE, BOOL flipY=FALSE, int anchor=0);
		// todo:
		//void drawFModule(SpriteFModule* fmodule);
		void loadImages();

		void getFrameWH(size_t frameindex, int* x, int* y, int* w, int* h) ;
		void getActionWH(size_t actionindex, int* x, int* y, int* w, int* h) ;
	public:
		// 相关数据
		vector<Texture* >	m_textures;
		vector<Image * > m_images;
		vector<SpriteSModule *>	m_smodules;		
		vector<SpriteSFrame *> m_sframes;			
		vector<SpriteAction* > m_actions;

		ActiveZone	m_activeZone;
	};
}

#endif
