/********************************************************************
	created:	2012/11/08
	filename: 	ControlDef.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef __Lotus2d_ControlDef_H__
#define __Lotus2d_ControlDef_H__

namespace Lotus2d {

	enum eCtrlType{
		eCtrlBase = -1,
		eCtrlText = 0,
		eCtrlInput,
		eCtrlAnimation,
		eCtrlImage,
		eCtrlPanel,
		eCtrlPatch,
		eCtrlPage,
		eCtrlList,
		eCtrlTab,
		eCtrlTabItem
	};

	enum eInputMode{
		eInputNormal = 0,
		eInputNumber = 1,
		eInputPassword = 2
	};

	enum eCtrlAlignMode{
		eAlignLeft = 0,
		eAlignRight,
		eAlignCenter,
	};

	enum eCtrlListTrend{
		eTrendHorizon = 0, // 水平
		eTrendVertical,			// 垂直
	};

	enum eCtrlListSlideMode{
		eSlideNormal = 0,	// 普通滑动
		eSlidePage				// 页面滑动
	};

	enum eCtrlEventType
	{
		eCtrlEventFire =0,
		eCtrlEventDragEnd,
		eCtrlEventEventChange,
		eCtrlEventCount
	};

#if 0
	static const int8 INPUT_LOWSER_ABC = 1;
	static const int8 INPUT_UPPDER_ABC = 2;
	static const int8 INPUT_NUMBER = 4;
	static const int8 INPUT_PINYIN = 8;
	static const int8 INPUT_PINYIN_EXCLUDE = 7;
	static const int8 INPUT_ANY = 15;
	static const int8 INPUT_HISTORY = 16;
	static const int8 INPUT_PASSWORD = 32;
#endif

}

#endif
