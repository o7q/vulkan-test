#pragma once

#include <optional>
#include <vector>
#include <cstdint>

#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include "device/vulkan_device.hpp"

class VulkanEngine
{
public:
    VulkanEngine()
    {
        init();

        bool running = true;
        while (running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                }
            }

            drawFrame();
        }

        vkDeviceWaitIdle(m_device.getVkDevice());
    }

    ~VulkanEngine()
    {
        cleanup();
    }

    void init();
    void cleanup();

private:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void drawFrame();

    void cleanupSwapChain();
    void recreateSwapChain();

private:
    void init_sdl();
    void init_vk_instance();
    void init_sdl_surface();
    void init_vk_swapChain();
    void init_vk_imageViews();
    void init_vk_renderPass();
    void init_vk_graphicsPipeline();
    void init_vk_framebuffers();
    void init_vk_commandPool();
    void init_vk_commandBuffers();
    void init_vk_syncObjects();

private:
    const std::vector<const char *> m_vk_layers = {
        "VK_LAYER_KHRONOS_validation"};

    // window
    SDL_Window *m_sdl_window = nullptr;
    VkSurfaceKHR m_vk_surface;
    //

    // instance
    VkInstance m_vk_instance = VK_NULL_HANDLE;
    //

    // device
    VulkanDevice m_device;
    //

    // swap chain
    VkSwapchainKHR m_vk_swapChain;
    std::vector<VkImage> m_vk_swapChainImages;
    std::vector<VkImageView> m_vk_swapChainImageViews;
    VkFormat m_vk_swapChainImageFormat;
    VkExtent2D m_vk_swapChainExtent;
    //

    // shaders
    VkShaderModule createShaderModule(const std::vector<char> &code);
    //

    // pipeline
    VkRenderPass m_vk_renderPass;
    VkPipelineLayout m_vk_pipelineLayout;
    VkPipeline m_vk_graphicsPipeline;
    //

    // framebuffers
    std::vector<VkFramebuffer> m_vk_swapChainFramebuffers;
    //

    // command buffers
    VkCommandPool m_vk_commandPool;
    std::vector<VkCommandBuffer> m_vk_commandBuffers;
    //

    // drawing
    std::vector<VkSemaphore> m_vk_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_vk_renderFinishedSemaphores;
    std::vector<VkFence> m_vk_inFlightFences;
    //

    uint32_t m_currentFrame = 0;
};
