#include "vulkan.hpp"

#include <iostream>

void VulkanEngine::init()
{
    init_sdl();
    init_vk_instance();
    init_sdl_surface();
    m_device.init(&m_vk_instance, &m_vk_surface, &m_vk_layers);
    init_vk_swapChain();
    init_vk_imageViews();
    init_vk_renderPass();
    init_vk_graphicsPipeline();
    init_vk_framebuffers();
    init_vk_commandPool();
    init_vk_commandBuffers();
    init_vk_syncObjects();
}

void VulkanEngine::cleanup()
{
    cleanupSwapChain();

    vkDestroyPipeline(m_device.getVkDevice(), m_vk_graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(m_device.getVkDevice(), m_vk_pipelineLayout, nullptr);

    vkDestroyRenderPass(m_device.getVkDevice(), m_vk_renderPass, nullptr);

    for (size_t i = 0; i < m_vk_swapChainImages.size(); i++)
    {
        vkDestroySemaphore(m_device.getVkDevice(), m_vk_renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(m_device.getVkDevice(), m_vk_imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_device.getVkDevice(), m_vk_inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(m_device.getVkDevice(), m_vk_commandPool, nullptr);

    m_device.destroy();
    vkDestroySurfaceKHR(m_vk_instance, m_vk_surface, nullptr);
    vkDestroyInstance(m_vk_instance, nullptr);
    SDL_DestroyWindow(m_sdl_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
