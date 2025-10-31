#include "vulkan.hpp"

void VulkanEngine::init()
{
    init_sdl();
    init_vk_instance();
    init_sdl_surface();
    init_vk_physicalDevice();
    init_vk_logicalDevice();
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

    vkDestroyPipeline(m_vk_device, m_vk_graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(m_vk_device, m_vk_pipelineLayout, nullptr);

    vkDestroyRenderPass(m_vk_device, m_vk_renderPass, nullptr);

    for (size_t i = 0; i < m_vk_swapChainImages.size(); i++)
    {
        vkDestroySemaphore(m_vk_device, m_vk_renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(m_vk_device, m_vk_imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_vk_device, m_vk_inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(m_vk_device, m_vk_commandPool, nullptr);

    vkDestroyDevice(m_vk_device, nullptr);
    vkDestroySurfaceKHR(m_vk_instance, m_vk_surface, nullptr);
    vkDestroyInstance(m_vk_instance, nullptr);
    SDL_DestroyWindow(m_sdl_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
