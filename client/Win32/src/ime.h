#include <windows.h>
#include <imm.h>
#include "Platform/textfield.h"

class CIme{
	bool g_bIme;//ime允许标志
	char g_szCompStr[ MAX_PATH ];//存储转换后的串
	char g_szCompReadStr[ MAX_PATH ];//存储输入的串
	char g_szCandList[ MAX_PATH ];//存储整理成字符串选字表
	int g_nImeCursor;//存储转换后的串中的光标位置
	CANDIDATELIST *g_lpCandList;//存储标准的选字表
	char g_szImeName[ 64 ];//存储输入法的名字
	bool g_bImeSharp;//全角标志
	bool g_bImeSymbol;//中文标点标志
	void ConvertCandList( CANDIDATELIST *pCandList, char *pszCandList );//将选字表整理成串
public:
	CIme() : g_lpCandList( NULL ){ DisableIme(); }//通过DisableIme初始化一些数据
	~CIme(){
		DisableIme();
		if( g_lpCandList ){
			GlobalFree( (HANDLE)g_lpCandList );
			g_lpCandList = NULL;
		}
	}
	//控制函数
	void DisableIme();//关闭并禁止输入法，如ime已经打开则关闭，此后玩家不能用热键呼出ime
	void EnableIme();//允许输入法，此后玩家可以用热键呼出ime
	void NextIme();//切换到下一种输入法，必须EnableIme后才有效
	void SharpIme( HWND hWnd );//切换全角/半角
	void SymbolIme( HWND hWnd );//切换中/英文标点

	//状态函数
	char* GetImeName();//得到输入法名字，如果当前是英文则返回NULL
	bool IfImeSharp();//是否全角
	bool IfImeSymbol();//是否中文标点
	void GetImeInput( char **pszCompStr, char **pszCompReadStr, int *pnImeCursor, char **pszCandList );
		//得到输入法状态，四个指针任意可为NULL则此状态不回返回
		//在pszCompStr中返回转换后的串
		//在pszCompReadStr中返回键盘直接输入的串
		//在pnImeCursor中返回szCompStr的光标位置
		//在pszCandList中返回选字表，每项之间以\t分隔

	//必须在消息中调用的函数，如果返回是true，则窗口函数应直接返回0，否则应传递给DefWindowProc
	bool OnWM_INPUTLANGCHANGEREQUEST();
	bool OnWM_INPUTLANGCHANGE( HWND hWnd );
	bool OnWM_IME_SETCONTEXT(){ return true; }
	bool OnWM_IME_STARTCOMPOSITION(){ return true; }
	bool OnWM_IME_ENDCOMPOSITION(){ return true; }
	bool OnWM_IME_NOTIFY( HWND hWnd, WPARAM wParam );
	bool OnWM_IME_COMPOSITION( HWND hWnd, LPARAM lParam );
};

void CIme::DisableIme(){
	// win7 下面出现死循环
	//while( ImmIsIME( GetKeyboardLayout( 0 ))) 
	if( ImmIsIME( GetKeyboardLayout( 0 )))
		ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );//如果ime打开通过循环切换到下一个关闭
	g_bIme = false;
	g_szImeName[ 0 ] = 0;
	g_szCompStr[ 0 ] = 0;
	g_szCompReadStr[ 0 ] = 0;
	g_szCandList[ 0 ] = 0;
	g_nImeCursor = 0;
}

void CIme::EnableIme(){
	g_bIme = true;
}

void CIme::NextIme(){
	if( g_bIme )ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );
}

void CIme::SharpIme( HWND hWnd ){
	ImmSimulateHotKey( hWnd, IME_CHOTKEY_SHAPE_TOGGLE );
}

void CIme::SymbolIme( HWND hWnd ){
	ImmSimulateHotKey( hWnd, IME_CHOTKEY_SYMBOL_TOGGLE );
}

