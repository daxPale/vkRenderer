#pragma once
#include "pch.h"
#include "Device.h"

class Model {
public:
	struct Vertex {
		glm::vec2 positon;

		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
	};
	Model(Device& device, const std::vector<Vertex>& vertices);
	~Model();

	Model(const Model&) = delete;
	Model(Model&&) = delete;

	Model& operator=(const Model&) = delete;
	Model& operator=(Model&&) = delete;

	void Bind(VkCommandBuffer commandBuffer);
	void Draw(VkCommandBuffer commandBuffer);
private:
	void CreateVertexBuffers(const std::vector<Vertex> vertices);
	Device& _device;
	VkBuffer _vertexBuffer;
	VkDeviceMemory _vertexBufferMemory;
	uint32_t _vertexCount;
};