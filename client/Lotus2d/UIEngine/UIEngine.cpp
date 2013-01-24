#include "UIEngine.h"
#include "Control.h"
#include "Base/Assert.h"

namespace Lotus2d {
	UIEngine::UIEngine():m_callback(0)
	{

	}

	UIEngine::~UIEngine()
	{
		
	}

	UIEngine* UIEngine::Instance()
	{
		static UIEngine s_instance;
		return &s_instance;
	}

	void UIEngine::setCallback(UICommandCallback pHandler)
	{
		m_callback = pHandler;
	}

	void UIEngine::handleUICommand(int command, Control* control)
	{
		ASSERT(m_callback);
		m_callback(command, control);
	}
}
