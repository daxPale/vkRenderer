#pragma once
#include "pch.h"
#include "Device.h"

struct PipelineConfigInfo
{
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
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

	static PipelineConfigInfo DefaultPipelineConfigInfo(Extent extent);
private:
	void CreateGraphicsPipeline(const std::string& vertexPath, const std::string& fragmentPath, const PipelineConfigInfo& configInfo);
	void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
	static std::vector<char> ReadFile(const std::string& path);

	Device& _device;
	VkPipeline _graphicsPipeline;
	VkShaderModule _vertexShaderModule;
	VkShaderModule _fragmentShaderModule;
};