#pragma once
#include "pch.h"
#include "Window.h"

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices 
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class Device 
{
public:
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
	Device(Window& window);

	Device(const Device&) = delete;
	Device(Device&&) = delete;

	Device& operator=(const Device&) = delete;
	Device& operator=(Device&&) = delete;

	~Device();

	VkCommandPool GetCommandPool() { return _commandPool; }
	VkDevice GetDevice() { return _device; }
	VkSurfaceKHR GetSurface() { return _surface; }
	VkQueue GetGraphicsQueue() { return _graphicsQueue; }
	VkQueue GetPesentQueue() { return _presentQueue; }

	SwapChainSupportDetails GetSwapChainSupport() { return QuerySwapChainSupport(_physicalDevice); }
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(_physicalDevice); }
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	// Buffer Helper Functions
	void CreateBuffer(
		VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkBuffer& buffer,
		VkDeviceMemory& bufferMemory);
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

	void CreateImageWithInfo(
		const VkImageCreateInfo& imageInfo,
		VkMemoryPropertyFlags properties,
		VkImage& image,
		VkDeviceMemory& imageMemory);

	VkPhysicalDeviceProperties _physicalDeviceProperties;
	VkPhysicalDeviceFeatures _physicalDeviceFeatures;
	VkPhysicalDeviceMemoryProperties _physicalDeviceMemoryProperties;

private:
	void CreateInstance();
	void SetupDebugMessenger();
	void CreateSurface();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateCommandPool();

	// helper functions
	bool IsDeviceSuitable(VkPhysicalDevice device);
	std::vector<const char*> GetRequiredExtensions();
	bool CheckValidationLayerSupport();
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void HasGflwRequiredInstanceExtensions();
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debugMessenger;
	VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
	Window& _window;
	VkCommandPool _commandPool;

	VkDevice _device;
	VkSurfaceKHR _surface;
	VkQueue _graphicsQueue;
	VkQueue _presentQueue;

	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};