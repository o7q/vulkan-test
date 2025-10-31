#include "vulkan.hpp"

#include <iostream>

VkShaderModule VulkanEngine::createShaderModule(const std::vector<char> &code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(m_device.getVkDevice(), &createInfo, nullptr, &shaderModule) == VK_SUCCESS)
    {
        std::cout << "Created the shader module successfully!\n";
    }

    return shaderModule;
}
