cmake_minimum_required(VERSION 3.5.0)

project(Engine)

#引擎
include_directories(Source)

message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

set(CORE_FILES
        Core/File.cpp
        Core/Font.cpp
        Core/Image.cpp
        Core/Kernel.cpp
        Core/MemoryBuffer.cpp
        Core/Properties.cpp
        Core/ResBase.cpp
        Core/ResourceManager.cpp
        Core/XmlAttribute.cpp
        Core/XmlDocument.cpp
        Core/XmlNode.cpp
        Core/Linux/glfw_window.cpp
        Core/Linux/PlatformLinux.cpp
        Core/Linux/main.cpp
        Core/Linux/Timer.cpp
)

set(RENDER_FILES
        Render/Animation.cpp
        Render/Material.cpp
        Render/Mesh.cpp
        Render/Model.cpp
        Render/PostProcess/HDRPostProcess.cpp
        Render/PostProcess/ImageStatPostProcess.cpp
        Render/PostProcess/LensEffectsPostProcess.cpp
        Render/PostProcess/PostProcess.cpp
        Render/PostProcess/ToneMappingPostProcess.cpp
        Render/RenderBlock.cpp
        Render/RenderObject.cpp
        Render/Renderer/Vulkan/VulkanInitializeHelper.cpp
        Render/Renderer/Vulkan/VulkanBlendState.cpp        
        Render/Renderer/Vulkan/VulkanDepthStencilState.cpp        
        Render/Renderer/Vulkan/VulkanRasterizerState.cpp
        Render/Renderer/Vulkan/VulkanRenderCommand.cpp        
        Render/Renderer/Vulkan/VulkanRenderBuffer.cpp
        Render/Renderer/Vulkan/VulkanUniformBuffer.cpp
        Render/Renderer/Vulkan/VulkanRenderPipeline.cpp
        Render/Renderer/Vulkan/VulkanSamplerState.cpp
        Render/Renderer/Vulkan/VulkanRenderTarget.cpp
        Render/Renderer/Vulkan/VulkanRenderTexture.cpp
        Render/Renderer/Vulkan/VulkanShaderProgram.cpp
        Render/Renderer/Vulkan/VulkanRenderer.cpp
        Render/RenderMgr.cpp
        Render/Skeleton.cpp
)

set(DESKTOP_FILES
        Desktop/DesktopMgr.cpp
        Desktop/FontTextureManager.cpp
        Desktop/RenderFont.cpp
        Desktop/RenderImage.cpp
        Desktop/RenderSprite.cpp
        Desktop/UIObjects/Button.cpp
        Desktop/UIObjects/CheckButton.cpp
        Desktop/UIObjects/FontString.cpp
        Desktop/UIObjects/Frame.cpp
        Desktop/UIObjects/Progress.cpp
        Desktop/UIObjects/Slider.cpp
        Desktop/UIObjects/Texture.cpp
        Desktop/UIObjects/UIObject.cpp
        Desktop/UIObjects/WorldFrame.cpp
)

set(SCENE_FILES
        Scene/GridNode.cpp
        Scene/OctreeScene.cpp
        Scene/SceneCamera.cpp
        Scene/SceneImporter.cpp
        Scene/SceneMgr.cpp
        Scene/SceneNode.cpp
        Scene/TreeNode.cpp
        Scene/XmlSceneImporter.cpp
)

add_library(${PROJECT_NAME} STATIC ${CORE_FILES} ${RENDER_FILES} ${DESKTOP_FILES} ${SCENE_FILES})

target_link_libraries(${PROJECT_NAME} vulkan)
target_link_libraries(${PROJECT_NAME} xcb)
target_link_libraries(${PROJECT_NAME} ${EXTERNAL_LIBS})