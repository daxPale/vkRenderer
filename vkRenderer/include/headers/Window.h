#pragma once
#include "pch.h"

struct WindowExtent {
	int width;
	int height;
};

class Window 
{
public:
	Window(int width, int height, const std::string& name);

	Window(const Window&) = delete;
	Window(Window&&) = delete;

	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	~Window();

	GLFWwindow* GetWindow() { return _window; }
	HWND GetHandle() { return _handle; }
	HINSTANCE GetInstance() { return _hInstance; }
	bool ShouldClose() { return glfwWindowShouldClose(_window); }
	bool WasWindowResized() { return _framebufferResized; }
	void ResetWindowResizedFlag() { _framebufferResized = false; }
	WindowExtent GetExtent() { return WindowExtent{_width, _height}; }
private:
	void InitWindow();
	static void FramebufferResizedCallback(GLFWwindow* window, int width, int height);
private:
	int _width;
	int _height;
	std::string _name;
	bool _framebufferResized = false;
	GLFWwindow* _window;
	HWND _handle;
	HINSTANCE _hInstance;
};