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

	VkRenderPass GetSwapChainRenderPass() const { return _swapChain->GetRenderPass(); }
	VkCommandBuffer GetCurrentCommandBuffer() const { return _commandBuffers[_currentFrameIndex]; }
	bool IsFrameInProgress() const { return _isFrameStarted; }
	int GetFrameIndex() const { return _currentFrameIndex; }

	VkCommandBuffer BeginFrame();
	void EndFrame();
	void BeginRenderPass(VkCommandBuffer commandBuffer);
	void EndRenderPass(VkCommandBuffer commandBuffer);

	void WaitIdle() { vkDeviceWaitIdle(_device.GetDevice()); }
	void DrawFrame();
private:
	void RecreateSwapChain();
	void CreatePipelineLayout();
	void CreatePipeline();
	void CreateCommandBuffers();
	void RecordCommandBuffer(int imageIndex);
	void FreeCommandBuffers();

	Window& _window;
	Device _device{_window};
	std::unique_ptr<SwapChain> _swapChain;
	VkPipelineLayout _pipelineLayout;
	std::unique_ptr<Pipeline> _pipeline;
	std::vector<VkCommandBuffer> _commandBuffers;

	uint32_t _currentImageIndex;
	int _currentFrameIndex = 0;
	bool _isFrameStarted = false;

	Model _model {_device, vertices};
};