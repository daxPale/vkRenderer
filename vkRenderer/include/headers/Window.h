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
	WindowExtent GetExtent() { return _extent; }
private:
	void InitWindow();
private:
	int _width;
	int _height;
	std::string _name;
	WindowExtent _extent;
	GLFWwindow* _window;
	HWND _handle;
	HINSTANCE _hInstance;
};