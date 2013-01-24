#include "Png.h"
#include "Zip.h"


namespace Lotus2d {

	enum
	{
		SCAN_load=0,
		SCAN_type,
		SCAN_header,
	};


	enum {
		F_none=0, F_sub=1, F_up=2, F_avg=3, F_paeth=4,
		F_avg_first, F_paeth_first,
	};

	static uint8 first_row_filter[5] =
	{
		F_none, F_sub, F_none, F_avg_first, F_paeth_first
	};


	#define PNG_TYPE(a,b,c,d)  (((a) << 24) + ((b) << 16) + ((c) << 8) + (d))

	Png::Png()
	{
		mStream = NULL;
		m_width = 0;
		m_height = 0;
		m_pixelData = NULL;    
		m_expanded = NULL; 
		m_idata = NULL;
	}

	Png* Png::loadPng(Stream*  stream,  int req_comp)
	{
		Png* png = new Png();
		png->mStream = stream;
		if(png->doParse(req_comp)){
			return png;
		}
		
		return NULL;
	}

	Png::~Png()
	{
		SAFE_FREE(m_pixelData);    
		SAFE_FREE(m_expanded); 
		SAFE_FREE(m_idata); 
	}

	BOOL Png::doParse(int req_comp)
	{
		unsigned char *result=NULL;
		m_expanded = NULL;
		m_idata = NULL;
		m_pixelData = NULL;
		if (parse(SCAN_load, req_comp)) 
		{
				return TRUE;
		}
		return FALSE;
	}

	BOOL Png::checkPngHeader()
	{
		static uint8 png_sig[8] = { 137,80,78,71,13,10,26,10 };
		uint8 header[8];
		mStream->read(header, 8);
		
		if  (memcmp(png_sig, header, 8) == 0)
			return TRUE;

		return FALSE;
	}

	Chunk Png::getChunkHeader()
	{
		Chunk c;
		c.length = mStream->read32();
		c.type = mStream->read32();
		return c;
	}

	static int paeth(int a, int b, int c)
	{
	   int p = a + b - c;
	   int pa = abs(p-a);
	   int pb = abs(p-b);
	   int pc = abs(p-c);
	   if (pa <= pb && pa <= pc) return a;
	   if (pb <= pc) return b;
	   return c;
	}

	static const int samples[] = { 1, 0, 3, 1, 2, 0, 4 };

	static int compute_bpp(const Png *pnginfo)
	{
		return samples[pnginfo->m_color] * pnginfo->m_depth / 8;
	}


	static int compute_rowbytes(const Png *pnginfo)
	{
		return (pnginfo->m_width * samples[pnginfo->m_color] * pnginfo->m_depth + 7) / 8;
	}

	/*-------------------------------------------------
		unfilter_row - unfilter a single row of pixels
	-------------------------------------------------*/

	static int unfilter_row(int type, uint8 *src, uint8 *dst, uint8 *dstprev, int bpp, int rowbytes)
	{
		int x;

		/* switch off of it */
		switch (type)
		{
			/* no filter, just copy */
			case F_none:
				for (x = 0; x < rowbytes; x++)
					*dst++ = *src++;
				break;

			/* SUB = previous pixel */
			case F_sub:
				for (x = 0; x < bpp; x++)
					*dst++ = *src++;
				for (x = bpp; x < rowbytes; x++, dst++)
					*dst = *src++ + dst[-bpp];
				break;

			/* UP = pixel above */
			case F_up:
				if (dstprev == NULL)
					return unfilter_row(F_none, src, dst, dstprev, bpp, rowbytes);
				for (x = 0; x < rowbytes; x++, dst++)
					*dst = *src++ + *dstprev++;
				break;

			/* AVERAGE = average of pixel above and previous pixel */
			case F_avg:
				if (dstprev == NULL)
				{
					for (x = 0; x < bpp; x++)
						*dst++ = *src++;
					for (x = bpp; x < rowbytes; x++, dst++)
						*dst = *src++ + dst[-bpp] / 2;
				}
				else
				{
					for (x = 0; x < bpp; x++, dst++)
						*dst = *src++ + *dstprev++ / 2;
					for (x = bpp; x < rowbytes; x++, dst++)
						*dst = *src++ + (*dstprev++ + dst[-bpp]) / 2;
				}
				break;

			/* PAETH = special filter */
			case F_paeth:
				for (x = 0; x < rowbytes; x++)
				{
					int32 pa = (x < bpp) ? 0 : dst[-bpp];
					int32 pc = (x < bpp || dstprev == NULL) ? 0 : dstprev[-bpp];
					int32 pb = (dstprev == NULL) ? 0 : *dstprev++;
					int32 prediction = pa + pb - pc;
					int32 da = abs(prediction - pa);
					int32 db = abs(prediction - pb);
					int32 dc = abs(prediction - pc);
					if (da <= db && da <= dc)
						*dst++ = *src++ + pa;
					else if (db <= dc)
						*dst++ = *src++ + pb;
					else
						*dst++ = *src++ + pc;
				}
				break;

			/* unknown filter type */
			default:
				return -1;
		}

		return 0;
	}

