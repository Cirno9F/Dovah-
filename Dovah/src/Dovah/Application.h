#pragma once

#include "Core.h"
#include "Dovah/Events/Event.h"

namespace Dovah
{
	class DOVAH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}

