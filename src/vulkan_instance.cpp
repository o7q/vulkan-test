#include "vulkan.hpp"

#include <iostream>

void VulkanEngine::init_vk_instance()
{
    VkApplicationInfo appInfo{};
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

    VkInstanceCreateInfo instanceCreateInfo{};
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
