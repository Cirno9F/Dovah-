#pragma once

#include "Window.h"
#include "Dovah/LayerStack.h"
#include "Dovah/Events/Event.h"
#include "Dovah/Events/ApplicationEvent.h"

#include "Dovah/Core/Timestep.h"

#include "Dovah/ImGui/ImGuiLayer.h"

namespace Dovah
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0;
	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}

