/*
 * textfield 
 * created by reedhong 2012.12.01
 */

#include "platform/textfield.h"
#include "RenderSystem/GLHeader.h"
#include "RenderSystem/Render.h"
using namespace Lotus2d;
#include "Base/Macros.h"
#import <UIKit/UIKit.h>

IInputEditInterface g_interface;

@interface NSIphoneEditor : NSObject
{
@public
	UITextField *textField;
	CGRect		 frame;
	BOOL		 visiable;
	Textfield_Input_Method mode;
	BOOL		 ispassword;
	NSString    *text; 
	int			 color;
	
	
}
@property (nonatomic,assign) UITextField* textField;
@property (nonatomic,assign) CGRect frame;
@property (nonatomic,assign) BOOL visiable;
@property (nonatomic,assign) Textfield_Input_Method mode;
@property (nonatomic,assign) BOOL ispassword;
@property (nonatomic,retain) NSString* text;
@property (nonatomic,assign) int color;

-(void) iphone_mainthread_setExtent;
-(void) iphone_mainthread_setVisiable;
-(void) iphone_mainthread_setInputMethod;
-(void) iphone_mainthread_setInputPassword;
-(void) iphone_mainthread_setText;
-(void) iphone_mainthread_setcolor;
@end

@implementation NSIphoneEditor
@synthesize textField,frame,visiable,mode,ispassword,text,color;

#define IPAD_EDIT_OFFSET    370
#define IPHONE_EDIT_OFFSET    158
-(void) iphone_mainthread_setExtent
{
	if(textField)
	{
		if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)//for ipad
		{
			if(frame.origin.y+frame.size.height<IPAD_EDIT_OFFSET)
			{
				[textField setBorderStyle:UITextBorderStyleNone];//不设置border
				textField.frame = frame;
				[textField setFont:[UIFont boldSystemFontOfSize:frame.size.height/2]];
				return;
			}
			else 
			{
                 
				glTranslatef(0,IPAD_EDIT_OFFSET-(frame.origin.y+frame.size.height),0);
				[textField setBorderStyle:UITextBorderStyleNone];//不设置border
				frame.origin.y += IPAD_EDIT_OFFSET-(frame.origin.y+frame.size.height);
				textField.frame = frame;
				[textField setFont:[UIFont boldSystemFontOfSize:frame.size.height/2]];
				return;
			}

		}
		else
		{
			if(frame.origin.y+frame.size.height<IPHONE_EDIT_OFFSET)
			{
				[textField setBorderStyle:UITextBorderStyleNone];//不设置border
				textField.frame = frame;
				[textField setFont:[UIFont boldSystemFontOfSize:frame.size.height/2]];
				return;
			}
			else
			{
				int scaleValue = [[UIScreen mainScreen] scale];
				int offset = (IPHONE_EDIT_OFFSET-(frame.origin.y+frame.size.height))*scaleValue;
				glTranslatef(0,offset,0);
				[textField setBorderStyle:UITextBorderStyleNone];//不设置border
				frame.origin.y += IPHONE_EDIT_OFFSET-(frame.origin.y+frame.size.height);
				textField.frame = frame;
				[textField setFont:[UIFont boldSystemFontOfSize:frame.size.height/2]];
				return;
			}

		}
/*
		if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)//for ipad
		{
			textField.frame = CGRectMake( 0, 1024-27, 1024, 30 );
		}
		else
		{
			textField.frame = CGRectMake( 0, 640-27, 480, 27 );
		}
		[textField setBorderStyle:UITextBorderStyleRoundedRect];//设置border
		
		[UIView beginAnimations:nil context:nil];
		[UIView setAnimationDuration:0.4f];
		
		if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)//for ipad
		{
			textField.frame = CGRectMake( 0, 334, 1024, 30 );
		}
		else
		{
			textField.frame = CGRectMake( 0, 131, 480, 27 );
		}
		[textField setFont:[UIFont boldSystemFontOfSize:textField.frame.size.height/2]];

		//textField.frame = frame;
		[UIView commitAnimations];
 */
	}
}

-(void) iphone_mainthread_setVisiable
{
	if(textField)
	{
		//UITextField *textField = (UITextField *)g_inputEditor.pEditor;
		//textField.hidden = !visiable;
		textField.hidden = !visiable;
		if(visiable)
		{
			[textField becomeFirstResponder];
		}
		else 
		{
			[textField resignFirstResponder];
		}

	}
}

-(void) iphone_mainthread_setInputPassword
{
	if(textField)
	{
		if(ispassword)
			textField.secureTextEntry = YES;
		else 
			textField.secureTextEntry = NO;
		
	}
}

