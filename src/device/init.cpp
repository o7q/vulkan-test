#include "vulkan_device.hpp"

#include <iostream>
#include <set>

void VulkanDevice::init_physicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(*p_vk_instance, &deviceCount, nullptr);

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(*p_vk_instance, &deviceCount, devices.data());

    for (VkPhysicalDevice device : devices)
    {
        if (isDeviceSuitable(device))
        {
            m_vk_physicalDevice = device;
            break;
        }
    }

    m_queueFamilyIndices = queryQueueFamilyIndices(m_vk_physicalDevice);
    m_swapChainSupportDetails = querySwapChainSupport(m_vk_physicalDevice);
}

void VulkanDevice::init_logicalDevice()
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {m_queueFamilyIndices.graphicsFamily.value(), m_queueFamilyIndices.presentFamily.value()};

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

    if (p_vk_layers)
    {
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>((*p_vk_layers).size());
        deviceCreateInfo.ppEnabledLayerNames = (*p_vk_layers).data();
    }
    else
    {
        deviceCreateInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(m_vk_physicalDevice, &deviceCreateInfo, nullptr, &m_vk_device) == VK_SUCCESS)
    {
        std::cout << "[VulkanDevice] Created the logical Vulkan device successfully!" << std::endl;
    }

    vkGetDeviceQueue(m_vk_device, m_queueFamilyIndices.graphicsFamily.value(), 0, &m_vk_graphicsQueue);
    vkGetDeviceQueue(m_vk_device, m_queueFamilyIndices.presentFamily.value(), 0, &m_vk_presentQueue);
}
