#include "pch.hpp"
#include "App.hpp"


int main(int, char**)
{
	App app(AppConfig{});
	app.Run();
}

// On Windows for Release build we don't want the console window to appear.

#ifdef _WIN32
#ifdef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	return main(__argc, __argv);
}
#endif
#endif