#include "vkRenderer.h"
#include "Device.h"
#include "SwapChain.h"

int main()
{
	//Application
	Application app(1200, 720, "Renderer");
	
	//Device
	Device device(app.GetWindow());
	
	//Swap Chain
	SwapChain swapChain(device, app.GetWindow().GetExtent());

	//Pipeline
	VkPipelineLayoutCreateInfo layoutCreateInfo{};
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutCreateInfo.setLayoutCount = 0;
	layoutCreateInfo.pSetLayouts = nullptr;
	layoutCreateInfo.pushConstantRangeCount = 0;
	layoutCreateInfo.pPushConstantRanges = nullptr;

	VkPipelineLayout pipelineLayout;
	if (vkCreatePipelineLayout(device.GetDevice(), &layoutCreateInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	auto pipelineConfig = Pipeline::DefaultPipelineConfigInfo(app.GetWindow().GetExtent());
	pipelineConfig.renderPass = swapChain.GetRenderPass(); 
	pipelineConfig.pipelineLayout = pipelineLayout;

	Pipeline pipeline(device, 
						"../../vkRenderer/res/shaders/vertex_shader.spv", 
						"../../vkRenderer/res/shaders/fragment_shader.spv", 
						pipelineConfig);

	//CommandBuffer
	std::vector<VkCommandBuffer> commandBuffers;
	commandBuffers.resize(swapChain.ImageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = device.GetCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(device.GetDevice(), &allocInfo, commandBuffers.data()) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	for (int i = 0; i < commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swapChain.GetRenderPass();
		renderPassInfo.framebuffer = swapChain.GetFrameBuffer(i);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChain.GetSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		pipeline.Bind(commandBuffers[i]);
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffers[i]);
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
	// 	app.Run();
	while (!app.GetWindow().ShouldClose())
	{
		glfwPollEvents();
		uint32_t imageIndex;
		auto result = swapChain.AcquireNextImage(&imageIndex);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		result = swapChain.SubmitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}
	}

	vkDeviceWaitIdle(device.GetDevice());

	vkDestroyPipelineLayout(device.GetDevice(), pipelineLayout, nullptr);
	app.Destroy();

	return 0;
}