	/*-------------------------------------------------
		png_expand_buffer_8bit - expand a buffer from
		sub 8-bit to 8-bit
	-------------------------------------------------*/

	static int png_expand_buffer_8bit(Png *pnginfo)
	{
		uint32  i,j, k;
		uint8 *inp, *outp; //*outbuf;

		/* nothing to do if we're at 8 or greater already */
		if (pnginfo->m_depth >= 8)
			return 0;

		if (pnginfo->m_pixelData == NULL)
			return -1;

		inp = pnginfo->m_expanded;
		outp = pnginfo->m_pixelData;

		for (i = 0; i < pnginfo->m_height; i++)
		{
			for(j = 0; j < pnginfo->m_width / ( 8 / pnginfo->m_depth); j++)
			{
				for (k = 8 / pnginfo->m_depth-1; k >= 0; k--)
					*outp++ = (*inp >> k * pnginfo->m_depth) & (0xff >> (8 - pnginfo->m_depth));
				inp++;
			}
			if (pnginfo->m_width % (8 / pnginfo->m_depth))
			{
				for (k = pnginfo->m_width % (8 / pnginfo->m_depth)-1; k >= 0; k--)
					*outp++ = (*inp >> k * pnginfo->m_depth) & (0xff >> (8 - pnginfo->m_depth));
				inp++;
			}
		}

		return 0;
	}

