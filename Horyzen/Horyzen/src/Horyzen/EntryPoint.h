#pragma once

Horyzen::Application* CreateApplication();


int main(int argc, char** argv)
{
	Horyzen::Logger::Init();
	HORYZEN_LOG_INFO("Engine logger initialized!");
	APP_LOG_INFO("Application logger initialized!");

	HORYZEN_INIT_PROFILE_SESSION_AND_START("Profile Session - Application Initialization");
	Horyzen::Application* app = CreateApplication();
	app->Init();
	HORYZEN_END_PROFILE_SESSION();
	APP_LOG_INFO("Application initialized!");

	HORYZEN_INIT_PROFILE_SESSION("Profile Session - Application Run");
	app->Run();
	HORYZEN_END_PROFILE_SESSION();

	delete app;

	return 0;
}