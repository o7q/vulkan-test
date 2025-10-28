#include "vulkan.hpp"

void VulkanEngine::init()
{
    init_sdl();
    init_vk_instance();
    init_sdl_surface();
    init_vk_physical_device();
    init_vk_logical_device();
    init_vk_swap_chain();
    init_vk_image_views();
    init_vk_graphics_pipeline();
}

void VulkanEngine::cleanup()
{
    for (auto imageView : m_vk_swapChainImageViews)
    {
        vkDestroyImageView(m_vk_device, imageView, nullptr);
    }
    vkDestroySwapchainKHR(m_vk_device, m_vk_swapChain, nullptr);
    vkDestroyDevice(m_vk_device, nullptr);
    vkDestroySurfaceKHR(m_vk_instance, m_vk_surface, nullptr);
    vkDestroyInstance(m_vk_instance, nullptr);
    SDL_DestroyWindow(m_sdl_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
