/********************************************************************
	created:	2012/09/22
	filename: 	UIEngine.h
	author:		reedhong
	
	purpose:	UIÒýÇæ
*********************************************************************/

#ifndef __Lotus2d_UIEngine_H__
#define __Lotus2d_UIEngine_H__

#include "Control.h"
namespace Lotus2d {	
	
	typedef void (*UICommandCallback )(int, Control*  control);
	class UIEngine
	{
	protected:
		UIEngine();
	public:
		~UIEngine();
		static UIEngine* Instance();
		void setCallback(UICommandCallback pHandler);
		void handleUICommand(int command, Control* control);
	public:

	private:
		UICommandCallback	m_callback;

	};
}


#endif
