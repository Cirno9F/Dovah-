#include <Dovah.h>

class Sandbox : public Dovah::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};

Dovah::Application* Dovah::CreateApplication()
{
	return new Sandbox();
}