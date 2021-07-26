#include "Window.h"

Window::Window(int width, int height, const std::string& name)
	:_width(width), _height(height), _name(name), _extent{width, height}
{
	InitWindow();
	_handle = glfwGetWin32Window(_window);
	_hInstance = GetModuleHandle(nullptr);
}

Window::~Window()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Window::InitWindow()
{
	int rc = glfwInit();
	assert(rc);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	_window = glfwCreateWindow(_width, _height, _name.c_str(), 0, 0);
	assert(_window);

	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, FramebufferResizedCallback);
}

void Window::FramebufferResizedCallback(GLFWwindow* window, int width, int height)
{
	auto windowPtr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	windowPtr->_framebufferResized = true;
	windowPtr->_width = width;
	windowPtr->_height = height;
}

