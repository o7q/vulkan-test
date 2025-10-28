#include "vulkan.hpp"

#include <iostream>

void VulkanEngine::init_sdl()
{
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    m_sdl_window = SDL_CreateWindow("SuperDuperWindow", 1280, 720, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
}

void VulkanEngine::init_sdl_surface()
{
    if (SDL_Vulkan_CreateSurface(m_sdl_window, m_vk_instance, nullptr, &m_vk_surface) == VK_SUCCESS)
    {
        std::cout << "Created VulkanSDL surface successfully!\n";
    }
}
