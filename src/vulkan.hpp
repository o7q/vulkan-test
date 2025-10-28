#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

class VulkanEngine
{
public:
    VulkanEngine()
    {
        init();
    }

    ~VulkanEngine()
    {
        cleanup();
    }

    void init();
    void cleanup();

private:
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

    QueueFamilyIndices queryQueueFamilyIndices(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);

private:
    void init_sdl();
    void init_vk_instance();
    void init_sdl_surface();
    void init_vk_physical_device();
    void init_vk_logical_device();

    void init_vk_swap_chain()
    {
    }

private:
    const std::vector<const char *> m_vk_layers = {
        "VK_LAYER_KHRONOS_validation"};

    const std::vector<const char *> m_vk_deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    // window
    SDL_Window *m_sdl_window = nullptr;
    VkSurfaceKHR m_vk_surface;
    //

    // instance
    VkInstance m_vk_instance = VK_NULL_HANDLE;
    //

    // device
    VkDevice m_vk_device = VK_NULL_HANDLE;
    VkPhysicalDevice m_vk_physicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceFeatures m_vk_physicalDeviceFeatures = {};
    //

    // queue
    VkQueue m_vk_graphicsQueue;
    VkQueue m_vk_presentQueue;
    //
};
