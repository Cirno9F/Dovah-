#include <Dovah.h>

//---Entry Point-----------------------------
#include <Dovah/Core/EntryPoint.h>
//-------------------------------------------

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

#include "Sandbox2D.h"
class ExampleLayer :public Dovah::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_CameraController(1280.0f/720.f, true)
	{
		//Quad
		m_VertexArray = Dovah::VertexArray::Create();
		float vertices[4 * 5] = {
			-0.5f,-0.5f,0.0f, 0.0f,0.0f,
			 0.5f,-0.5f,0.0f, 1.0f,0.0f,
			 0.5f, 0.5f,0.0f, 1.0f,1.0f,
			-0.5f, 0.5f,0.0f, 0.0f,1.0f,
		};										    
		Dovah::Ref<Dovah::VertexBuffer> triangleVB;
		triangleVB.reset(Dovah::VertexBuffer::Create(vertices, sizeof(vertices)));
		Dovah::BufferLayout layout = {
			{ Dovah::ShaderDataType::Float3, "a_Position" },
			{ Dovah::ShaderDataType::Float2, "a_TexCoord" }
		};
		triangleVB->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(triangleVB);

		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		Dovah::Ref<Dovah::IndexBuffer> triangleIB;
		triangleIB.reset(Dovah::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(triangleIB);


		//square
		m_SquareVA = Dovah::VertexArray::Create();
		float squareVertices[4 * 7] = {
			-0.5f,-0.5f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			 0.5f,-0.5f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			 0.5f, 0.5f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			-0.5f, 0.5f,0.0f, 0.2f,0.3f,0.8f,1.0f,
		};
		Dovah::Ref<Dovah::VertexBuffer> squareVB;
		squareVB.reset(Dovah::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		Dovah::BufferLayout squareLayout = {
			{ Dovah::ShaderDataType::Float3, "a_Position" },
			{ Dovah::ShaderDataType::Float4, "a_Color" },
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 0, 2, 3 };
		Dovah::Ref<Dovah::IndexBuffer> squareIB;
		squareIB.reset(Dovah::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;		

			uniform mat4 u_ViewProjection;		
			uniform mat4 u_Transform;		//Model Matrix

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
				
			in vec3 v_Position;
			in vec4 v_Color;	

			uniform vec4 u_Color;

			void main()
			{
				color = u_Color;
			}

		)";

		m_FlatColorShader = Dovah::Shader::Create("FlatColor", vertexSrc, fragmentSrc);
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		m_Texture2D = Dovah::Texture2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<Dovah::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Dovah::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Dovah::Timestep ts) override
	{
		//Update
		m_CameraController.OnUpdate(ts);


		if (Dovah::Input::IsKeyPressed(DOVAH_KEY_J))
			m_SquarePosition.x -= m_SquareMoveSpeed * ts;
		else if (Dovah::Input::IsKeyPressed(DOVAH_KEY_L))
			m_SquarePosition.x += m_SquareMoveSpeed * ts;
		if (Dovah::Input::IsKeyPressed(DOVAH_KEY_K))
			m_SquarePosition.y -= m_SquareMoveSpeed * ts;
		else if (Dovah::Input::IsKeyPressed(DOVAH_KEY_I))
			m_SquarePosition.y += m_SquareMoveSpeed * ts;


		//Render
		Dovah::RenderCommand::SetClearColor(glm::vec4{ 0.1f,0.1f,0.1f,1 });
		Dovah::RenderCommand::Clear();

		Dovah::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Dovah::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Dovah::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

		for (int y = -10; y < 10; y++)
		{
			for (int x = -10; x < 10; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Dovah::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		m_Texture2D->Bind();
		auto textureShader = m_ShaderLibrary.Get("Texture");
		std::dynamic_pointer_cast<Dovah::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Dovah::OpenGLShader>(textureShader)->UploadUniformFloat4("u_Color", m_QuadColor0);
	    Dovah::Renderer::Submit(textureShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_Texture2D->Bind();
		std::dynamic_pointer_cast<Dovah::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Dovah::OpenGLShader>(textureShader)->UploadUniformFloat4("u_Color", m_QuadColor1);
		Dovah::Renderer::Submit(textureShader, m_VertexArray, glm::translate(glm::mat4(1.0f), glm::vec3(0.5f))  * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Dovah::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::ColorEdit4("Quad Color 0", glm::value_ptr(m_QuadColor0));
		ImGui::ColorEdit4("Quad Color 1", glm::value_ptr(m_QuadColor1));
		ImGui::End();
	}

	void OnEvent(Dovah::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

private:
	Dovah::ShaderLibrary m_ShaderLibrary;
	Dovah::Ref<Dovah::VertexArray> m_VertexArray;
	Dovah::Ref<Dovah::VertexArray> m_SquareVA;
	
	Dovah::Ref<Dovah::Shader> m_FlatColorShader;
	Dovah::Ref<Dovah::Texture2D> m_Texture2D;

	Dovah::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePosition = glm::vec3(0.0f);
	float m_SquareMoveSpeed = 1.0f;
	glm::vec4 m_SquareColor = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);
	glm::vec4 m_QuadColor0 = glm::vec4(1.08f, 1.0f, 1.0f, 1.0f);
	glm::vec4 m_QuadColor1 = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);
};

class Sandbox : public Dovah::Application
{
public:
	Sandbox() 
		:Application()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox() {}
};

Dovah::Application* Dovah::CreateApplication()
{
	return new Sandbox();
}