-(void) iphone_mainthread_setInputMethod
{
	if(textField)
	{
		switch(mode)
		{
			case E_TF_IM_NUM:
				textField.keyboardType = UIKeyboardTypeNumberPad;
				break;
			case E_TF_IM_ABC:
				textField.keyboardType = UIKeyboardTypeASCIICapable;
				break;
			default:
				textField.keyboardType = UIKeyboardTypeDefault;
				break;
		}
	}
}

-(void)iphone_mainthread_setText
{
	textField.text = text;
}

-(void) iphone_mainthread_setcolor
{
	textField.textColor = [UIColor colorWithRed:((float)GL_COLOR_RED_BYTE(color))/255 green:((float)GL_COLOR_GREEN_BYTE(color))/255 
										   blue:((float)GL_COLOR_BLUE_BYTE(color))/255 alpha:((float)GL_COLOR_ALPHA_BYTE(color))/255];
}
@end


void iphone_setText(const char* text)
{
	if(g_inputEditor.pEditor)
	{
		NSIphoneEditor* editor = (NSIphoneEditor*)g_inputEditor.pEditor;
		editor.text = [ NSString stringWithUTF8String: text];
		[editor performSelectorOnMainThread:@selector(iphone_mainthread_setText) withObject:nil waitUntilDone:NO];
		//editor.textField.text = [ NSString stringWithUTF8String: (const char*)text];
	}
}

char* iphone_getText()
{
	/*
	if(g_inputEditor.pEditor)
	{
		NSIphoneEditor* editor = (NSIphoneEditor*)g_inputEditor.pEditor;
		return string_create1([editor.textField.text UTF8String],[editor.textField.text length]);
	}
	 */
	return NULL;
}

void iphone_setExtent(int x,int y,int w,int h)
{
	if(g_inputEditor.pEditor)
	{
		NSIphoneEditor* editor = (NSIphoneEditor*)g_inputEditor.pEditor;
        float scale = Render::Instance()->getScale();
		int scaleValue = [[UIScreen mainScreen] scale]/scale;
		editor.frame = CGRectMake( x/scaleValue, y/scaleValue, w/scaleValue, h/scaleValue );
		[editor performSelectorOnMainThread:@selector(iphone_mainthread_setExtent) withObject:nil waitUntilDone:NO];
	}
}

void iphone_setVisiable(BOOL visiable)
{
	if(g_inputEditor.pEditor)
	{
		NSIphoneEditor* editor = (NSIphoneEditor*)g_inputEditor.pEditor;
		editor.visiable = visiable;
		if(visiable)
			glPushMatrix();
		else
			glPopMatrix();

		[editor performSelectorOnMainThread:@selector(iphone_mainthread_setVisiable) withObject:nil waitUntilDone:NO];
	}
}

void iphone_setInputPassword(BOOL password)
{
	if(g_inputEditor.pEditor)
	{
		NSIphoneEditor* editor = (NSIphoneEditor*)g_inputEditor.pEditor;
		editor.ispassword = password;
		[editor performSelectorOnMainThread:@selector(iphone_mainthread_setInputPassword) withObject:nil waitUntilDone:NO];
	}
}

void iphone_setInputMaxLength(int maxLength)
{
	// TODO:
}

void iphone_setInputMethod(Textfield_Input_Method mode)
{
	if(g_inputEditor.pEditor)
	{
		NSIphoneEditor* editor = (NSIphoneEditor*)g_inputEditor.pEditor;
		editor.mode = mode;
		[editor performSelectorOnMainThread:@selector(iphone_mainthread_setInputMethod) withObject:nil waitUntilDone:NO];
	}
}

void iphone_setInputColor(int argb)
{
	if(g_inputEditor.pEditor)
	{
		NSIphoneEditor* editor = (NSIphoneEditor*)g_inputEditor.pEditor;
		editor.color = argb;
		[editor performSelectorOnMainThread:@selector(iphone_mainthread_setcolor) withObject:nil waitUntilDone:NO];
	}
}


void lotus2d_edit_init(void* pEditor)
{
	NSIphoneEditor* editor = [[NSIphoneEditor alloc] init];
	editor.textField = (UITextField*)pEditor;
	g_inputEditor.pEditor = editor; 
	g_interface._setText = iphone_setText;
	g_interface._getText = iphone_getText;
	g_interface._setExtent = iphone_setExtent;
	g_interface._setVisiable = iphone_setVisiable;
	g_interface._setInputMethod = iphone_setInputMethod;
	g_interface._setInputPassword = iphone_setInputPassword;
	g_interface._setInputMaxLength = iphone_setInputMaxLength;
	g_interface._setInputColor = iphone_setInputColor;
	g_inputEditor.delegate = &g_interface;
}
