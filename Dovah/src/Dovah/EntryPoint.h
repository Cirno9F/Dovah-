#pragma once

#ifdef DOVAH_PLATFORM_WINDOWS

extern Dovah::Application*  Dovah::CreateApplication();

int main(int argc, char** argv)
{
	Dovah::Log::Init();
	DOVAH_CORE_WARN("Initialized Log!");
	int a = 5;
	DOVAH_INFO("Hello! Var={0}",a);

	auto app = Dovah::CreateApplication();
	app->Run();
	delete app;
}

#endif