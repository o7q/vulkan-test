#include "vulkan.hpp"

#include <iostream>

void VulkanEngine::init_vk_framebuffers()
{
    m_vk_swapChainFramebuffers.resize(m_vk_swapChainImageViews.size());

    for (size_t i = 0; i < m_vk_swapChainImageViews.size(); i++)
    {
        VkImageView attachments[] = {
            m_vk_swapChainImageViews[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_vk_renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_vk_swapChainExtent.width;
        framebufferInfo.height = m_vk_swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(m_vk_device, &framebufferInfo, nullptr, &m_vk_swapChainFramebuffers[i]) == VK_SUCCESS)
        {
            std::cout << '[' << i << "] Created the framebuffer successfully!\n";
        }
    }
}
