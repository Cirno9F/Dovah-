#include <Dovah.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"


class ExampleLayer :public Dovah::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		//Quad
		m_VertexArray.reset(Dovah::VertexArray::Create());
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
		m_SquareVA.reset(Dovah::VertexArray::Create());
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

		m_FlatColorShader.reset(Dovah::Shader::Create(vertexSrc, fragmentSrc));


		m_TextureShader.reset(Dovah::Shader::Create("assets/shaders/Texture.glsl"));
		m_Texture2D = Dovah::Texture2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<Dovah::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Dovah::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Dovah::Timestep ts) override
	{
		//DOVAH_TRACE("Delta time: {0} ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		if (Dovah::Input::IsKeyPressed(DOVAH_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Dovah::Input::IsKeyPressed(DOVAH_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		if (Dovah::Input::IsKeyPressed(DOVAH_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		else if (Dovah::Input::IsKeyPressed(DOVAH_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Dovah::Input::IsKeyPressed(DOVAH_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (Dovah::Input::IsKeyPressed(DOVAH_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;


		if (Dovah::Input::IsKeyPressed(DOVAH_KEY_J))
			m_SquarePosition.x -= m_SquareMoveSpeed * ts;
		else if (Dovah::Input::IsKeyPressed(DOVAH_KEY_L))
			m_SquarePosition.x += m_SquareMoveSpeed * ts;
		if (Dovah::Input::IsKeyPressed(DOVAH_KEY_K))
			m_SquarePosition.y -= m_SquareMoveSpeed * ts;
		else if (Dovah::Input::IsKeyPressed(DOVAH_KEY_I))
			m_SquarePosition.y += m_SquareMoveSpeed * ts;

		Dovah::RenderCommand::SetClearColor(glm::vec4{ 0.1f,0.1f,0.1f,1 });
		Dovah::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Dovah::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		//Dovah::MaterialRef material = new Dovah::Material(m_Shader);
		//Dovah::MaterialInstanceRef mi = new Dovah::MaterialInstance(material);
		//
		//mi->SetValue("u_Color", redColor);
		//mi->SetTexture("u_AlbedoMap", texture);
		//squareMesh->SetMaterial(mi);

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
		std::dynamic_pointer_cast<Dovah::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Dovah::OpenGLShader>(m_TextureShader)->UploadUniformFloat4("u_Color", m_QuadColor0);
	    Dovah::Renderer::Submit(m_TextureShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_Texture2D->Bind();
		std::dynamic_pointer_cast<Dovah::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Dovah::OpenGLShader>(m_TextureShader)->UploadUniformFloat4("u_Color", m_QuadColor1);
		Dovah::Renderer::Submit(m_TextureShader, m_VertexArray, glm::translate(glm::mat4(1.0f), glm::vec3(0.5f))  * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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
	}

	bool OnKeyPressedEvent(Dovah::KeyPressedEvent& event)
	{
	}

private:
	Dovah::Ref<Dovah::VertexArray> m_VertexArray;
	Dovah::Ref<Dovah::VertexArray> m_SquareVA;
	
	Dovah::Ref<Dovah::Shader> m_FlatColorShader, m_TextureShader;
	Dovah::Ref<Dovah::Texture2D> m_Texture2D;

	Dovah::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 1.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 10.0f;

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
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

Dovah::Application* Dovah::CreateApplication()
{
	return new Sandbox();
}