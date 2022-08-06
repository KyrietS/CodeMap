#include "App.hpp"

int main()
{
	auto app = std::make_unique<App>(AppConfig{});
	app->Run();
}