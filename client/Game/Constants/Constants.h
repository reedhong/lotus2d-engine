/********************************************************************
	created:	2012/10/27
	filename: 	Constants.h
	author:		reedhong
	
	purpose:	定义游戏中用到的常量
*********************************************************************/

#ifndef __Game_Constants_H__
#define __Game_Constants_H__

//============================== ACTOR FLAG ======================
static const int FLAG_FLIPX = (1 << 0);
static const int FLAG_FLIPY = (1 << 1);
static const int FLAG_PAUSE = (1 << 2);
static const int FLAG_ANIM_END_AND_DEL = (1 << 3);
static const int FLAG_ACTION_CYCLE = (1 << 4);
static const int FLAG_MOVE_OVER_DEL = (1 << 5);
static const int FLAG_ACTION_END = (1 << 6);
static const int FLAG_BATTLE = (1 << 7);
static const int FLAG_HAS_PET = (1 << 8);
static const int FLAG_RESET_ACTION = (1 << 9);
static const int FLAG_TASK_END = (1 << 10);
static const int FLAG_TASK_RUNNING = (1 << 11);
static const int FLAG_TASK_ENABLED = (1 << 12);
static const int FLAG_STAND = (1 << 13);
static const int FLAG_ACT_END = (1 << 14);
static const int FLAG_ACT_RUNNING = (1 << 15);
static const int FLAG_ACT_ENABLED = (1 << 16);
static const int FLAG_SHOW_FURNITURE_SURFACE = (1 << 17);

static const int FLAG_POINT_HOVER = (1 << 18);
static const int FLAG_POINT_SELECTED = (1 << 19);
static const int FLAG_AIDONE = (1 << 20);

static const int FLAG_ESCAPE = (1 << 21);




//action of actor
static const int ACTION_ZHANLI = 0;
static const int ACTION_ZHANLI_DOWN = 1;
static const int ACTION_ZHANLI_UP = 2;
static const int ACTION_XINGZOU = 3;
static const int ACTION_XINGZOU_DOWN = 4;
static const int ACTION_XINGZOU_UP = 5;

static const int ACTION_ZUOQI_ZHANLI = 6;
static const int ACTION_ZUOQI_ZHANLI_DOWN = 7;
static const int ACTION_ZUOQI_ZHANLI_UP = 8;
static const int ACTION_ZUOQI_XINGZOU = 9;
static const int ACTION_ZUOQI_XINGZOU_DOWN = 10;
static const int ACTION_ZUOQI_XINGZOU_UP = 11;

static const int ACTION_FEIXING = 12;
static const int ACTION_FEIXING_DOWN = 13;
static const int ACTION_FEIXING_UP = 14;

static const int ACTION_ZHANDOU_ZHANLI = 15;
static const int ACTION_ZHANDOU_GONGJI = 16;
static const int ACTION_ZHANDOU_QIANCHONG = 17;
static const int ACTION_ZHANDOU_SHANBI = 18;
static const int ACTION_ZHANDOU_SHOUSHANG = 19;
static const int ACTION_ZHANDOU_SHIFA = 20;
static const int ACTION_ZHANDOU_FANGYU = 21;
static const int ACTION_ZHANDOU_FANGYU_ZHANLI = 22;
static const int ACTION_ZHANDOU_GONGJI_BAD = 23;

static const int ACTION_LENGTH_PER_WEAPON = 24;

static const int MAX_ALLOCATE_ACTOR_NUMBER = 160;
static const int LAYER_RATIO = 12;
static const int ACTOR_CHUNK_DATA_LEN = 15;

#endif