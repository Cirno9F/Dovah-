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

	Dovah::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Dovah::Renderer2D::DrawQuad(glm::vec2{ 0.0f, 1.0f }, glm::vec2{ 1.0f, 2.0f }, glm::vec4{ 0.8f,0.2f,0.3f,1.0f });
	Dovah::Renderer2D::DrawQuad(glm::vec2{ 1.0f, 0.0f }, glm::vec2{ 1.0f, 0.5f }, glm::vec4{ 0.2f,0.3f,0.8f,1.0f });
	Dovah::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Dovah::Event& event)
{
	m_CameraController.OnEvent(event);
}
