#include "vkRenderer.h"

Application::Application(int width, int height, const std::string& name)
	:_height(height), _width(width), _name(name)
{

}

void Application::Run()
{
	while (!_window.ShouldClose())
	{
		glfwPollEvents();
		_graphics.DrawFrame();
	}
}

void Application::Destroy()
{
	_graphics.WaitIdle();
}

bool Application::Init()
{
	return true;
}