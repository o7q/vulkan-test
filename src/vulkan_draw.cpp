#include "vulkan.hpp"

#include <iostream>

void VulkanEngine::init_vk_syncObjects()
{
    m_vk_imageAvailableSemaphores.resize(M_MAX_FRAMES_IN_FLIGHT);
    m_vk_renderFinishedSemaphores.resize(M_MAX_FRAMES_IN_FLIGHT);
    m_vk_inFlightFences.resize(M_MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < M_MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(m_vk_device, &semaphoreInfo, nullptr, &m_vk_imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(m_vk_device, &semaphoreInfo, nullptr, &m_vk_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(m_vk_device, &fenceInfo, nullptr, &m_vk_inFlightFences[i]) != VK_SUCCESS)
        {

            std::cout << '[' << i << "] Created the semaphores successfully!\n";
        }
    }
}

void VulkanEngine::drawFrame()
{
    vkWaitForFences(m_vk_device, 1, &m_vk_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(m_vk_device, 1, &m_vk_inFlightFences[m_currentFrame]);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(m_vk_device, m_vk_swapChain, UINT64_MAX, m_vk_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);

    vkResetCommandBuffer(m_vk_commandBuffers[m_currentFrame], 0);
    recordCommandBuffer(m_vk_commandBuffers[m_currentFrame], imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pCommandBuffers = &m_vk_commandBuffers[m_currentFrame];

    VkSemaphore waitSemaphores[] = {m_vk_imageAvailableSemaphores[m_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;

    VkSemaphore signalSemaphores[] = {m_vk_renderFinishedSemaphores[m_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(m_vk_graphicsQueue, 1, &submitInfo, m_vk_inFlightFences[m_currentFrame]) != VK_SUCCESS)
    {
        std::cout << "Failed to submit draw command buffer!\n";
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {m_vk_swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // optional

    vkQueuePresentKHR(m_vk_presentQueue, &presentInfo);

    m_currentFrame = (m_currentFrame + 1) % M_MAX_FRAMES_IN_FLIGHT;
}