void CIme::ConvertCandList( CANDIDATELIST *pCandList, char *pszCandList ){//转换CandidateList到一个串，\t分隔每一项
	unsigned int i;
	if( pCandList->dwCount < pCandList->dwSelection ){
		pszCandList[ 0 ] = 0;
		return;
	}
		//待选字序号超出总数，微软拼音第二次到选字表最后一页后再按PageDown会出现这种情况，并且会退出选字状态，开始一个新的输入
		//但微软拼音自己的ime窗口可以解决这个问题，估计微软拼音实现了更多的接口，所以使用了这种不太标准的数据
		//我现在无法解决这个问题，而且实际使用中也很少遇到这种事，而且其它标准输入法不会引起这种bug
		//非标准输入法估计实现的接口比较少，所以应该也不会引起这种bug
	for( i = 0; ( i < pCandList->dwCount - pCandList->dwSelection )&&( i < pCandList->dwPageSize ); i++ ){
		*pszCandList++ = ( i % 10 != 9 )? i % 10 + '1' : '0';//每项对应的数字键
		*pszCandList++ = '.';//用'.'分隔
		strncpy( pszCandList, (char*)pCandList
			+ pCandList->dwOffset[ pCandList->dwSelection + i ] ,259);//每项实际的内容
		pszCandList += strlen( pszCandList );
		*pszCandList++ = '\t';//项之间以'\t'分隔
	}
	*( pszCandList - 1 )= 0;//串尾，并覆盖最后一个'\t'
}

bool CIme::OnWM_INPUTLANGCHANGEREQUEST(){
	return !g_bIme;//如果禁止ime则返回false，此时窗口函数应返回0，否则DefWindowProc会打开输入法
}

bool CIme::OnWM_INPUTLANGCHANGE( HWND hWnd ){
	//ime改变
	HKL hKL = GetKeyboardLayout( 0 );
	if( ImmIsIME( hKL )){
		HIMC hIMC = ImmGetContext( hWnd );
		ImmEscape( hKL, hIMC, IME_ESC_IME_NAME, g_szImeName );//取得新输入法名字
		DWORD dwConversion, dwSentence;
		ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
		g_bImeSharp = ( dwConversion & IME_CMODE_FULLSHAPE )? true : false;//取得全角标志
		g_bImeSymbol = ( dwConversion & IME_CMODE_SYMBOL )? true : false;//取得中文标点标志
		ImmReleaseContext( hWnd, hIMC );
	}
	else//英文输入
		g_szImeName[ 0 ] = 0;
	return false;//总是返回false，因为需要窗口函数调用DefWindowProc继续处理
}

bool CIme::OnWM_IME_NOTIFY( HWND hWnd, WPARAM wParam ){
	HIMC hIMC;
	DWORD dwSize;
	DWORD dwConversion, dwSentence;
	switch( wParam ){
		case IMN_SETCONVERSIONMODE://全角/半角，中/英文标点改变
			hIMC = ImmGetContext( hWnd );
			ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
			g_bImeSharp = ( dwConversion & IME_CMODE_FULLSHAPE )? true : false;
			g_bImeSymbol = ( dwConversion & IME_CMODE_SYMBOL )? true : false;
			ImmReleaseContext( hWnd, hIMC );
			break;
		case IMN_OPENCANDIDATE://进入选字状态
		case IMN_CHANGECANDIDATE://选字表翻页
			hIMC = ImmGetContext( hWnd );
			if( g_lpCandList ){
				GlobalFree( (HANDLE)g_lpCandList );
				g_lpCandList = NULL;
			}//释放以前的选字表
			if( dwSize = ImmGetCandidateList( hIMC, 0, NULL, 0 )){
				g_lpCandList = (LPCANDIDATELIST)GlobalAlloc( GPTR, dwSize );
				if( g_lpCandList )
					ImmGetCandidateList( hIMC, 0, g_lpCandList, dwSize );
			}//得到新的选字表
			ImmReleaseContext( hWnd, hIMC );
			if( g_lpCandList )ConvertCandList( g_lpCandList, g_szCandList );//选字表整理成串
			break;
		case IMN_CLOSECANDIDATE://关闭选字表
			if( g_lpCandList ){
				GlobalFree( (HANDLE)g_lpCandList );
				g_lpCandList = NULL;
			}//释放
			g_szCandList[ 0 ] = 0;
			break;
	}
	return true;//总是返回true，防止ime窗口打开
}
char* ucs_to_utf8(const wchar_t* pucs)
{
	static char tmpbuf[1024];
	memset(tmpbuf, 0, sizeof(tmpbuf));
	WideCharToMultiByte(CP_UTF8, 0, pucs, (int)wcslen(pucs), tmpbuf, 1023,0,0);
	return tmpbuf;

}

