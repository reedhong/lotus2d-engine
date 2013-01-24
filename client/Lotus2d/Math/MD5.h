/********************************************************************
	created:	2012/12/13
	filename: 	MD5.h
	author:		reedhong
	
	purpose:	
*********************************************************************/
#ifndef __Lotus2d_MD5_H__
#define __Lotus2d_MD5_H__

namespace Lotus2d{

	/* Constants for MD5Transform routine.
	 */
	#define S11 7
	#define S12 12
	#define S13 17
	#define S14 22
	#define S21 5
	#define S22 9
	#define S23 14
	#define S24 20
	#define S31 4
	#define S32 11
	#define S33 16
	#define S34 23
	#define S41 6
	#define S42 10
	#define S43 15
	#define S44 21

	//---------------------------------------------------------------------------
	/* MD5 context. */
	typedef struct __MD5_CTX__
	{
		unsigned int state[4];           /* state (ABCD) */
		unsigned int count[2];           /* number of bits, modulo 2^64 (lsb first) */
		unsigned char buffer[64]; /* input buffer */
	} MD5_CTX;


	class TMD5
	{
	public :
		 TMD5();
		 ~TMD5();
		/**
		* @param [IN] szInput const unsigned char * 输入buffer
		* @param [IN] inputLen unsigned int 输入长度
		* @param [OUT] szOutput[16] unsigned char 输出buffer
		* @param [IN] iIteration int update次数
		* @return TInt
		* @note MD5加密函数
		*/	
		 void MessageDigest(const unsigned char *szInput, unsigned int inputLen,
				unsigned char szOutput[16], int iIteration = 1);
	private :
		unsigned char PADDING[64];
		MD5_CTX context;
		void Init(MD5_CTX *context);
		void Update(MD5_CTX *context, const unsigned char *input, unsigned int inputLen);
		void Final(unsigned char digest[16], MD5_CTX *context);
		void Transform(unsigned int state[4], const unsigned char block[64]);
		void memcpy(const void* a, const void* b, int size);
		void memset(const void* a, int size,char v);
	};

	//---------------------------------------------------------------------------
	/* F, G, H and I are basic MD5 functions.
	 */
	#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
	#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
	#define H(x, y, z) ((x) ^ (y) ^ (z))
	#define I(x, y, z) ((y) ^ ((x) | (~z)))

	//---------------------------------------------------------------------------
	/* ROTATE_LEFT rotates x left n bits.
	 */
	#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

	/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
	Rotation is separate from addition to prevent recomputation.
	 */
	#define FF(a, b, c, d, x, s, ac)\
	{ \
		(a) += F ((b), (c), (d)) + (x) + (unsigned int)(ac); \
		(a) = ROTATE_LEFT ((a), (s)); \
		(a) += (b); \
	}
	#define GG(a, b, c, d, x, s, ac)\
	{ \
		(a) += G ((b), (c), (d)) + (x) + (unsigned int)(ac); \
		(a) = ROTATE_LEFT ((a), (s)); \
		(a) += (b); \
	}
	#define HH(a, b, c, d, x, s, ac)\
	{ \
		(a) += H ((b), (c), (d)) + (x) + (unsigned int)(ac); \
		(a) = ROTATE_LEFT ((a), (s)); \
		(a) += (b); \
	}
	#define II(a, b, c, d, x, s, ac)\
	{ \
		(a) += I ((b), (c), (d)) + (x) + (unsigned int)(ac); \
		(a) = ROTATE_LEFT ((a), (s)); \
		(a) += (b); \
	}


}
#endif