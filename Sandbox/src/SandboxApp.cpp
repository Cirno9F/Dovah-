#include <Dovah.h>

#include "imgui/imgui.h"

class ExampleLayer :public Dovah::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		//triangle
		m_VertexArray.reset(Dovah::VertexArray::Create());
		float vertices[3 * 7] = {
			-0.5f,-0.5f,0.0f, 0.8f,0.2f,0.8f,1.0f,
			 0.5f,-0.5f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			 0.0f, 0.5f,0.0f, 0.8f,0.8f,0.2f,1.0f,
		};
		std::shared_ptr<Dovah::VertexBuffer> triangleVB;
		triangleVB.reset(Dovah::VertexBuffer::Create(vertices, sizeof(vertices)));
		Dovah::BufferLayout layout = {
			{ Dovah::ShaderDataType::Float3, "a_Position" },
			{ Dovah::ShaderDataType::Float4, "a_Color" },
			//{ ShaderDataType::Float3, "a_Normal" }
		};
		triangleVB->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(triangleVB);

		uint32_t indices[3] = { 0,1,2 };
		std::shared_ptr<Dovah::IndexBuffer> triangleIB;
		triangleIB.reset(Dovah::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(triangleIB);

		//square
		m_SquareVA.reset(Dovah::VertexArray::Create());
		float squareVertices[4 * 7] = {
			-0.75f,-0.75f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			 0.75f,-0.75f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			 0.75f, 0.75f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			-0.75f, 0.75f,0.0f, 0.2f,0.3f,0.8f,1.0f,
		};
		std::shared_ptr<Dovah::VertexBuffer> squareVB;
		squareVB.reset(Dovah::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout(layout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 0, 2, 3 };
		std::shared_ptr<Dovah::IndexBuffer> squareIB;
		squareIB.reset(Dovah::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		m_Shader.reset(new Dovah::Shader(vertexSrc, fragmentSrc));
	}

	void OnUpdate() override
	{
		if (Dovah::Input::IsKeyPressed(DOVAH_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed;
		else if (Dovah::Input::IsKeyPressed(DOVAH_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed;
		if (Dovah::Input::IsKeyPressed(DOVAH_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed;
		else if (Dovah::Input::IsKeyPressed(DOVAH_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed;

		if (Dovah::Input::IsKeyPressed(DOVAH_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed;
		if (Dovah::Input::IsKeyPressed(DOVAH_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed;

		Dovah::RenderCommand::SetClearColor(glm::vec4{ 0.1f,0.1f,0.1f,1 });
		Dovah::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Dovah::Renderer::BeginScene(m_Camera);

		Dovah::Renderer::Submit(m_Shader, m_SquareVA);
		Dovah::Renderer::Submit(m_Shader, m_VertexArray);

		Dovah::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{

	}

	void OnEvent(Dovah::Event& event) override
	{
		//Dovah::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<Dovah::KeyPressedEvent>(DOVAH_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Dovah::KeyPressedEvent& event)
	{
		//if (event.GetKeyCode() == DOVAH_KEY_LEFT)
		//	m_CameraPosition.x -= m_CameraSpeed;
		//if (event.GetKeyCode() == DOVAH_KEY_RIGHT)
		//	m_CameraPosition.x += m_CameraSpeed;
		//if (event.GetKeyCode() == DOVAH_KEY_DOWN)
		//	m_CameraPosition.y -= m_CameraSpeed;
		//if (event.GetKeyCode() == DOVAH_KEY_UP)
		//	m_CameraPosition.y += m_CameraSpeed;
		//
		//return false;
	}

private:
	std::shared_ptr<Dovah::Shader> m_Shader;
	std::shared_ptr<Dovah::VertexArray> m_VertexArray;
	std::shared_ptr<Dovah::VertexArray> m_SquareVA;

	Dovah::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.1f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 0.1f;
};

class Sandbox : public Dovah::Application
{
public:
	Sandbox() 
		:Application()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

Dovah::Application* Dovah::CreateApplication()
{
	return new Sandbox();
}