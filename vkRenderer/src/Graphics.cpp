#include "Graphics.h"

Graphics::Graphics(Window& window)
	:_window(window)
{
	CreatePipelineLayout();
	RecreateSwapChain();
	CreateCommandBuffers();
}

Graphics::~Graphics()
{
	vkDestroyPipelineLayout(_device.GetDevice(), _pipelineLayout, nullptr);
}

void Graphics::DrawFrame()
{
	uint32_t imageIndex;
	auto result = _swapChain->AcquireNextImage(&imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		RecreateSwapChain();
		return;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	RecordCommandBuffer(imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window.WasWindowResized()) {
		_window.ResetWindowResizedFlag();
		RecreateSwapChain();                                                                                                                                                                
		return;
	}

	result = _swapChain->SubmitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}
}

void Graphics::RecreateSwapChain()
{
	auto extent = _window.GetExtent();
	while (extent.width == 0 || extent.height == 0)
	{
		extent = _window.GetExtent();
		glfwWaitEvents();
	}

	WaitIdle();
	if (_swapChain == nullptr) {
		_swapChain = std::make_unique<SwapChain>(_device, extent);
	}else{
		_swapChain = std::make_unique<SwapChain>(_device, extent, std::move(_swapChain));
		if (_swapChain->ImageCount() != _commandBuffers.size()) {
			FreeCommandBuffers();
			CreateCommandBuffers();
		}
	}
		
	CreatePipeline();
}

void Graphics::CreatePipelineLayout()
{
	VkPipelineLayoutCreateInfo layoutCreateInfo{};
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutCreateInfo.setLayoutCount = 0;
	layoutCreateInfo.pSetLayouts = nullptr;
	layoutCreateInfo.pushConstantRangeCount = 0;
	layoutCreateInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(_device.GetDevice(), &layoutCreateInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void Graphics::CreatePipeline()
{
	PipelineConfigInfo pipelineConfig{};
	Pipeline::DefaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = _swapChain->GetRenderPass();
	pipelineConfig.pipelineLayout = _pipelineLayout;

	_pipeline = std::make_unique<Pipeline>(_device,
					"../../vkRenderer/res/shaders/vertex_shader.spv", 
					"../../vkRenderer/res/shaders/fragment_shader.spv", 
					pipelineConfig);
}

void Graphics::CreateCommandBuffers()
{
	_commandBuffers.resize(_swapChain->ImageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _device.GetCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

	if (vkAllocateCommandBuffers(_device.GetDevice(), &allocInfo, _commandBuffers.data()) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void Graphics::RecordCommandBuffer(int imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(_commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = _swapChain->GetRenderPass();
	renderPassInfo.framebuffer = _swapChain->GetFrameBuffer(imageIndex);

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = _swapChain->GetSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(_commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(_swapChain->GetSwapChainExtent().width);
	viewport.height = static_cast<float>(_swapChain->GetSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0, 0}, _swapChain->GetSwapChainExtent() };
	vkCmdSetViewport(_commandBuffers[imageIndex], 0, 1, &viewport);
	vkCmdSetScissor(_commandBuffers[imageIndex], 0, 1, &scissor);

	_pipeline->Bind(_commandBuffers[imageIndex]);  
	_model.Bind(_commandBuffers[imageIndex]);
	_model.Draw(_commandBuffers[imageIndex]);

	vkCmdEndRenderPass(_commandBuffers[imageIndex]);
	if (vkEndCommandBuffer(_commandBuffers[imageIndex]) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

void Graphics::FreeCommandBuffers()
{
	vkFreeCommandBuffers(
		_device.GetDevice(),
		_device.GetCommandPool(),
		static_cast<uint32_t>(_commandBuffers.size()),
		_commandBuffers.data());
	_commandBuffers.clear();
}