bool CIme::OnWM_IME_COMPOSITION( HWND hWnd, LPARAM lParam ){//输入改变
	HIMC hIMC;
	DWORD dwSize;
	hIMC = ImmGetContext( hWnd );
	if( lParam & GCS_COMPSTR ){
		dwSize = ImmGetCompositionString( hIMC, GCS_COMPSTR, (void*)g_szCompStr, sizeof( g_szCompStr ));
		g_szCompStr[ dwSize ] = 0;
	}//取得szCompStr
	if( lParam & GCS_COMPREADSTR ){
		dwSize = ImmGetCompositionString( hIMC, GCS_COMPREADSTR, (void*)g_szCompReadStr, sizeof( g_szCompReadStr ));
		g_szCompReadStr[ dwSize ] = 0;
	}//取得szCompReadStr
	if( lParam & GCS_CURSORPOS ){
		g_nImeCursor = 0xffff & ImmGetCompositionString( hIMC, GCS_CURSORPOS, NULL, 0 );
	}//取得nImeCursor
	if( lParam & GCS_RESULTSTR ){
		static wchar_t str[ MAX_PATH ];
		wchar_t* pStr = str;
		LONG buflen = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR,NULL,0);
		if(buflen>0)
		{
			buflen += sizeof(wchar_t);
			if(buflen>MAX_PATH)
			{
				pStr = new wchar_t[buflen];
			}
			else
			{
				buflen = MAX_PATH;
			}
			memset(pStr,0,buflen*sizeof(wchar_t));
			ImmGetCompositionStringW( hIMC, GCS_RESULTSTR, (void*)pStr, buflen);//取得汉字输入串
			wchar_t *p = str;
			lotus2d_inpututf8(ucs_to_utf8((const wchar_t*)p));

			if(buflen>MAX_PATH)
				delete[] pStr;
			
		}
		
		//dwSize = ImmGetCompositionString( hIMC, GCS_RESULTSTR, (void*)str, sizeof( str ));//取得汉字输入串
		//str[ dwSize ] = 0;
		//unsigned char *p = str;
		//ge_inpututf8(ucs_to_utf8((const wchar_t*)p));
		//while( *p )PostMessage( hWnd, WM_CHAR, (WPARAM)(*p++), 1 );//转成WM_CHAR消息
	}
	ImmReleaseContext( hWnd, hIMC );
	return true;//总是返回true，防止ime窗口打开
}

char* CIme::GetImeName(){
	return g_szImeName[ 0 ]? g_szImeName : NULL;
}

bool CIme::IfImeSharp(){//是否全角
	return g_bImeSharp;
}

bool CIme::IfImeSymbol(){//是否中文标点
	return g_bImeSymbol;
}

void CIme::GetImeInput( char **pszCompStr, char **pszCompReadStr, int *pnImeCursor, char **pszCandList ){
	if( pszCompStr )*pszCompStr = g_szCompStr;
	if( pszCompReadStr )*pszCompReadStr = g_szCompReadStr;
	if( pnImeCursor )*pnImeCursor = g_nImeCursor;
	if( pszCandList )*pszCandList = g_szCandList;
}

//由于微软拼音实现了很多自己的东西，CIme和它的兼容性有些问题
//1、在函数ConvertCandList中所说的选字表的问题
//2、函数GetImeInput返回的szCompReadStr显然经过了加工而不是最初的键盘输入
//   它的每个可组合的输入占以空格补足的8byte，且新的不可组合的输入存为0xa1
//   我们可以在输入法名字中有子串"微软拼音"时，只显示末尾的一组8byte，如果有0xa1就什么都不显示，也可以直接用TextOut显示所有的
