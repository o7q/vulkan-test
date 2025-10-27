#pragma once

#include <iostream>
#include <optional>
#include <vector>
#include <set>

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

private:
    void init()
    {
        init_sdl();
        init_vk_instance();
        init_vk_surface();
        init_vk_physical_device();
        init_vk_logical_device();
    }

    void cleanup()
    {
        vkDestroyDevice(m_vk_device, nullptr);
        vkDestroySurfaceKHR(m_vk_instance, m_vk_surface, nullptr);
        vkDestroyInstance(m_vk_instance, nullptr);
        SDL_DestroyWindow(m_sdl_window);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    QueueFamilyIndices getQueueFamilyIndices(VkPhysicalDevice physicalDevice)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto &queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_vk_surface, &presentSupport);

            if (presentSupport)
            {
                indices.presentFamily = i;
            }

            i++;
        }

        return indices;
    }

    bool isDeviceSuitable(VkPhysicalDevice physicalDevice)
    {
        QueueFamilyIndices indices = getQueueFamilyIndices(physicalDevice);

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
        bool validDeviceType = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
                               deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;

        bool valid = indices.isComplete() && validDeviceType;

        if (valid)
        {
            std::cout << "Found valid physical device: " << '[' << deviceProperties.deviceID << "] " << deviceProperties.deviceName << '\n';
        }

        return valid;
    }

private:
    void init_sdl()
    {
        SDL_InitSubSystem(SDL_INIT_VIDEO);
        m_sdl_window = SDL_CreateWindow("SuperDuperWindow", 1280, 720, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    }

    void init_vk_instance()
    {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan Engine";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        Uint32 extensionCount = 0;
        const char *const *extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

        for (Uint32 i = 0; i < extensionCount; ++i)
        {
            std::cout << "Found extension: " << extensions[i] << '\n';
        }

        VkInstanceCreateInfo instanceCreateInfo = {};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledExtensionCount = extensionCount;
        instanceCreateInfo.ppEnabledExtensionNames = extensions;

        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_vk_layers.size());
        instanceCreateInfo.ppEnabledLayerNames = m_vk_layers.data();

        if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_vk_instance) == VK_SUCCESS)
        {
            std::cout << "Created the Vulkan instance successfully!\n";
        }
    }

    void init_vk_surface()
    {
        if (SDL_Vulkan_CreateSurface(m_sdl_window, m_vk_instance, NULL, &m_vk_surface) == VK_SUCCESS)
        {
            std::cout << "Created VulkanSDL surface successfully!\n";
        }
    }

    void init_vk_physical_device()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_vk_instance, &deviceCount, nullptr);

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_vk_instance, &deviceCount, devices.data());

        for (VkPhysicalDevice device : devices)
        {
            if (isDeviceSuitable(device))
            {
                m_vk_physicalDevice = device;
                break;
            }
        }
    }

    void init_vk_logical_device()
    {
        QueueFamilyIndices queueFamilyIndices = getQueueFamilyIndices(m_vk_physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.pEnabledFeatures = &m_vk_physicalDeviceFeatures;

        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_vk_layers.size());
        deviceCreateInfo.ppEnabledLayerNames = m_vk_layers.data();

        if (vkCreateDevice(m_vk_physicalDevice, &deviceCreateInfo, nullptr, &m_vk_device) == VK_SUCCESS)
        {
            std::cout << "Created the logical Vulkan device successfully!\n";
        }

        vkGetDeviceQueue(m_vk_device, queueFamilyIndices.graphicsFamily.value(), 0, &m_vk_graphicsQueue);
        vkGetDeviceQueue(m_vk_device, queueFamilyIndices.presentFamily.value(), 0, &m_vk_presentQueue);
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
