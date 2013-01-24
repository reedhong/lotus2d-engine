/********************************************************************
	created:	2012/09/18
	filename: 	Sprite.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Sprite.h"
#include "Render.h"
#include "TinyXML/tinyxml.h"
#include "Resource/ResManagerBase.h"
#include "Base/FileStream.h"

namespace Lotus2d {
	SpriteSFrame::SpriteSFrame(string name)
	{
		m_name = name;
	}

	SpriteSFrame::~SpriteSFrame()
	{
		vector<SpriteFModule* >::iterator iter = m_fmodules.begin();
		for(;iter != m_fmodules.end(); ++iter){
			delete (*iter);
		}
		m_fmodules.clear();
	}

	SpriteAction::SpriteAction()
	{

	}

	SpriteAction::~SpriteAction()
	{
		vector<SpriteAFrame* >::iterator iter = m_aframes.begin();
		for(;iter != m_aframes.end(); ++iter){
			delete (*iter);
		}
		m_aframes.clear();
	}

	Sprite::Sprite(void)
	{
	}

	Sprite::~Sprite(void)
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

		vector<SpriteSFrame* >::iterator sframeIter = m_sframes.begin();
		for(;sframeIter != m_sframes.end(); ++sframeIter){
			delete (*sframeIter);
		}
		m_sframes.clear();

		vector<SpriteAction* >::iterator actionIter = m_actions.begin();
		for(;actionIter != m_actions.end(); ++actionIter){
			delete (*actionIter);
		}
		m_actions.clear();
	}

	Sprite* Sprite::loadFromBinary(Stream* stream)
	{
		Sprite* sprite = new Sprite();
		if(sprite->parseBinary(stream)){
			return sprite;
		}else{
			SAFE_RELEASE(sprite);
			return NULL;
		}
	}

	bool Sprite::parseBinary(Stream* stream)
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
	
		// frames
		int frameCount  = stream->read16le();
		for(int i = 0; i < frameCount; i++){
			SpriteSFrame* sframe= new SpriteSFrame("");
			int fmoduleCount = stream->read16le();
			for(int j = 0; j < fmoduleCount; j++){
				SpriteFModule* fmodule = new SpriteFModule();
				fmodule->m_ox = stream->read16le();
				fmodule->m_oy = stream->read16le();
				fmodule->m_moduleIndex = stream->read16le();
				fmodule->m_transFlag = stream->read8();
				sframe->m_fmodules.push_back(fmodule);
			}
			m_sframes.push_back(sframe);
		}

		// actions
		m_activeZone.x = stream->read16le();
		m_activeZone.y = stream->read16le();
		m_activeZone.w =stream->read16le();
		m_activeZone.h = stream->read16le();

		int actionCount = stream->read16le();
		for(int i = 0; i < actionCount; i++){
			// todo: id
			SpriteAction* action = new SpriteAction();
			int aframeCount  = stream->read16le();
			for(int j  = 0; j < aframeCount; j++){
				SpriteAFrame* aframe = new SpriteAFrame();
				aframe->m_frameIndex = stream->read16le();
				aframe->m_fx = stream->read16le();
				aframe->m_fy = stream->read16le();
				aframe->m_times = stream->read16le();
				aframe->m_flipFlag = stream->read8();
				action->m_aframes.push_back(aframe);
			}
			m_actions.push_back(action);
		}

		loadImages();

		return true;
	}

	Sprite* Sprite::loadFromXML(const string& path)
	{
		Sprite* sprite = new Sprite();
		if(sprite->parseXML(path)){
			return sprite;
		}else{
			SAFE_RELEASE(sprite);
			return NULL;
		}
	}

	bool Sprite::parseXML(const string& path)
	{
		TiXmlNode* node = 0;
		TiXmlElement* element = 0;
		TiXmlElement* elem_images = 0;
		TiXmlDocument docImport(path.c_str());

		if(!docImport.LoadFile()) 
			return false;
		node = docImport.FirstChild("sprite");

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
					if(!file)return false;
					//char* imageName = Util::getFileNameNoExt(file);
					Texture* texture = (Texture *)ResourceManagerBase::getInstance()->loadResouce(file, eResTexture);
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
					if(!x)return false;

					y = (char*)elem_moduleItem->Attribute("y");
					if(!y)return false;

					width  = (char*)elem_moduleItem->Attribute("width");
					if(!width)return false;

					height  = (char*)elem_moduleItem->Attribute("height");
					if(!height)return false;

					img  = (char*)elem_moduleItem->Attribute("img");
					if(!img)return false;


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


		//loadframes
		TiXmlElement* elem_frames = 0;
		elem_frames = element->FirstChildElement("frames");
		if(elem_frames)
		{
			TiXmlElement* elem_frameItem = 0;
			elem_frameItem = elem_frames->FirstChildElement();
			if(elem_frameItem)
			{
				char *name;
				do
				{
					name  = (char*)elem_frameItem->Attribute("name");
					if(!name)return false;

					SpriteSFrame* sframe= new SpriteSFrame(name);

					TiXmlElement* elem_fmdouleItem = 0;
					elem_fmdouleItem = elem_frameItem->FirstChildElement();
				
					if(elem_fmdouleItem)
					{
						char *module,*x,*y,*trans;
						do
						{
							module = (char*)elem_fmdouleItem->Attribute("module");
							if(!module)return false;

							x = (char*)elem_fmdouleItem->Attribute("x");
							if(!x)return false;

							y = (char*)elem_fmdouleItem->Attribute("y");
							if(!y)return false;

							trans  = (char*)elem_fmdouleItem->Attribute("trans");
							if(!trans)return false;

							SpriteFModule* fmodule = new SpriteFModule();
							fmodule->m_ox = atoi(x);
							fmodule->m_oy = atoi(y);
							fmodule->m_moduleIndex = atoi(module);
							fmodule->m_transFlag = atoi(trans);
							sframe->m_fmodules.push_back(fmodule);
						}while(elem_fmdouleItem = elem_fmdouleItem->NextSiblingElement());
					}
					m_sframes.push_back(sframe);
				}while(elem_frameItem = elem_frameItem->NextSiblingElement());
			}
		}
		//end load frames

		//load actions
		TiXmlElement* elem_actions = 0;
		elem_actions = element->FirstChildElement("actions");
		if(elem_actions)
		{
			m_activeZone.x = atoi(elem_actions->Attribute("bound_x"));
			m_activeZone.y = atoi(elem_actions->Attribute("bound_y"));
			m_activeZone.w = atoi(elem_actions->Attribute("kzone_w"));
			m_activeZone.h = atoi(elem_actions->Attribute("kzone_h"));
			TiXmlElement* elem_actionItem = 0;
			elem_actionItem = elem_actions->FirstChildElement();
			if(elem_actionItem)
			{
				char *id,*name;//,*key,*px,*py;
				int actionid = 0;
				do
				{
					id  = (char*)elem_actionItem->Attribute("id");
					name = (char*)elem_actionItem->Attribute("name");
					if(!name)return false;

					SpriteAction* action = new SpriteAction();

					action->m_id = atoi(id);
					action->m_name = name;
					action->m_key   = (char*)elem_actionItem->Attribute("key");
					//action->m_px  = (char*)elem_actionItem->Attribute("px");
					//action->m_py  = (char*)elem_actionItem->Attribute("py");
					
					m_actions.push_back(action);

					TiXmlElement* elem_aframeItem = 0;
					elem_aframeItem = elem_actionItem->FirstChildElement();
					if(elem_aframeItem)
					{
						char *frame,*flip,*fx,*fy,*visible,*time;
						do
						{
							SpriteAFrame* aframe = new SpriteAFrame();

							frame  = (char*)elem_aframeItem->Attribute("frame");
							ASSERT(frame!=NULL);
							aframe->m_frameIndex = atoi(frame);

							flip	  = (char*)elem_aframeItem->Attribute("flip");
							ASSERT(flip!=NULL);
							aframe->m_flipFlag = atoi(flip);

							fx   = (char*)elem_aframeItem->Attribute("fx");
							ASSERT(fx!=NULL);
							aframe->m_fx = atoi(fx);


							fy  = (char*)elem_aframeItem->Attribute("fy");
							ASSERT(fy!=NULL);
							aframe->m_fy = atoi(fy);

							time = (char*)elem_aframeItem->Attribute("time");
							ASSERT(time!=NULL);
							aframe->m_times = atoi(time);
							
							visible  = (char*)elem_aframeItem->Attribute("visible");
							if(strcmp(visible, "true")){
								aframe->m_isVisible = true;
							}else{
								aframe->m_isVisible = false;
							}

							action->m_aframes.push_back(aframe);

						}while(elem_aframeItem = elem_aframeItem->NextSiblingElement());
					}
				}while(elem_actionItem = elem_actionItem->NextSiblingElement());
			}
		}
		loadImages();

		return true;
	}

	void Sprite::loadImages()
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

	void Sprite::drawAction(size_t actionIndex, size_t frameIndex, int x, int y, BOOL flipX, BOOL flipY, int anchor)
	{
		SpriteAction* action = m_actions[actionIndex];
		// 动态帧
		SpriteAFrame* aframe = action->m_aframes[frameIndex];
		/** 获得静态帧翻转信息 */
		flipX = (((aframe->m_flipFlag & FLIP_HORIZONTALLY) == 1) ^ flipX);
		flipY = ((((aframe->m_flipFlag & FLIP_VERTICALLY) >> 1) == 1) ^ flipY);
		
		// 获得静态帧
		drawSFrame(aframe->m_frameIndex, x, y, flipX, flipY, anchor, aframe->m_fx, aframe->m_fy);
	}

	void Sprite::drawSFrame(size_t frameIndex,  int x, int y, BOOL flipX, BOOL flipY, int anchor,  int aframe_fx, int aframe_fy)
	{
		// 获得静态帧
		SpriteSFrame* sframe = m_sframes[frameIndex];
		vector<SpriteFModule *>::iterator iter = sframe->m_fmodules.begin();

		/** 循环内变量 **/
		int fmx, fmy, trans, mx, my, mw, mh, px, py, dx, dy;

		int index = 0;
		for( ; iter != sframe->m_fmodules.end(); ++iter){
			SpriteFModule* fmodule = *iter;
			SpriteSModule* smodule = m_smodules[fmodule->m_moduleIndex];
			
			fmx = fmodule->m_ox;
			fmy = fmodule->m_oy;
			trans = fmodule->m_transFlag;

			/** 获得静态module在图片上的区域位置信息 */
			mx = smodule->m_x;
			my = smodule->m_y;
			mw =smodule->m_width;
			mh = smodule->m_height; 

			bool rot = trans > TRANS_ROT180;

			px = fmx;
			py = fmy;

			/** 计算帧反转后module的信息 */
			if (flipX) {
				if(rot){
					px = -fmx - mh;
					trans ^= TRANS_MIRROR_ROT180 ;

				}else{
					px = -fmx - mw;
					trans ^= TRANS_MIRROR ;

				}
			}
			if (flipY) {
				if(rot){
					py = -fmy - mw;
					trans ^= TRANS_MIRROR ;
				}else{
					py = -fmy - mh;
					trans ^= TRANS_MIRROR_ROT180 ;
				}

			}
			dx = x + px + ((flipX)?-aframe_fx : aframe_fx);
			dy = y + py + aframe_fy;

			Image* image =  m_images[fmodule->m_moduleIndex];
			Render::Instance()->drawImage(image, (float)dx, (float)dy, trans, 0);	
			index++;
		}
		//Render::Instance()->checkError("in Sprite::drawAFrame");
	}

	int Sprite::getActionCount()
	{
		return m_actions.size();
	}

	int Sprite::getSFrameCount()
	{
		return m_sframes.size();
	}

	string Sprite::getActionName(unsigned int index)
	{
		SpriteAction* action = m_actions[index];
		return action->m_name;
	}


	void Sprite::getFrameWH(size_t frameindex, int* x, int* y, int* w, int* h) 
	{
		ASSERT(0<=frameindex && frameindex < m_sframes.size());
	
		SpriteSFrame* sframe = m_sframes[frameindex];
		SpriteFModule* fmodule = NULL;
		SpriteSModule* sMoudle = NULL;

		/** 循环内变量 */
		size_t module_index = 0;
 		int fmx, fmy, trans, mx, my, mw, mh;

		int imgId = 0;
		int minX,maxX,minY,maxY;
		minX = maxX = minY = maxY = 0;
		/** 循环绘制module */
		for (size_t i = 0; i < sframe->m_fmodules.size(); i++){
			fmodule = 	sframe->m_fmodules[i];
			module_index = fmodule->m_moduleIndex;
			ASSERT(0<=module_index && module_index < m_smodules.size());
			sMoudle = m_smodules[module_index];

			/** 获得静态module的x坐标 */
			fmx = fmodule->m_ox;
			/** 获得静态module的y坐标 */
			fmy = fmodule->m_oy;
			/** 获得静态module的变换信息 */
			trans = fmodule->m_transFlag;
			/** 获得静态module所对应的图片 */
			BOOL rot = trans > TRANS_ROT180;
			/** 获得静态module在图片上的区域位置信息 */
			mx = sMoudle->m_x;
			my = sMoudle->m_y;
			mw = rot?sMoudle->m_height:sMoudle->m_width;
			mh = rot?sMoudle->m_width:sMoudle->m_height;
			if(minX > fmx)minX = fmx;
			if(minY > fmy)minY = fmy;
			if(maxX < fmx + mw)maxX = fmx + mw;
			if(maxY < fmy + mh)maxY = fmy + mh;
		}
		*x = minX;
		*y = minY;
		*w = maxX - minX;
		*h = maxY - minY;
	
	}

	void Sprite::getActionWH(size_t actionId,int *x, int *y, int* w, int* h) 
	{
		ASSERT(0<=actionId && actionId < m_actions.size());
		int frameOffsetX, frameOffsetY,frameX,frameY,frameW,frameH;
		frameOffsetX = frameOffsetY = frameX = frameY = frameW = frameH = 0;

		int sframe_index = 0;
		int minX,maxX,minY,maxY;
		minX = maxX = minY = maxY = 0;
		SpriteAction* action =m_actions[actionId];
		int frameCount = action->m_aframes.size();

		SpriteAFrame* aframe = NULL;
		for(int i=0; i < frameCount;i++)
		{
			aframe = action->m_aframes[i];
			sframe_index = aframe->m_frameIndex;
			getFrameWH(sframe_index,&frameX,&frameY, &frameW, &frameH);

			/** 获得静态帧x坐标偏移量 */
			frameOffsetX = aframe->m_fx;
			/** 获得静态帧y坐标偏移量 */
			frameOffsetY = aframe->m_fy;

			if(minX > frameX + frameOffsetX)minX = frameX + frameOffsetX;
			if(minY > frameY + frameOffsetY)minY = frameY + frameOffsetY;
			if(maxX < frameX + frameOffsetX + frameW)maxX = frameX + frameOffsetX + frameW;
			if(maxY < frameY + frameOffsetY + frameH)maxY = frameY + frameOffsetY + frameH;
		}

		*x = minX;
		*y = minY;
		*w = maxX - minX;
		*h = maxY - minY;
	}
} // end lotus2d