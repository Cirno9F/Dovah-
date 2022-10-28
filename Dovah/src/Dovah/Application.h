#pragma once

#include "Window.h"
#include "Dovah/LayerStack.h"
#include "Dovah/Events/Event.h"
#include "Dovah/Events/ApplicationEvent.h"


namespace Dovah
{
	class DOVAH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}

