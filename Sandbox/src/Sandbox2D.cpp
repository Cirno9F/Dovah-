#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.f, true)
{
}

void Sandbox2D::OnAttach()
{
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

	m_FlatColorShader = Dovah::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Dovah::Timestep ts)
{
	//Update
	m_CameraController.OnUpdate(ts);

	//Render
	Dovah::RenderCommand::SetClearColor(glm::vec4{ 0.1f,0.1f,0.1f,1 });
	Dovah::RenderCommand::Clear();

	Dovah::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Dovah::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Dovah::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Dovah::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Dovah::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Dovah::Event& event)
{
}
