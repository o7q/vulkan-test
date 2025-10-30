#include "vulkan.hpp"

#include <iostream>

void VulkanEngine::init_vk_commandPool()
{
    QueueFamilyIndices queueFamilyIndices = queryQueueFamilyIndices(m_vk_physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(m_vk_device, &poolInfo, nullptr, &m_vk_commandPool) == VK_SUCCESS)
    {
        std::cout << "Created the command pool successfully!\n";
    }
}

void VulkanEngine::init_vk_commandBuffer()
{
    m_vk_commandBuffers.resize(M_MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_vk_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)m_vk_commandBuffers.size();

    if (vkAllocateCommandBuffers(m_vk_device, &allocInfo, m_vk_commandBuffers.data()) == VK_SUCCESS)
    {
        std::cout << "Allocated the command buffers successfully!\n";
    }
}

void VulkanEngine::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;                  // optional
    beginInfo.pInheritanceInfo = nullptr; // optional

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) == VK_SUCCESS)
    {
        std::cout << "Created the command buffer successfully!\n";
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_vk_renderPass;
    renderPassInfo.framebuffer = m_vk_swapChainFramebuffers[imageIndex];

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_vk_swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_vk_graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_vk_swapChainExtent.width);
    viewport.height = static_cast<float>(m_vk_swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_vk_swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) == VK_SUCCESS)
    {
        std::cout << "Recorded the command buffer successfully!\n";
    }
}