	// create the png data from post-deflated data
	int Png::createPngImage2(uint8 *raw, uint32 raw_len, int out_n)
	{
		uint8 *src, *dst;
		int rowbytes, bpp, imagesize;
		uint32 y;
		/* compute some basic parameters */
		bpp = compute_bpp(this);
		rowbytes = compute_rowbytes(this);
		imagesize = m_height * (rowbytes + 1);

		/* we de-filter in place */
		src = dst = raw;


		/* iterate over rows */
		for (y = 0; y < m_height ; y++)
		{
			/* first byte of each row is the filter type */
			int filter = *src++;
			if(unfilter_row(filter, src, dst, (y == 0) ? NULL : &dst[-rowbytes], bpp, rowbytes)!=0)
			{
				break;
			}
			src += rowbytes;
			dst += rowbytes;
		}

		m_pixelData = (uint8 *) lotus2d_malloc(m_width * m_height);

		if(0!=png_expand_buffer_8bit(this))
			return 0;

		return 1;
	}
	// create the png data from post-deflated data
	int Png::createPngImage(uint8 *raw, uint32 raw_len, int out_n)
	{
		//ge_stream *s = &a->s;
		uint32 i,j,stride = m_width*out_n;
		int k;
		int img_n = m_img_n; // copy it into a local for later
		ASSERT(out_n == img_n || out_n == img_n+1);
		m_pixelData = (uint8 *) lotus2d_malloc(m_width * m_height * out_n);
		if (!m_pixelData) return 0;//e("outofmem", "Out of memory");
		if (raw_len != (img_n * m_width + 1) * m_height) return 0;//e("not enough pixels","Corrupt PNG");
		for (j=0; j < m_height; ++j) {
			uint8 *cur = m_pixelData + stride*j;
			uint8 *prior = cur - stride;
			int filter = *raw++;
			if (filter > 4) return 0;//e("invalid filter","Corrupt PNG");
			// if first row, use special filter that doesn't sample previous row
			if (j == 0) filter = first_row_filter[filter];
			// handle first pixel explicitly
			for (k=0; k < img_n; ++k) {
				switch(filter) {
				case F_none       : cur[k] = raw[k]; break;
				case F_sub        : cur[k] = raw[k]; break;
				case F_up         : cur[k] = raw[k] + prior[k]; break;
				case F_avg        : cur[k] = raw[k] + (prior[k]>>1); break;
				case F_paeth      : cur[k] = (uint8) (raw[k] + paeth(0,prior[k],0)); break;
				case F_avg_first  : cur[k] = raw[k]; break;
				case F_paeth_first: cur[k] = raw[k]; break;
				}
			}
			if (img_n != out_n) cur[img_n] = 255;
			raw += img_n;
			cur += out_n;
			prior += out_n;
			// this is a little gross, so that we don't switch per-pixel or per-component
			if (img_n == out_n) {
	#define CASE(f) \
				 case f:     \
				 for (i=m_width-1; i >= 1; --i, raw+=m_img_n,cur+=m_img_n,prior+=m_img_n) \
				 for (k=0; k < m_img_n; ++k)
				switch(filter) {
					CASE(F_none)  cur[k] = raw[k]; break;
					CASE(F_sub)   cur[k] = raw[k] + cur[k-img_n]; break;
					CASE(F_up)    cur[k] = raw[k] + prior[k]; break;
					CASE(F_avg)   cur[k] = raw[k] + ((prior[k] + cur[k-img_n])>>1); break;
					CASE(F_paeth)  cur[k] = (uint8) (raw[k] + paeth(cur[k-img_n],prior[k],prior[k-img_n])); break;
					CASE(F_avg_first)    cur[k] = raw[k] + (cur[k-img_n] >> 1); break;
					CASE(F_paeth_first)  cur[k] = (uint8) (raw[k] + paeth(cur[k-img_n],0,0)); break;
				}
	#undef CASE
			} else {
				ASSERT(img_n+1 == out_n);
	#define CASE(f) \
				 case f:     \
				 for (i=m_width-1; i >= 1; --i, cur[m_img_n]=255,raw+=m_img_n,cur+=out_n,prior+=out_n) \
				 for (k=0; k < m_img_n; ++k)
				switch(filter) {
					CASE(F_none)  cur[k] = raw[k]; break;
					CASE(F_sub)   cur[k] = raw[k] + cur[k-out_n]; break;
					CASE(F_up)    cur[k] = raw[k] + prior[k]; break;
					CASE(F_avg)   cur[k] = raw[k] + ((prior[k] + cur[k-out_n])>>1); break;
					CASE(F_paeth)  cur[k] = (uint8) (raw[k] + paeth(cur[k-out_n],prior[k],prior[k-out_n])); break;
					CASE(F_avg_first)    cur[k] = raw[k] + (cur[k-out_n] >> 1); break;
					CASE(F_paeth_first)  cur[k] = (uint8) (raw[k] + paeth(cur[k-out_n],0,0)); break;
				}
	#undef CASE
			}
		}
		return 1;
	}

