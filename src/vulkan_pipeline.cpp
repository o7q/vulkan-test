#include "vulkan.hpp"

#include "utils.hpp"

void VulkanEngine::init_vk_graphics_pipeline()
{
    auto vertShaderCode = readFile("./src/vert.spv");
    auto fragShaderCode = readFile("./src/frag.spv");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    vkDestroyShaderModule(m_vk_device, fragShaderModule, nullptr);
    vkDestroyShaderModule(m_vk_device, vertShaderModule, nullptr);
}
