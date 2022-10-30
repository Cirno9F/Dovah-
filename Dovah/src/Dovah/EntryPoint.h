#pragma once

#ifdef DOVAH_PLATFORM_WINDOWS

extern Dovah::Application*  Dovah::CreateApplication();

int main(int argc, char** argv)
{
	Dovah::Log::Init();
	DOVAH_CORE_WARN("Initialized Log!");

	auto app = Dovah::CreateApplication();
	app->Run();
	delete app;

	//After closing app£¬shutdown engine Core
	//Dovah::ShutdownCore();
}

#endif