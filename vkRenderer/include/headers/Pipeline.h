#pragma once
#include "pch.h"
#include "Device.h"
#include "Model.h"

struct PipelineConfigInfo
{
	PipelineConfigInfo(const PipelineConfigInfo&) = delete;
	PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

	VkPipelineViewportStateCreateInfo viewportInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	std::vector<VkDynamicState> dynamicStateEnables;
	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};

class Pipeline 
{
public:
	Pipeline(Device& device, const std::string& vertexPath, const std::string& fragmentPath, const PipelineConfigInfo& configInfo);

	Pipeline(const Pipeline&) = delete;
	Pipeline(Pipeline&&) = delete;

	Pipeline& operator=(const Pipeline&) = delete;
	Pipeline& operator=(Pipeline&&) = delete;

	~Pipeline();

	void Bind(VkCommandBuffer commmandBuffer);
	static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
private:
	void CreateGraphicsPipeline(const std::string& vertexPath, const std::string& fragmentPath, const PipelineConfigInfo& configInfo);
	void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
	static std::vector<char> ReadFile(const std::string& path);

	Device& _device;
	VkPipeline _graphicsPipeline;
	VkShaderModule _vertexShaderModule;
	VkShaderModule _fragmentShaderModule;
};