#pragma once

#include "Window.h"
#include "Dovah/LayerStack.h"
#include "Dovah/Events/Event.h"
#include "Dovah/Events/ApplicationEvent.h"

#include "Dovah/ImGui/ImGuiLayer.h"

#include "Dovah/Renderer/Shader.h"
#include "Dovah/Renderer/Buffer.h"

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

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}

