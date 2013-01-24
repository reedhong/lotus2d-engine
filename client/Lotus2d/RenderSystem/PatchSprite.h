/********************************************************************
	created:	2012/11/26
	filename: 	PatchSprite.h
	author:		reedhong
	
	purpose:	九宫格样式Sprite绘制
*********************************************************************/

#ifndef __Lotus2d_PatchSprite_H__
#define __Lotus2d_PatchSprite_H__

#include "Resource/Resource.h"
#include "Base/ObjectBase.h"
#include "Sprite.h"

namespace Lotus2d{

	enum ePatchType{
		ePatch9 = 0,	
		ePatch3X,
		ePatch3Y,
	};

#define MAGIC_ALIGN_TOPLEFT				0
#define MAGIC_ALIGN_TOPCENETER			1
#define MAGIC_ALIGN_TOPRIGHT			2
#define MAGIC_ALIGN_MIDDLELEFT			3
#define MAGIC_ALIGN_MIDDLECENTER		4
#define MAGIC_ALIGN_MIDDLERIGHT			5
#define MAGIC_ALIGN_BOTTOMLEFT			6
#define MAGIC_ALIGN_BOTTOMCENTER		7
#define MAGIC_ALIGN_BOTTOMRIGHT			8

	class Patch: public ObjectBase
	{
	public:
		Patch(){}
		~Patch();
	public:
		string	m_name;
		char m_type;
		vector<SpriteFModule *> m_fmodules;
	};

	class PatchSprite: public Resource
	{
	protected:
		PatchSprite(void);
		~PatchSprite(void);
	public:
		static PatchSprite*  loadFromXML(const string& path);
		static PatchSprite* loadFromBinary(Stream* stream);
	protected:
		bool parseXML(const string& path);
		bool parseBinary(Stream* stream);

		void loadImages();
	public:
		void drawFMoudle(SpriteFModule* fModule, int x, int y,bool flipX = false,bool flipY = false,float scale = 1.0f,int align = MAGIC_ALIGN_TOPLEFT);

		void drawPatch(Patch* patch, int x, int y,int w, int h,bool drawMinW = false,bool flipX = false,bool flipY = false,float scale = 1.0f);

		void drawPatch(unsigned int patchIndex, int x, int y,int w, int h,bool drawMinW = false,bool flipX = false,bool flipY = false,float scale = 1.0f);

		void drawPatch3X(Patch* patch, unsigned int startIndex, int x, int y,int w, int h,bool drawMinW = false);

		void drawPatch3Y(Patch* patch, unsigned int startIndex, int x, int y,int w, int h,bool drawMinW = false);

		////////////////////////////////////////////////////////////////////////// utils
		bool getBorderSize(Patch* patch,int* leftw, int* rightw,int* middlew,int* middleh,int* toph,int* bottomh);
		bool getLineWidth(Patch* patch,unsigned int lineIndex,int* leftw,int* middlew, int* rightw,int* height);

	public:
		// 相关数据
		vector<Texture* >	m_textures;
		vector<Image * > m_images;
		vector<SpriteSModule *>	m_smodules;
		vector<Patch *> m_patches;
	};


}
#endif