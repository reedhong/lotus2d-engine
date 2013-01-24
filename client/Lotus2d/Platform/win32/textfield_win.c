/********************************************************************
	created:	2012/11/26
	filename: 	textfield_win.c
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Platform/textfield.h"
#include "Base/Log.h"

struct lotus2d_inputedit g_inputEditor;

void lotus2d_inputkey(int key)
{
	LOGD("in ge_inputkey: key=%d!!!", key);
	if(!g_inputEditor.observer)
		return;
	g_inputEditor.observer->inputkey(key);
}

void lotus2d_inpututf8(char* utf8)
{
	if(g_inputEditor.observer&&g_inputEditor.observer->inpututf8)
	{
		g_inputEditor.observer->inpututf8(utf8);
	}
}

