#include "dovahpch.h"
#include "Application.h"

#include "Dovah/Log.h"

#include <Dovah/Renderer/Renderer.h>

#include "Input.h"

 
namespace Dovah
{
#define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() 
		:m_Camera(-1.6f,1.6f,-0.9f,0.9f)
	{
		DOVAH_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		//triangle
		m_VertexArray.reset(VertexArray::Create());
		float vertices[3 * 7] = {
			-0.5f,-0.5f,0.0f, 0.8f,0.2f,0.8f,1.0f,
			 0.5f,-0.5f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			 0.0f, 0.5f,0.0f, 0.8f,0.8f,0.2f,1.0f,
		};
		std::shared_ptr<VertexBuffer> triangleVB;
		triangleVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			//{ ShaderDataType::Float3, "a_Normal" }
		};
		triangleVB->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(triangleVB);

		uint32_t indices[3] = { 0,1,2 };
		std::shared_ptr<IndexBuffer> triangleIB;
		triangleIB.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(triangleIB);

		//square
		m_SquareVA.reset(VertexArray::Create());
		float squareVertices[4 * 7] = {
			-0.75f,-0.75f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			 0.75f,-0.75f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			 0.75f, 0.75f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			-0.75f, 0.75f,0.0f, 0.2f,0.3f,0.8f,1.0f,
		};
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout(layout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 0, 2, 3 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;		

			uniform mat4 u_ViewProjection;		

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
				
			in vec3 v_Position;
			in vec4 v_Color;	

			void main()
			{
				color = vec4(v_Position + 0.5,1.0);
				color = v_Color;
			}

		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Application::~Application() 
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));

		//DOVAH_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);

			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
	
		return true;
	}

	void Application::Run()
	{		
		while (m_Running)
		{
			RenderCommand::SetClearColor(glm::vec4{ 0.1f,0.1f,0.1f,1 });
			RenderCommand::Clear();

			m_Camera.SetPosition(glm::vec3{ 0.5f,0.0f,0.0f });
			m_Camera.SetRotation(10.0f);

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_Shader, m_SquareVA);
			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window -> OnUpdate();
		}
	}
}