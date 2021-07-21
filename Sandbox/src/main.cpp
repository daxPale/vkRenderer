#include "vkRenderer.h"
#include "Device.h"
#include "SwapChain.h"

int main()
{
	Application app(1200, 720, "Renderer");

	Device device(app.GetWindow());
	SwapChain swapChain(device, app.GetWindow().GetExtent());

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

	app.Run();
	app.Destroy();

	return 0;
}