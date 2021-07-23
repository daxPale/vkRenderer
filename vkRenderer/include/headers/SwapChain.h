#pragma once
#include "Device.h"

class SwapChain {
public:
	static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

	SwapChain(Device& device, WindowExtent windowExtent);
	~SwapChain();

	SwapChain(const SwapChain&) = delete;
	SwapChain(SwapChain&&) = delete;

	SwapChain& operator=(const SwapChain&) = delete;
	SwapChain& operator=(SwapChain&&) = delete;

	VkFramebuffer GetFrameBuffer(int index) { return _swapChainFramebuffers[index]; }
	VkRenderPass GetRenderPass() { return _renderPass; }
	VkImageView GetImageView(int index) { return _swapChainImageViews[index]; }
	size_t ImageCount() { return _swapChainImages.size(); }
	VkFormat GetSwapChainImageFormat() { return _swapChainImageFormat; }
	VkExtent2D GetSwapChainExtent() { return _swapChainExtent; }
	uint32_t Width() { return _swapChainExtent.width; }
	uint32_t Height() { return _swapChainExtent.height; }

	float ExtentAspectRatio() {
		return static_cast<float>(_swapChainExtent.width) / static_cast<float>(_swapChainExtent.height);
	}
	VkFormat FindDepthFormat();

	VkResult AcquireNextImage(uint32_t* imageIndex);
	VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

private:
	void CreateSwapChain();
	void CreateImageViews();
	void CreateDepthResources();
	void CreateRenderPass();
	void CreateFramebuffers();
	void CreateSyncObjects();

	// Helper functions
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;

	std::vector<VkFramebuffer> _swapChainFramebuffers;
	VkRenderPass _renderPass;

	std::vector<VkImage> _depthImages;
	std::vector<VkDeviceMemory> _depthImageMemorys;
	std::vector<VkImageView> _depthImageViews;
	std::vector<VkImage> _swapChainImages;
	std::vector<VkImageView> _swapChainImageViews;

	Device& _device;
	VkExtent2D _windowExtent;

	VkSwapchainKHR _swapChain;

	std::vector<VkSemaphore> _imageAvailableSemaphores;
	std::vector<VkSemaphore> _renderFinishedSemaphores;
	std::vector<VkFence> _inFlightFences;
	std::vector<VkFence> _imagesInFlight;
	size_t currentFrame = 0;
};