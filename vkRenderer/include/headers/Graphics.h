#pragma once
#include "Device.h"
#include "SwapChain.h"
#include "Pipeline.h"
#include "Window.h"

class Graphics {
public:
	std::vector<Model::Vertex> vertices{
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}}
	};

	Graphics(Window& window);
	~Graphics();

	Graphics(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;

	Graphics& operator=(const Graphics&) = delete;
	Graphics& operator=(Graphics&&) = delete;

	void WaitIdle() { vkDeviceWaitIdle(_device.GetDevice()); }
	void DrawFrame();
private:
	void CreatePipelineLayout();
	void CreatePipeline();
	void CreateCommandBuffer();

	Window& _window;
	Device _device{_window};
	std::unique_ptr<SwapChain> _swapChain;
	VkPipelineLayout _pipelineLayout;
	std::unique_ptr<Pipeline> _pipeline;
	std::vector<VkCommandBuffer> _commandBuffers;

	Model _model {_device, vertices};
};