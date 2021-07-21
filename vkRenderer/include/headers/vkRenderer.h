#pragma once
#include "pch.h"
#include "Window.h"
#include "Pipeline.h"

class Application {
public:
	Application(int width, int height, const std::string& name);

	Application(const Application&) = delete;
	Application(Application&&) = delete;

	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	virtual ~Application() = default;

	bool Init();
	void Run();
	void Destroy();

	Window& GetWindow() { return _window; }
private:
	int _width;
	int _height;
	std::string _name;
	Window _window{ _width, _height, _name };
};