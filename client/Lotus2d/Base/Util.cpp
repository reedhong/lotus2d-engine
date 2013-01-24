/********************************************************************
	created:	2012/09/16
	filename: 	Util.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Util.h"

namespace Lotus2d {
	// todo: 暂时只能解决windows中的情况
	char* Util::getFileNameNoExt(char* szPath)
	{
		if (!szPath) return NULL;
		int endpos = 0;
		int len = strlen(szPath);
		char *p = szPath + len;
		while (len){
			if(*p == '.'){
				endpos = len;	
			}else if (*p == '\\'){
					break;
				}
				p--; len--;
		}

		if(len < endpos){
			char *pnew = new char[endpos - len];
			strncpy (pnew,szPath + len + 1,endpos - len - 1);
			*(pnew + (endpos - len - 1)) = '\0';
			return pnew;
		}
		return NULL;
	}

	bool Util::isExistedPath(const char* path)
	{
		FILE *fp =fopen(path,"rb");
		if(fp){
			fclose(fp);
			return true;
		}
		return false;
	}

	size_t Util::getFileSize(const char* path)
	{
		long  size;
		FILE *fp;

		fp=fopen(path,"rb");
		ASSERT(fp!=0);
		fseek(fp,0,SEEK_END);
		size=ftell(fp);
		fclose(fp);

		return size;
	}


#define PTR_SET(lptr, rvalue)	if((lptr) != NULL){*(lptr) = (rvalue);}

	void Util::geoTransform(int *px, int *py, int *pwidth, int *pheight, int *protateDegree, bool *pmirror, int trans, int anchor)
	{
		int new_x = *px, new_y = *py, new_width = *pwidth, new_height = *pheight;

		PTR_SET(protateDegree, 0);
		PTR_SET(pmirror, FALSE);
		if(px == NULL || py == NULL || pwidth == NULL || pheight == NULL ){
			return;
		}

		switch(trans)
		{
		case TRANS_MIRROR_ROT90: /* Mirror rotate 90 */
			{
				new_width = *pheight;
				new_height = *pwidth;
				new_x += new_width;
				new_y += new_height;
				PTR_SET(protateDegree, -90);
				PTR_SET(pmirror, TRUE);
			}
			break;
		case TRANS_ROT90:		/* Rotate 90 */
			{
				new_width = *pheight;
				new_height = *pwidth;
				new_x += new_width;
				PTR_SET(protateDegree, 90);
			}
			break;
		case TRANS_MIRROR_ROT180: /* Mirror rotate 180 */
			{
				new_y += new_height;
				PTR_SET(protateDegree, -180);
				PTR_SET(pmirror, TRUE);
			}
			break;
		case TRANS_ROT180:		  /* Rotate 180 */
			{
				new_x += new_width;
				new_y += new_height;
				PTR_SET(protateDegree, 180);
			}
			break;
		case TRANS_MIRROR_ROT270: /* Mirror roate 270 */
			{
				new_width = *pheight;
				new_height = *pwidth;
				PTR_SET(protateDegree, -270);
				PTR_SET(pmirror, TRUE);
			}
			break;
		case TRANS_ROT270:		 /* Rotate 270 */
			{
				new_width = *pheight;
				new_height = *pwidth;
				new_y += new_height;
				PTR_SET(protateDegree, 270);
			}
			break;
		case TRANS_MIRROR:		/* Mirror */
			{
				new_x += new_width;
				PTR_SET(pmirror, TRUE);
			}
			break;
		default:
			break;
		}

		if((anchor & ANCHOR_HCENTER) >= 1){
			*px = new_x + new_width / 2;
		}
		if((anchor & ANCHOR_VCENTER) >= 1){
			*py = new_y - new_height / 2;
		}
		if((anchor & ANCHOR_RIGHT) >= 1){
			*px = new_x - new_width;
		}
		if((anchor & ANCHOR_BOTTOM) >= 1){
			*py = new_x - new_height;
		}
		if((anchor & ANCHOR_TOP) >= 1){
			*py = new_y - new_height;
		}
	}

	bool Util::isInRect(int px,int py,int x,int y,int width,int height)
	{
		return (px > x) && (px < x + width) && (py > y) && (py < y + height);
	}

	bool Util::String2Buf(const string& str, unsigned char* buf, int len)
	{
		int beginPos = 0;
		int  endPos = str.find(',', beginPos);
		int index = 0;
		while(endPos != string::npos && index < len){
			string subStr =  str.substr(beginPos, endPos-beginPos);
			buf[index++] = atoi(subStr.c_str());
			beginPos = endPos+1;
			endPos = str.find(',', beginPos);
		}
		return true;
	}

	 /** Method for standardising paths
        */
	string Util::standardisePath( const string &init, bool endSlash )
	{
		string path = init;

		std::replace( path.begin(), path.end(), '\\', '/' );
		if(endSlash && path[path.length() - 1] != '/' )
			path += '/';

		return path;
	}
}