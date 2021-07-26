#include "Graphics.h"

Graphics::Graphics(Window& window)
	:_window(window)
{
	_swapChain = std::make_unique<SwapChain>(_device,_window.GetExtent());
	CreatePipelineLayout();
	CreatePipeline();
	CreateCommandBuffer();
}

Graphics::~Graphics()
{
	vkDestroyPipelineLayout(_device.GetDevice(), _pipelineLayout, nullptr);
}

void Graphics::DrawFrame()
{
	uint32_t imageIndex;
	auto result = _swapChain->AcquireNextImage(&imageIndex);
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	result = _swapChain->SubmitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}
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
	auto pipelineConfig = Pipeline::DefaultPipelineConfigInfo(_window.GetExtent());
	pipelineConfig.renderPass = _swapChain->GetRenderPass(); 
	pipelineConfig.pipelineLayout = _pipelineLayout;

	_pipeline = std::make_unique<Pipeline>(_device,
					"../../vkRenderer/res/shaders/vertex_shader.spv", 
					"../../vkRenderer/res/shaders/fragment_shader.spv", 
					pipelineConfig);
}

void Graphics::CreateCommandBuffer()
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

	for (int i = 0; i < _commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _swapChain->GetRenderPass();
		renderPassInfo.framebuffer = _swapChain->GetFrameBuffer(i);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = _swapChain->GetSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		_pipeline->Bind(_commandBuffers[i]);
		_model.Bind(_commandBuffers[i]);
		_model.Draw(_commandBuffers[i]);

		vkCmdEndRenderPass(_commandBuffers[i]);
		if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}
