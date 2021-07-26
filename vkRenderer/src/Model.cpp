#include "Model.h"

Model::Model(Device& device, const std::vector<Vertex>& vertices)
	:_device(device)
{
	CreateVertexBuffers(vertices);
}

Model::~Model()
{
	vkDestroyBuffer(_device.GetDevice(), _vertexBuffer, nullptr);
	vkFreeMemory(_device.GetDevice(), _vertexBufferMemory, nullptr);
}

void Model::Draw(VkCommandBuffer commandBuffer)
{
	vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
}

void Model::Bind(VkCommandBuffer commandBuffer)
{
	VkBuffer buffers[] = { _vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

}

void Model::CreateVertexBuffers(const std::vector<Vertex> vertices)
{
	_vertexCount = static_cast<uint32_t>(vertices.size());
	VkDeviceSize bufferSize = sizeof(vertices[0]) * _vertexCount;
	_device.CreateBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _vertexBuffer, _vertexBufferMemory);

	void* data;
	vkMapMemory(_device.GetDevice(), _vertexBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(_device.GetDevice(), _vertexBufferMemory);
}

std::vector<VkVertexInputBindingDescription> Model::Vertex::GetBindingDescriptions()
{
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescriptions;

}

std::vector<VkVertexInputAttributeDescription> Model::Vertex::GetAttributeDescriptions()
{
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, positon);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);
 
	return attributeDescriptions;
}
