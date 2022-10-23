#include "Application.h"

#include "Dovah/Events/ApplicationEvent.h"
#include "Dovah/Log.h"

namespace Dovah
{
	Application::Application() {}
	Application::~Application() {}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		
		if (e.IsInCategory(EventCategoryApplication))
		{
			DOVAH_TRACE(e);
		}
		
		while (true)
		{

		}
	}
}