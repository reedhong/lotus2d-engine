/********************************************************************
	created:	2012/11/26
	filename: 	textfield.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef  __Lotus2d_textfield_H__
#define  __Lotus2d_textfield_H__

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#ifndef BOOL
#define BOOL int//signed char
#endif 

typedef enum _Textfield_INPUT_Method
{
	E_TF_IM_ABC, 
	E_TF_IM_NUM,   
	E_TF_IM_ALL
}Textfield_Input_Method;
	
	
typedef void (onInputMethodChange)();
typedef void (onTextChange)(char* utf8);
typedef void (onInputFinished)();
typedef void (onInputkey)(int key);
typedef void (onInpututf8)(char* utf8);

	
typedef void (setText)(const char* text);
typedef	char* (getText)();
typedef void (setExtent)(int x,int y,int w,int h);
typedef void (setVisiable)(BOOL visiable);
typedef void (setInputMethod)(Textfield_Input_Method mode);
typedef void (setInputPassword)(BOOL isPassword);
typedef void (setInputMaxLength)(int maxLength);
typedef void (setInputColor)(int argb);

	
typedef struct IInputEditObserver IInputEditObserver;
struct IInputEditObserver
{
	onInputMethodChange *inputMethodChange;
	onTextChange *textChange;
	onInputkey   *inputkey; 
	onInpututf8	 *inpututf8;
    onInputFinished *inputFinished;
};

typedef struct IInputEditInterface IInputEditInterface;

struct IInputEditInterface
{
	setText *_setText;
	getText *_getText;
	setExtent	*_setExtent;
	setVisiable	*_setVisiable;
	setInputMethod *_setInputMethod;
	setInputPassword *_setInputPassword;
	setInputMaxLength *_setInputMaxLength;
	setInputColor     *_setInputColor;
};

struct lotus2d_inputedit
{
	IInputEditObserver	*observer;
	IInputEditInterface	*delegate;
	void				*pEditor;				
};

extern struct lotus2d_inputedit	g_inputEditor;


void lotus2d_edit_init(void* pEditor);	

// 主要是给windows用的
void lotus2d_inputkey(int key);
void lotus2d_inpututf8(char* utf8);



#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif