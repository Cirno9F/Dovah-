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
		//DOVAH_INFO("ExampleLayer::Update");

		if (Dovah::Input::IsKeyPressed(DOVAH_KEY_TAB))
			DOVAH_TRACE("Tab key is pressed! (poll)");
	}

	void OnEvent(Dovah::Event& event) override
	{
		//DOVAH_TRACE("{0}", event);

		if(event.GetEventType() == Dovah::EventType::KeyPressed)
		{
			Dovah::KeyPressedEvent& e = (Dovah::KeyPressedEvent&)event;

			if (e.GetKeyCode() == DOVAH_KEY_TAB)
				DOVAH_TRACE("Tab key is pressed! (event)");

			DOVAH_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Dovah::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Dovah::ImGuiLayer());
	}
	~Sandbox() {}
};

Dovah::Application* Dovah::CreateApplication()
{
	return new Sandbox();
}