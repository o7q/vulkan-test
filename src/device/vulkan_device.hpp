#pragma once

#include <vector>
#include <optional>

#include <vulkan/vulkan.h>

class VulkanDevice
{
public:
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

public:
    void init(VkInstance *vk_instance, VkSurfaceKHR *vk_surface, const std::vector<const char *> *vk_layers = nullptr)
    {
        p_vk_instance = vk_instance;
        p_vk_surface = vk_surface;
        p_vk_layers = vk_layers;
        init_physicalDevice();
        init_logicalDevice();
    }

    void destroy()
    {
        vkDestroyDevice(m_vk_device, nullptr);
    }

    VkDevice &getVkDevice()
    {
        return m_vk_device;
    }

    VkQueue &getGraphicsQueue()
    {
        return m_vk_graphicsQueue;
    }

    VkQueue &getPresentQueue()
    {
        return m_vk_presentQueue;
    }

    QueueFamilyIndices &getQueueFamilyIndices()
    {
        return m_queueFamilyIndices;
    }

    SwapChainSupportDetails &getSwapChainSupportDetails()
    {
        return m_swapChainSupportDetails;
    }

private:
    const std::vector<const char *> m_vk_deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    bool isDeviceSuitable(VkPhysicalDevice physicalDevice);
    bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

    QueueFamilyIndices queryQueueFamilyIndices(VkPhysicalDevice physicalDevice);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice);

    void init_physicalDevice();
    void init_logicalDevice();

private:
    const std::vector<const char *> *p_vk_layers = nullptr;

    VkDevice m_vk_device = VK_NULL_HANDLE;
    VkPhysicalDevice m_vk_physicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceFeatures m_vk_physicalDeviceFeatures = {};

    VkInstance *p_vk_instance = VK_NULL_HANDLE;
    VkSurfaceKHR *p_vk_surface = VK_NULL_HANDLE;

    VkQueue m_vk_graphicsQueue;
    VkQueue m_vk_presentQueue;

    QueueFamilyIndices m_queueFamilyIndices;
    SwapChainSupportDetails m_swapChainSupportDetails;
};