	int Png::computeTransparency(uint8 tc[3], int out_n)
	{
		//ge_stream *s = &z->s;
		uint32 i, pixel_count = m_width * m_height;
		uint8 *p = m_pixelData;

		// compute color-based transparency, assuming we've
		// already got 255 as the alpha value in the output
		ASSERT(out_n == 2 || out_n == 4);

		if (out_n == 2) {
			for (i=0; i < pixel_count; ++i) {
				p[1] = (p[0] == tc[0] ? 0 : 255);
				p += 2;
			}
		} else {
			for (i=0; i < pixel_count; ++i) {
				if (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
					p[3] = 0;
				p += 4;
			}
		}
		return 1;
	}

	int Png::expandPalette(uint8 *palette, int len, int pal_img_n)
	{
		uint32 i, pixel_count = m_width *m_height;
		uint8 *p, *temp_out, *orig = m_pixelData;

		p = (uint8 *) lotus2d_malloc(pixel_count * pal_img_n);
		if (p == NULL) return 0;//e("outofmem", "Out of memory");

		// between here and ge_free(out) below, exitting would leak
		temp_out = p;

		if (pal_img_n == 3) {
			for (i=0; i < pixel_count; ++i) {
				int n = orig[i]*4;
				p[0] = palette[n  ];
				p[1] = palette[n+1];
				p[2] = palette[n+2];
				p += 3;
			}
		} else {
			for (i=0; i < pixel_count; ++i) {
				int n = orig[i]*4;
				p[0] = palette[n  ];
				p[1] = palette[n+1];
				p[2] = palette[n+2];
				p[3] = palette[n+3];
				p += 4;
			}
		}
		lotus2d_free(m_pixelData);
		m_pixelData = temp_out;
		return 1;
	}

	int Png::parse(int scan, int req_comp)
	{
		uint8 palette[1024], pal_img_n=0;
		uint8 has_trans=0, tc[3];
		uint32 ioff=0, idata_limit=0, i, pal_len=0;
		int first=1,k;

		if (!checkPngHeader()) return 0;

		if (scan == SCAN_type) return 1;

		for(;;first=0) {
			Chunk c = getChunkHeader();
			if (first && c.type != PNG_TYPE('I','H','D','R'))
				return 0;//e("first not IHDR","Corrupt PNG");
			switch (c.type) {
			 case PNG_TYPE('I','H','D','R'): {
				int interlace,comp,filter;
				 if (!first) return 0;//e("multiple IHDR","Corrupt PNG");
				 if (c.length != 13) return 0;//e("bad IHDR len","Corrupt PNG");
				 m_width = mStream->read32(); if (m_width > (1 << 24)) return 0;//e("too large","Very large image (corrupt?)");
				 m_height = mStream->read32(); if (m_height > (1 << 24)) return 0;//e("too large","Very large image (corrupt?)");
				 m_depth = mStream->read8();
				 //if (depth != 8)        return 0;//e("8bit only","PNG not supported: 8-bit only");
				 m_color =  mStream->read8();
				 if (m_color > 6)         return 0;//e("bad ctype","Corrupt PNG");
				 if (m_color == 3) pal_img_n = 3; 
				 else if (m_color & 1) return 0;//e("bad ctype","Corrupt PNG");
				 comp  = mStream->read8(); 
				 if (comp) return 0;//e("bad comp method","Corrupt PNG");
				 filter= mStream->read8();
				 if (filter) return 0;//e("bad filter method","Corrupt PNG");
				 interlace = mStream->read8();
				 if (interlace) return 0;//e("interlaced","PNG not supported: interlaced mode");
				 if (!m_width || !m_height) return 0;//e("0-pixel image","Corrupt PNG");
				 if (!pal_img_n) {
					 m_img_n = (m_color & 2 ? 3 : 1) + (m_color & 4 ? 1 : 0);
					 if ((1 << 30) / m_width / m_img_n < m_height) return 0;//e("too large", "Image too large to decode");
					 if (scan == SCAN_header) return 1;
				 } else {
					 // if paletted, then pal_n is our final components, and
					 // img_n is # components to decompress/filter.
					 m_img_n = 1;
					 if ((1 << 30) / m_width / 4 < m_height) return 0;//e("too large","Corrupt PNG");
					 // if SCAN_header, have to scan to see if we have a tRNS
				 }
				 break;
											 }

			 case PNG_TYPE('P','L','T','E'):  {
				 if (c.length > 256*3) return 0;//e("invalid PLTE","Corrupt PNG");
				 pal_len = c.length / 3;
				 if (pal_len * 3 != c.length) return 0;//e("invalid PLTE","Corrupt PNG");
				 for (i=0; i < pal_len; ++i) {
					 palette[i*4+0] = mStream->read8();
					 palette[i*4+1] = mStream->read8();
					 palette[i*4+2] = mStream->read8();
					 palette[i*4+3] = 255;
				 }
				 break;
											  }

			 case PNG_TYPE('t','R','N','S'): {
				 if (m_idata) return 0;//e("tRNS after IDAT","Corrupt PNG");
				 if (pal_img_n) {
					 if (scan == SCAN_header) { m_img_n = 4; return 1; }
					 if (pal_len == 0) return 0;//e("tRNS before PLTE","Corrupt PNG");
					 if (c.length > pal_len) return 0;//e("bad tRNS len","Corrupt PNG");
					 pal_img_n = 4;
					 for (i=0; i < c.length; ++i)
						 palette[i*4+3] = mStream->read8();
				 } else {
					 if (!(m_img_n & 1)) return 0;//e("tRNS with alpha","Corrupt PNG");
					 if (c.length != (uint32) m_img_n*2) return 0;//e("bad tRNS len","Corrupt PNG");
					 has_trans = 1;
					 for (k=0; k < m_img_n; ++k)
						 tc[k] = (uint8) mStream->read16(); // non 8-bit images will be larger
				 }
				 break;
											 }

			 case PNG_TYPE('I','D','A','T'): {
				 if (pal_img_n && !pal_len) return 0;//e("no PLTE","Corrupt PNG");
				 if (scan == SCAN_header) { m_img_n = pal_img_n; return 1; }
				 if (ioff + c.length > idata_limit) {
					 uint8 *p;
					 if (idata_limit == 0) idata_limit = c.length > 4096 ? c.length : 4096;
					 while (ioff + c.length > idata_limit)
						 idata_limit *= 2;
					 if(m_idata){
						 p = (uint8 *)lotus2d_realloc(m_idata, idata_limit); 
						 if (p == NULL) return 0;//e("outofmem", "Out of memory");
					 }else{
						 p = (uint8 *) lotus2d_malloc(idata_limit);
						 if (p == NULL) return 0;//e("outofmem", "Out of memory");
					 }
					 m_idata = p;
				 }
	#ifndef STBI_NO_STDIO
				// if (s->file)
				 if(0)
				 {
					// if (fread(z->idata+ioff,1,c.length,s->file) != c.length) return 0;//e("outofdata","Corrupt PNG");
				 }
				 else
	#endif
				 {
					 mStream->read(m_idata+ioff, c.length);
					 //memcpy(m_idata+ioff, mStream->m_buffer, c.length);
					 //mStream->m_buffer += c.length;
				 }
				 ioff += c.length;
				 break;
											 }

			 case PNG_TYPE('I','E','N','D'): {
				 uint32 raw_len;
				 if (scan != SCAN_load) return 1;
				 if (m_idata == NULL) 0;//return e("no IDAT","Corrupt PNG");
				 m_expanded = (uint8 *) Zip::ge_zlib_decode_malloc((char *) m_idata, ioff, (int *) &raw_len);
				 if (m_expanded == NULL) return 0; // zlib should set error
				 lotus2d_free(m_idata); m_idata = NULL;
				 if ((req_comp == m_img_n+1 && req_comp != 3 && !pal_img_n) || has_trans)
					 m_channel = m_img_n+1;
				 else
					 m_channel = m_img_n;
				 if(m_depth==8)
				 {
					 if (!createPngImage(m_expanded, raw_len, m_channel)) return 0;
				 }
				 else
				 {
					 if (!createPngImage2(m_expanded, raw_len, m_channel)) return 0;
				 }

				 if (has_trans)
					 if (!computeTransparency(tc, m_channel)) return 0;
				 if (pal_img_n) {
					 // pal_img_n == 3 or 4
					 m_img_n = pal_img_n; // record the actual colors we had
					 m_channel = pal_img_n;
					 if (req_comp >= 3) m_channel = req_comp;
					 if (!expandPalette(palette, pal_len, m_channel))
						 return 0;
				 }
				 lotus2d_free(m_expanded); m_expanded = NULL;
				 return 1;
											 }

			 default:
				 // if critical, fail
				 if ((c.type & (1 << 29)) == 0) {
	#ifndef STBI_NO_FAILURE_STRINGS
					 // not threadsafe
					 static char invalid_chunk[] = "XXXX chunk not known";
					 invalid_chunk[0] = (uint8) (c.type >> 24);
					 invalid_chunk[1] = (uint8) (c.type >> 16);
					 invalid_chunk[2] = (uint8) (c.type >>  8);
					 invalid_chunk[3] = (uint8) (c.type >>  0);
	#endif
					 return 0;//e(invalid_chunk, "PNG not supported: unknown chunk type");
				 }
				 mStream->skip(c.length);
				 break;
			}
			// end of chunk, read and skip CRC
			mStream->read32();
		}
	}
} // end namespace