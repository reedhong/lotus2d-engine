/********************************************************************
	created:	2013/01/15
	filename: 	Framework.h
	author:		reedhong
	
	purpose:	抽象一个游戏的外部接口框架
*********************************************************************/
#ifndef __Lotus2d_Framework_H__
#define __Lotus2d_Framework_H__

#include "Event/Event.h"

namespace Lotus2d {
	class Framework
	{
	public:
		Framework();
		virtual ~Framework();
	public:
	 	virtual void gameStartup(int width,int height,float scale=1.0) = 0;

		virtual  void gameEnd() = 0;

		virtual void gameFrame() = 0;

		virtual void gamePause() = 0;

		virtual void gameResume() = 0;

		virtual void addAction(eRawActionEventType event, int x, int y) = 0;

		virtual void addKey(unsigned char key) = 0;

		int		getFrameInterverl()
		{
			return m_frameInterval;
		}
		void	setFrameInterval(int frameInterval)
		{
			m_frameInterval = frameInterval;
		}
	protected:
		int	m_frameInterval;		// 每帧间隔
	};
}


#endif // __Framework_H__

