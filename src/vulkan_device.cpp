#include "vulkan.hpp"

#include <iostream>
#include <set>

bool VulkanEngine::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(m_vk_deviceExtensions.begin(), m_vk_deviceExtensions.end());

    for (const auto &extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool VulkanEngine::isDeviceSuitable(VkPhysicalDevice physicalDevice)
{
    QueueFamilyIndices indices = queryQueueFamilyIndices(physicalDevice);

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    bool validDeviceType = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
                           deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;

    bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    bool valid = indices.isComplete() && validDeviceType && extensionsSupported && swapChainAdequate;

    if (valid)
    {
        std::cout << "Found valid physical device: " << '[' << deviceProperties.deviceID << "] " << deviceProperties.deviceName << '\n';
    }

    return valid;
}

void VulkanEngine::init_vk_physical_device()
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

void VulkanEngine::init_vk_logical_device()
{
    QueueFamilyIndices queueFamilyIndices = queryQueueFamilyIndices(m_vk_physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &m_vk_physicalDeviceFeatures;

    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_vk_deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = m_vk_deviceExtensions.data();

    deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_vk_layers.size());
    deviceCreateInfo.ppEnabledLayerNames = m_vk_layers.data();

    if (vkCreateDevice(m_vk_physicalDevice, &deviceCreateInfo, nullptr, &m_vk_device) == VK_SUCCESS)
    {
        std::cout << "Created the logical Vulkan device successfully!\n";
    }

    vkGetDeviceQueue(m_vk_device, queueFamilyIndices.graphicsFamily.value(), 0, &m_vk_graphicsQueue);
    vkGetDeviceQueue(m_vk_device, queueFamilyIndices.presentFamily.value(), 0, &m_vk_presentQueue);
}
