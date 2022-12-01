#pragma once

#include "Dovah.h"

class Sandbox2D : public Dovah::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Dovah::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Dovah::Event& event) override;
private:
	Dovah::OrthographicCameraController m_CameraController;

	//Temp
	Dovah::Ref<Dovah::VertexArray> m_SquareVA;
	Dovah::Ref<Dovah::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);
};