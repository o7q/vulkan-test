#include "vulkan.hpp"

#include <iostream>
#include <algorithm>
#include <limits>

VkSurfaceFormatKHR VulkanEngine::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    for (const auto &availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR VulkanEngine::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    for (const auto &availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanEngine::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        SDL_GetWindowSize(m_sdl_window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void VulkanEngine::init_vk_swapChain()
{
    VulkanDevice::SwapChainSupportDetails swapChainSupport = m_device.getSwapChainSupportDetails();

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_vk_surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    VulkanDevice::QueueFamilyIndices indices = m_device.getQueueFamilyIndices();
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        // make the image shared across multiple queue families
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        // make the image exclusive to each queue family, and transfer ownership
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;     // optional
        createInfo.pQueueFamilyIndices = nullptr; // optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_device.getVkDevice(), &createInfo, nullptr, &m_vk_swapChain) == VK_SUCCESS)
    {
        std::cout << "Created the swap chain successfully!\n";
    }

    vkGetSwapchainImagesKHR(m_device.getVkDevice(), m_vk_swapChain, &imageCount, nullptr);
    m_vk_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device.getVkDevice(), m_vk_swapChain, &imageCount, m_vk_swapChainImages.data());

    m_vk_swapChainImageFormat = surfaceFormat.format;
    m_vk_swapChainExtent = extent;
}

void VulkanEngine::cleanupSwapChain()
{
    for (auto framebuffer : m_vk_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(m_device.getVkDevice(), framebuffer, nullptr);
    }

    for (auto imageView : m_vk_swapChainImageViews)
    {
        vkDestroyImageView(m_device.getVkDevice(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(m_device.getVkDevice(), m_vk_swapChain, nullptr);
}

void VulkanEngine::recreateSwapChain()
{
    vkDeviceWaitIdle(m_device.getVkDevice());

    cleanupSwapChain();

    init_vk_swapChain();
    init_vk_imageViews();
    init_vk_framebuffers();
}