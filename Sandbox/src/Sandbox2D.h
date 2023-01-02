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

	Dovah::Ref<Dovah::Texture2D> m_CheckerboardTexture;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};
	std::vector<ProfileResult> m_ProfileResults;

	glm::vec4 m_SquareColor = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);
};