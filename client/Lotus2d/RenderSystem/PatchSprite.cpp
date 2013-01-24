/********************************************************************
	created:	2012/11/26
	filename: 	PatchSprite.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "PatchSprite.h"
#include "Render.h"
#include "TinyXML/tinyxml.h"
#include "Base/FileStream.h"
#include "Resource/ResManagerBase.h"

namespace Lotus2d {
	Patch::~Patch()
	{
		vector<SpriteFModule* >::iterator iter = m_fmodules.begin();
		for(;iter != m_fmodules.end(); ++iter){
			delete (*iter);
		}
		m_fmodules.clear();
	}

	PatchSprite::PatchSprite(void)
	{

	}
	PatchSprite::~PatchSprite(void)
	{
		vector<Texture* >::iterator textureIter = m_textures.begin();
		for(; textureIter != m_textures.end(); ++textureIter){
			SAFE_RELEASE(*textureIter);
		}
		m_textures.clear();

		vector<Image* >::iterator imageIter = m_images.begin();
		for(; imageIter != m_images.end(); ++imageIter){
			delete(*imageIter);
		}
		m_images.clear();

		vector<SpriteSModule* >::iterator smoduleIter = m_smodules.begin();
		for(;smoduleIter != m_smodules.end(); ++smoduleIter){
			delete (*smoduleIter);
		}
		m_smodules.clear();

		vector<Patch * >::iterator patchIter = m_patches.begin();
		for(;patchIter != m_patches.end(); ++patchIter){
			delete (*patchIter);
		}
		m_patches.clear();
	}

	PatchSprite* PatchSprite::loadFromBinary(Stream* stream)
	{
		PatchSprite* sprite = new PatchSprite();
		if(sprite->parseBinary(stream)){
			return sprite;
		}else{
			SAFE_RELEASE(sprite);
			return NULL;
		}
	}


	bool PatchSprite::parseBinary(Stream* stream)
	{
		// texture
		int textureCount = stream->read8();
		for(int i = 0; i < textureCount; i++){
			unsigned int textureKey = (unsigned int)stream->read32le();
			Texture* texture = (Texture *)ResourceManagerBase::getInstance()->loadResouce(textureKey, eResTexture);
			m_textures.push_back(texture);
		}

		// modules
		int moduleCount = stream->read16le();
		for(int i = 0; i < moduleCount; i++){
			SpriteSModule* smodule = new SpriteSModule();
			smodule->m_x = stream->read16le();
			smodule->m_y = stream->read16le();
			smodule->m_width = stream->read16le();
			smodule->m_height = stream->read16le();
			smodule->m_imageIndex = stream->read8();
			m_smodules.push_back(smodule);
		}
		// patch
		int patchCount  = stream->read16le();
		for(int i = 0; i < patchCount; i++){
			Patch* patch= new Patch();
			patch->m_type = stream->read8();
			int fmoduleCount = stream->read16le();
			for(int j = 0; j < fmoduleCount; j++){
				SpriteFModule* fmodule = new SpriteFModule();
				fmodule->m_ox = stream->read16le();
				fmodule->m_oy = stream->read16le();
				fmodule->m_moduleIndex = stream->read16le();
				fmodule->m_transFlag = stream->read8();
				patch->m_fmodules.push_back(fmodule);
			}
			m_patches.push_back(patch);
		}
		loadImages();
		return true;
	}

	PatchSprite* PatchSprite::loadFromXML(const string& path)
	{
		PatchSprite* sprite = new PatchSprite();
		if(sprite->parseXML(path)){
			return sprite;
		}else{
			SAFE_RELEASE(sprite);
			return NULL;
		}
	}

	bool PatchSprite::parseXML(const string& path)
	{
		TiXmlNode* node = 0;
		TiXmlElement* element = 0;
		TiXmlElement* elem_images = 0;
		TiXmlDocument docImport(path.c_str());

		if(!docImport.LoadFile()) 
			return false;
		node = docImport.FirstChild("magicsprite");

		if(!node)
			return false;

		element = node->ToElement();
		char* char_version = (char*)element->Attribute("version");
		int version = 1; 
		if(char_version)
			version = atoi(char_version);
		if(NULL == element)
			return false;



		//load images
		elem_images = element->FirstChildElement("images");
		if(elem_images)
		{
			TiXmlElement* elem_imageItem = 0;
			elem_imageItem = elem_images->FirstChildElement();
			int imageIndex = 0;
			if(elem_imageItem)
			{
				//char *hexid,*file,*key,*bg;
				char* file;
				do
				{
					file = (char*)elem_imageItem->Attribute("file");
					if(!file)return FALSE;
					//char* imageName = Util::getFileNameNoExt(file);
					Texture* texture = (Texture *)ResourceManagerBase::getInstance()->loadResouce(path, eResTexture);
					m_textures.push_back(texture);
					//SAFE_DELETE(imageName);

				}while(elem_imageItem = elem_imageItem->NextSiblingElement());
			}
		}

		//load modules
		TiXmlElement* elem_modules = 0;
		elem_modules = element->FirstChildElement("modules");
		if(elem_modules)
		{
			TiXmlElement* elem_moduleItem = 0;
			elem_moduleItem = elem_modules->FirstChildElement();
			if(elem_moduleItem)
			{
				char *x,*y,*width,*height,*img;
				do
				{
					x = (char*)elem_moduleItem->Attribute("x");
					if(!x)return FALSE;

					y = (char*)elem_moduleItem->Attribute("y");
					if(!y)return FALSE;

					width  = (char*)elem_moduleItem->Attribute("width");
					if(!width)return FALSE;

					height  = (char*)elem_moduleItem->Attribute("height");
					if(!height)return FALSE;

					img  = (char*)elem_moduleItem->Attribute("img");
					if(!img)return FALSE;


					SpriteSModule* smodule = new SpriteSModule();
					smodule->m_x = atoi(x);
					smodule->m_y = atoi(y);
					smodule->m_width = atoi(width);
					smodule->m_height = atoi(height);
					smodule->m_imageIndex = atoi(img);
					m_smodules.push_back(smodule);

				}while(elem_moduleItem = elem_moduleItem->NextSiblingElement());
			}
		}


		//load magicbgs -->patch
		TiXmlElement* elem_patches = 0;
		elem_patches = element->FirstChildElement("magicbgs");
		if(elem_patches)
		{
			TiXmlElement* elem_patchItem = 0;
			elem_patchItem = elem_patches->FirstChildElement();
			if(elem_patchItem)
			{
				char *name;
				char *type;
				do
				{
					name  = (char*)elem_patchItem->Attribute("name");
					type = (char*)elem_patchItem->Attribute("type");
					if(!name)return FALSE;

					Patch* patch= new Patch();
					patch->m_name = name;
					patch->m_type = atoi(type);

					TiXmlElement* elem_fmdouleItem = 0;
					elem_fmdouleItem = elem_patchItem->FirstChildElement();

					if(elem_fmdouleItem)
					{
						char *module,*x,*y,*trans;
						do
						{
							module = (char*)elem_fmdouleItem->Attribute("moduleindex");
							if(!module)return FALSE;

							x = (char*)elem_fmdouleItem->Attribute("x");
							if(!x)return FALSE;

							y = (char*)elem_fmdouleItem->Attribute("y");
							if(!y)return FALSE;

							trans  = (char*)elem_fmdouleItem->Attribute("trans");
							if(!trans)return FALSE;

							SpriteFModule* fmodule = new SpriteFModule();
							fmodule->m_ox = atoi(x);
							fmodule->m_oy = atoi(y);
							fmodule->m_moduleIndex = atoi(module);
							fmodule->m_transFlag = atoi(trans);
							patch->m_fmodules.push_back(fmodule);
						}while(elem_fmdouleItem = elem_fmdouleItem->NextSiblingElement());
					}
					m_patches.push_back(patch);
				}while(elem_patchItem = elem_patchItem->NextSiblingElement());
			}
		}

		loadImages();
		return true;
	}

	void PatchSprite::loadImages()
	{
		vector<SpriteSModule* >::iterator iter = m_smodules.begin();
		SpriteSModule* smodule = NULL;
		for(;iter != m_smodules.end(); ++iter){
			smodule = *iter;
			Texture* texture = m_textures[smodule->m_imageIndex];
			Image* image = new Image(texture, smodule->m_x,
				smodule->m_y, smodule->m_width, smodule->m_height);
			m_images.push_back(image);
		}
	}

	void PatchSprite::drawFMoudle(SpriteFModule* fModule, int x, int y,bool flipX,bool flipY,float scale,int align)
	{
		ASSERT(fModule);
		int posx = 0;
		int posy = 0;
		unsigned int module_index = (unsigned int)fModule->m_moduleIndex;
		if(module_index>=0 && module_index < m_smodules.size()){
			Image* image = m_images[module_index];
			int trans = fModule->m_transFlag;
			bool rot = trans > TRANS_ROT180;

			posx = 0;
			posy = 0;

			int moduleW = (int)image->m_width;
			int moduleH = (int)image->m_height;


			/** 计算帧反转后module的信息 */
			if (flipX) {
				if(rot){
					posx =  - moduleH;
					trans ^= TRANS_MIRROR_ROT180 ;

				}else{
					posx =  - moduleW;
					trans ^= TRANS_MIRROR ;
				}
			}
			if (flipY) {
				if(rot){
					posy =  - moduleW;
					trans ^= TRANS_MIRROR ;
				}else{
					posy =  - moduleH;
					trans ^= TRANS_MIRROR_ROT180 ;
				}
			}

			int dx = x + posx ;
			int dy = y + posy ;

			switch (align)
			{
			default:
			case MAGIC_ALIGN_TOPLEFT:

				break;
			case MAGIC_ALIGN_TOPCENETER:
				dx-= rot?(moduleH>>1):(moduleW>>1);
				break;
			case MAGIC_ALIGN_TOPRIGHT:
				dx-= rot?(moduleH):(moduleW);
				break;
			case MAGIC_ALIGN_MIDDLELEFT:
				dy-= rot?(moduleW >> 1):(moduleH >> 1);
				break;
			case MAGIC_ALIGN_MIDDLECENTER:
				dx-= rot?(moduleH>>1):(moduleW>>1);
				dy-= rot?(moduleW >> 1):(moduleH >> 1);
				break;
			case MAGIC_ALIGN_MIDDLERIGHT:
				dx-= rot?(moduleH):(moduleW);
				dy-= rot?(moduleW >> 1):(moduleH >> 1);
				break;
			case MAGIC_ALIGN_BOTTOMLEFT:
				dy-= rot?(moduleW):(moduleH);
				break;
			case MAGIC_ALIGN_BOTTOMCENTER:
				dx-= rot?(moduleH>>1):(moduleW>>1);
				dy-= rot?(moduleW):(moduleH);
				break;
			case MAGIC_ALIGN_BOTTOMRIGHT:
				dx-= rot?(moduleH):(moduleW);
				dy-= rot?(moduleW):(moduleH);	
				break;

			}
			Render::Instance()->drawImage(image,(float)dx, (float)dy ,trans,0);
			//Render::Instance()->drawImage(image,dx, dy +  (rot?(moduleW):(moduleH)),trans,0);
		}
	}

	void PatchSprite::drawPatch(Patch* patch, int x, int y,int w, int h,bool drawMinW,bool flipX,bool flipY ,float scale)
	{
		ASSERT(patch);
		int leftW=0,rightW=0,middleW=0,middleH=0,topH=0,bottomH=0;
		int patchType = patch->m_type;
		if (patchType == ePatch9){
			getBorderSize(patch,&leftW,&rightW,&middleW,&middleH,&topH,&bottomH);

			drawPatch3X(patch, 0, x, y,w, topH);

			int extH = h - topH - bottomH;
			int middleCount = extH / middleH;
			int ext = extH % middleH;

			for (int i=0;i<middleCount;i++){
				drawPatch3X(patch, 1, x, y + topH + i * middleH,w, middleH);
			}

			drawPatch3X(patch, 1, x, y + topH + middleCount * middleH,w, ext);
			drawPatch3X(patch, 2, x, y + h - bottomH,w, bottomH);
		}else if (patchType == ePatch3X){
			drawPatch3X(patch, 0, x, y,w, h);
		}else if (patchType == ePatch3Y){
			drawPatch3Y(patch, 0, x, y,w, h);
		}

		// todo:
		Render::Instance()->end();
	}
	void PatchSprite::drawPatch(unsigned int index, int x, int y,int w, int h,bool drawMinW,bool flipX,bool flipY,float scale)
	{
		if ( 0<= index && index < m_patches.size()){
			drawPatch(m_patches[index], x, y,w, h,drawMinW,flipX,flipY,scale);
		}
	}


	void PatchSprite::drawPatch3X(Patch* patch, unsigned int startIndex, int x, int y,int w, int h,bool drawMinW)
	{
		//int clipX,clipY,clipW,clipH;

		// todo: 暂时不用这个
		//glGetClipArea(&clipX, &clipY, &clipW, &clipH);
		int leftW=0,rightW=0,middleW=0,height=0;;
		if (patch && (patch->m_fmodules.size()>=((startIndex +1) * 3))){
			int magicType = patch->m_type;
			getLineWidth(patch,startIndex,&leftW,&middleW,&rightW,&height);
			if (drawMinW && w < leftW + rightW){
				w = leftW + rightW;
			}

			SpriteFModule* fmodule = patch->m_fmodules[startIndex * 3];
			int realheight = height < h?height:h;

			Render::Instance()->setClip(x, y,leftW,realheight);

			drawFMoudle(fmodule, x + leftW, y + (startIndex == 2?0:realheight),false,false,1.0f,startIndex == 2? MAGIC_ALIGN_TOPRIGHT : MAGIC_ALIGN_BOTTOMRIGHT);

			Render::Instance()->disableClip();

			int extW = w - leftW - rightW;
			int middleCount = extW / middleW;
			int ext = extW % middleW;

		
			Render::Instance()->setClip(x + leftW, y,extW,realheight);
			for (int i=0;i<middleCount + 1;i++){
#if 1
				fmodule = patch->m_fmodules[startIndex * 3 + 1];
				drawFMoudle(fmodule, x + leftW + i * middleW, y + (startIndex == 2?0:realheight),FALSE,FALSE,1.0f,startIndex == 2? MAGIC_ALIGN_TOPLEFT : MAGIC_ALIGN_BOTTOMLEFT);
#endif
			}
			Render::Instance()->disableClip();

			fmodule = patch->m_fmodules[startIndex * 3 + 2];

			Render::Instance()->setClip(x + w - rightW, y,rightW,realheight);
			drawFMoudle(fmodule, x + w - rightW, y + (startIndex == 2?0:realheight),FALSE,FALSE,1.0f,startIndex == 2? MAGIC_ALIGN_TOPLEFT : MAGIC_ALIGN_BOTTOMLEFT);
			Render::Instance()->disableClip();

		}
	}

	void PatchSprite::drawPatch3Y(Patch* patch, unsigned int startIndex, int x, int y,int w, int h,bool drawMinW)
	{
		//int clipX,clipY,clipW,clipH;

		//glGetClipArea(&clipX, &clipY, &clipW, &clipH);
		int leftW=0,rightW=0,middleW=0,height=0;;
		if (patch && (patch->m_fmodules.size()>=((startIndex +1) * 3))){

			int patchType = patch->m_type;
			getLineWidth(patch,startIndex,&leftW,&middleW,&rightW,&height);
			if (drawMinW && w < leftW + rightW)
			{
				h = leftW + rightW;
			}

			SpriteFModule* fmodule = patch->m_fmodules[startIndex * 3];
			int realheight = height < w?height:w;


			Render::Instance()->setClip(x, y,realheight,leftW);
			drawFMoudle(fmodule, x, y + leftW,FALSE,FALSE,1.0f,MAGIC_ALIGN_BOTTOMLEFT);
			Render::Instance()->disableClip();

			int extW = h - leftW - rightW;
			int middleCount = extW / middleW;
			int ext = extW % middleW;


			Render::Instance()->setClip(x, y + leftW,w,extW);
			for (int i=0;i<middleCount + 1;i++)
			{

				fmodule = patch->m_fmodules[startIndex * 3 + 1];
				drawFMoudle(fmodule, x, y + leftW + i * middleW,FALSE,FALSE,1.0f,MAGIC_ALIGN_TOPLEFT);
			}
			Render::Instance()->disableClip();

			fmodule = patch->m_fmodules[startIndex * 3 + 2];


			Render::Instance()->setClip(x, y + h - rightW,realheight,rightW);
			drawFMoudle(fmodule, x , y + h - rightW,FALSE,FALSE,1.0f,MAGIC_ALIGN_TOPLEFT);
			Render::Instance()->disableClip();
		}
	}

	bool PatchSprite::getBorderSize(Patch* patch,int* leftw, int* rightw,int* middlew,int* middleh,int* toph,int* bottomh)
	{
		ASSERT(patch);
		*leftw = 0;
		*rightw = 0;
		*toph = 0;
		*bottomh = 0;
		*middlew = 0;
		*middleh = 0;

		int totalW = 0;
		int totalH = 0;
		SpriteFModule* fmodule = NULL;
		Image* image = NULL;
		unsigned int module_index;
		int mw,mh;

		int trans = 0;
		int wleft = 0,wright = 0,wmiddle=0,hmiddle=0,htop = 0,hbottom = 0;
		for (unsigned int i = 0; i < patch->m_fmodules.size(); i++){
			fmodule = 	patch->m_fmodules[i];
			module_index = fmodule->m_moduleIndex;
			if (module_index<0 || module_index >= m_smodules.size()){
				return false;
			}
			image = m_images[module_index];

			trans = fmodule->m_transFlag;
			/** 获得静态module所对应的图片 */
			bool rot = trans > TRANS_ROT180;
			/** 获得静态module在图片上的区域位置信息 */
			mw = (int)(rot?image->m_height:image->m_width);
			mh = (int)(rot?image->m_width:image->m_height);
			if (i<3){		// 0, 1, 2
				if (htop < mh){
					htop = mh;
				}

			}else if (5<i && i<9){ // 6, 7, 8
				if (hbottom < mh){
					hbottom = mh;
				}
			}

			if (i == 0 || i == 3 || i == 6){
				if (wleft < mw){
					wleft = mw;
				}
			}else if (i == 2 || i == 5 || i == 8){
				if (wright < mw){
					wright = mw;
				}
			}else if (i==4){
				wmiddle = mw;
				hmiddle = mh;
			}
		}

		*leftw = wleft;
		*rightw = wright;
		*middlew = wmiddle;
		*middleh = hmiddle;
		*toph = htop;
		*bottomh = hbottom;

		return true;
	}

	bool PatchSprite::getLineWidth(Patch* patch,unsigned int lineIndex,int* leftw,int* middlew, int* rightw,int* height)
	{
		*leftw = 0;
		*rightw = 0;
		*middlew = 0;
		*height = 0;

		if (patch && ((lineIndex + 1) * 3 <= patch->m_fmodules.size())){
			int patchType = patch->m_type; 
			SpriteFModule* fmodule = NULL;
			Image* image = NULL;
			unsigned int module_index;
			int mw,mh;
			int trans = 0;
			int h = 0;
			for (unsigned int i = lineIndex * 3; i < (lineIndex + 1) * 3; i++)
			{
				fmodule = 	patch->m_fmodules[i];
				module_index = fmodule->m_moduleIndex;
				if (module_index<0 || module_index >= m_images.size()){
					return false;
				}

				image = m_images[module_index];

				trans = fmodule->m_transFlag;
				/** 获得静态module所对应的图片 */
				bool rot = trans > TRANS_ROT180;
				/** 获得静态module在图片上的区域位置信息 */
				mw = (int)(rot?image->m_height:image->m_width);
				mh = (int)(rot?image->m_width:image->m_height);

				if (patchType == ePatch9 || patchType == ePatch3X){
					if (i == lineIndex * 3){
						*leftw = mw;
					}else if (i == lineIndex * 3 + 1){
						*middlew = mw;
					}else if (i == lineIndex * 3 + 2){
						*rightw = mw;
					}

					if (h < mh){
						h = mh;
					}
				}
				else if (patchType == ePatch3Y)
				{
					if (i == lineIndex * 3){
						*leftw = mh;
					}else if (i == lineIndex * 3 + 1){
						*middlew = mh;
					}else if (i == lineIndex * 3 + 2)
					{
						*rightw = mh;
					}

					if (h < mw){
						h = mw;
					}
				}

			}
			*height = h;
			return true;
		}
		return false;
	}
}