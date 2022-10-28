#include <Dovah.h>

class ExampleLayer :public Dovah::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{

	}

	void OnUpdate() override
	{
		DOVAH_INFO("ExampleLayer::Update");
	}

	void OnEvent(Dovah::Event& event) override
	{
		DOVAH_TRACE("{0}", event);
	}
};

class Sandbox : public Dovah::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

Dovah::Application* Dovah::CreateApplication()
{
	return new Sandbox();
}