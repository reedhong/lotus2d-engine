/********************************************************************
	created:	2012/09/14 
	filename: 	Png.h
	author:		reedhong 
	
	purpose:	º”‘ÿPngÕº∆¨
*********************************************************************/

#include  "Base/Prerequisites.h"
#include "Base/Stream.h"

namespace Lotus2d {
	struct Chunk
	{
		uint32 length;
		uint32 type;
	};

	class Png
	{
	public:
		static Png* loadPng(Stream*  stream, int req_comp=4); 
		~Png();

	protected:
		Png();

	private:
		BOOL doParse(int req_comp);
		int parse(int scan, int req_comp);
		BOOL checkPngHeader();
		Chunk getChunkHeader();
		int createPngImage(uint8 *raw, uint32 raw_len, int out_n);
		int createPngImage2(uint8 *raw, uint32 raw_len, int out_n);
		int computeTransparency(uint8 tc[3], int out_n);
		int expandPalette(uint8 *palette, int len, int pal_img_n);

	public:
		Stream*		mStream;
		uint8 *m_idata;
		uint8 *m_expanded;
		uint8 *m_pixelData;
		uint32 m_width;
		uint32 m_height;
		uint8  m_depth;
		uint8 m_color;
		int m_img_n;
		int m_channel;
	};

}