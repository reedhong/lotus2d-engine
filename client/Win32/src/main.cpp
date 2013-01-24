/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2007, 2009, 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

#include <GLES/gl.h>
#include <EGL/egl.h>
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#include <stdio.h>

#include "Framework/Framework.h"
using namespace  Lotus2d;

#include "Game.h"

#include "ime.h"


#define WIDTH 800
#define HEIGTH 480

static BOOL s_keyPressed[256];
static BOOL bTrackLeave=FALSE;
static float s_scale = 1.0f;
static BOOL s_bActive = TRUE;
static CIme* pIme = NULL;
static Framework* g_framework;

DWORD tNOW;
DWORD tPRE;



LRESULT CALLBACK process_window(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
    switch(uiMsg) {
	case WM_CREATE:
		{
			pIme = new CIme;
			pIme->EnableIme();
		}
		break;
	case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

     case WM_KEYDOWN:
		{
			s_keyPressed[wParam] = TRUE;	
			if(wParam==VK_ESCAPE){
				g_framework->addAction(eRawActionGoBack,0, 0);
			}
		}
		break;
	case WM_KEYUP:				
		s_keyPressed[wParam] = FALSE;		
		break;
    case WM_ACTIVATE:
		{
			if (!HIWORD(wParam))					
			{
				s_bActive=TRUE;
			}
			else
			{
				s_bActive=FALSE;				
			}
			return 0;					
		}
		break;
	case WM_LBUTTONUP:
		{
			g_framework->addAction(eRawActionUp,lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			g_framework->addAction(eRawActionDown,lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);
		}
		break;
	case WM_MOUSEMOVE:
		{
		#if 0
	if (!bTrackLeave)
			{
				// 鼠标第一次移入窗口时， 请求一个WM_MOUSELEAVE 消息
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(tme);
				tme.hwndTrack = hWnd;
				tme.dwFlags = TME_LEAVE;
				TrackMouseEvent(&tme);
				bTrackLeave = TRUE;
			}

			int buttons = (int)wParam; 
			//必须是鼠标按下的拖拽事件
			if(buttons & MK_LBUTTON){
				addEvent(eTouch_Move,lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);

				int ptx = lParam & 0xFFFF;
				int pty = (lParam >> 16) & 0xFFFF;
				 
				if (!((ptx > 0) && (ptx < 0 + SIMULATOR_WIDTH) && (pty > 0) && (pty < SIMULATOR_HEIGHT)))
				{
					putEvent(Touch_Cancel,lParam & 0xFFFF, (lParam >> 16) & 0xFFFF, mScale);
				}
				 
			}
#endif
		}
		break;
	case WM_MOUSELEAVE:
		{
			bTrackLeave = FALSE;
			//putEvent(Touch_Cancel,lParam & 0xFFFF, (lParam >> 16) & 0xFFFF, mScale);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			g_framework->addAction((short)HIWORD(wParam)> 0?eRawActionPinchScaleDown : eRawActionPinchScaleUp,0, 0);
		}
		break;
	case WM_RBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_CHAR:
		{
			g_framework->addKey(wParam);
			//lotus2d_inputkey(wParam);
		}
		break;
case WM_IME_COMPOSITION:
		{
			if(pIme) return pIme->OnWM_IME_COMPOSITION(hWnd,lParam);
		}
		break;

	case WM_INPUTLANGCHANGEREQUEST:
		{
			if(pIme) return pIme->OnWM_INPUTLANGCHANGEREQUEST();
		}
		break;
	case WM_INPUTLANGCHANGE:
		{
			if(pIme) return pIme->OnWM_INPUTLANGCHANGE(hWnd);
		}
		break;
	default:
		break;
    }

    return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}


void platform(HWND *result, int width, int height) {
    WNDCLASS wc;
    RECT wRect;
    HWND hwnd;
    HINSTANCE hInstance;

    wRect.left = 0L;
    wRect.right = (long)width;
    wRect.top = 0L;
    wRect.bottom = (long)height;

    hInstance = GetModuleHandle(NULL);

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)process_window;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "OGLES";

    RegisterClass(&wc);

    AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        "OGLES", "Game-Win32", 
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
        0, 0, 
        wRect.right-wRect.left, wRect.bottom-wRect.top, 
        NULL, NULL, 
        hInstance, 
        NULL
        );
    *result = hwnd;
}

int main(void) {
    EGLDisplay m_eglDisplay;
    EGLContext m_eglContext;
    EGLSurface m_eglSurface;
    EGLint attributeList[] = { EGL_RED_SIZE, 1, EGL_DEPTH_SIZE, 1, EGL_NONE };
    EGLint		aEGLAttributes[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
        EGL_NONE
    };
    EGLint		aEGLContextAttributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 1,
        EGL_NONE
    };
    EGLConfig m_eglConfig[1];
    EGLint nConfigs;
    unsigned char mIndices[] = { 0, 1, 2 };
    signed short mVertices[] = {
        -50, -29, 0,
        50, -29, 0,
        0,  58, 0
    };
    HWND hwnd;
    HDC hdc;
    MSG sMessage;
    int bDone = 0;

    // Platform init.
    platform(&hwnd, WIDTH, HEIGTH);
    ShowWindow(hwnd, SW_SHOW);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    // EGL init.
    hdc = GetDC(hwnd);
    m_eglDisplay = eglGetDisplay(hdc);
    eglInitialize(m_eglDisplay, NULL, NULL);
    eglChooseConfig(m_eglDisplay, aEGLAttributes, m_eglConfig, 1, &nConfigs);
    printf("EGLConfig = %p\n", m_eglConfig[0]);
    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig[0], (NativeWindowType)hwnd, 0);
    m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig[0], EGL_NO_CONTEXT, aEGLContextAttributes);
    printf("EGLContext = %p\n", m_eglContext);
    eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

	g_framework = Game::Instance();
	char strAppPathName[1024];    
	GetCurrentDirectoryA(1024,strAppPathName);   
	g_framework->gameStartup(WIDTH, HEIGTH);

    // Main event loop
    while(!bDone)
    {
        // Do Windows stuff:
        if(PeekMessage(&sMessage, NULL, 0, 0, PM_REMOVE))
        {
            if(sMessage.message == WM_QUIT)
            {
                bDone = 1;
            }
            else 
            {
                TranslateMessage(&sMessage);
                DispatchMessage(&sMessage);
            }
		}else{
			if (s_bActive)						// 程序激活的么?
			{
				if (s_keyPressed[VK_F3])				// ESC 按下了么?
				{
					//running = FALSE;				// ESC 发出退出信号
				}
				else						// 不是退出的时候，刷新屏幕
				{
					tNOW = GetTickCount();

					if(tNOW - tPRE >= g_framework->getFrameInterverl()) //转到gameloop里
					{
						g_framework->gameFrame();
						eglSwapBuffers(m_eglDisplay, m_eglSurface);
						tPRE = tNOW;
					}else{
						Sleep(0);
					}
				}
			}
		}
    }

    // Exit.
    eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(m_eglDisplay, m_eglContext);
    eglDestroySurface(m_eglDisplay, m_eglSurface);
    eglTerminate(m_eglDisplay);

    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);


    return 0;
}
