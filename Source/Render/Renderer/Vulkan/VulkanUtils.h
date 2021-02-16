#pragma once

#define CHECK_ENUM(i, e) assert(i == e)

namespace Pharos
{
	namespace Render
	{
		//VkFormat转换成EPixelFormat
		inline EPixelFormat VkFormat2PixelFormat(VkFormat fmt)
		{
			switch (fmt)
			{
			case VK_FORMAT_MAX_ENUM: return EPF_END;
					
			case VK_FORMAT_R32G32B32A32_SFLOAT: return EPF_RGBA32_FLOAT;
			case VK_FORMAT_R32G32B32A32_UINT: return EPF_RGBA32_UINT;
			case VK_FORMAT_R32G32B32A32_SINT: return EPF_RGBA32_SINT;

			case VK_FORMAT_R32G32_SFLOAT: return EPF_RG32_FLOAT;
			case VK_FORMAT_R32G32_UINT: return EPF_RG32_UINT;
			case VK_FORMAT_R32G32_SINT: return EPF_RG32_SINT;

			case VK_FORMAT_R32_SFLOAT: return EPF_R32_FLOAT;
			case VK_FORMAT_R32_UINT: return EPF_R32_UINT;
			case VK_FORMAT_R32_SINT: return EPF_R32_SINT;

			case VK_FORMAT_R16G16B16A16_SFLOAT: return EPF_RGBA16_FLOAT;
			case VK_FORMAT_R16G16B16A16_UINT: return EPF_RGBA16_UINT;
			case VK_FORMAT_R16G16B16A16_SINT: return EPF_RGBA16_SINT;

			case VK_FORMAT_R16G16_SFLOAT: return EPF_RG16_FLOAT;
			case VK_FORMAT_R16G16_UINT: return EPF_RG16_UINT;
			case VK_FORMAT_R16G16_SINT: return EPF_RG16_SINT;

			case VK_FORMAT_R16_SFLOAT: return EPF_R16_FLOAT;
			case VK_FORMAT_R16_UINT: return EPF_R16_UINT;
			case VK_FORMAT_R16_SINT: return EPF_R16_SINT;
					
			case VK_FORMAT_R8G8B8A8_UNORM: return EPF_RGBA8_UNORM;
			case VK_FORMAT_R8G8B8A8_SNORM: return EPF_RGBA8_SNORM;
			case VK_FORMAT_R8G8B8A8_UINT: return EPF_RGBA8_UINT;
			case VK_FORMAT_R8G8B8A8_SINT: return EPF_RGBA8_SINT;

			case VK_FORMAT_R8G8_UNORM: return EPF_RG8_UNORM;
			case VK_FORMAT_R8G8_SNORM: return EPF_RG8_SNORM;
			case VK_FORMAT_R8G8_UINT: return EPF_RG8_UINT;
			case VK_FORMAT_R8G8_SINT: return EPF_RG8_SINT;

			case VK_FORMAT_R8_UNORM: return EPF_R8_UNORM;
			case VK_FORMAT_R8_SNORM: return EPF_R8_SNORM;
			case VK_FORMAT_R8_UINT: return EPF_R8_UINT;
			case VK_FORMAT_R8_SINT: return EPF_R8_SINT;

			//case VK_FORMAT_R8_UNORM: return EPF_A8_UNORM;	//Vulkan 没有A8_UNORM

			case VK_FORMAT_B8G8R8A8_UNORM: return EPF_BGRA8_UNORM;
			case VK_FORMAT_B8G8R8A8_SRGB: return EPF_BGRA8_UNORM_SRGB;
					
			case VK_FORMAT_D32_SFLOAT_S8_UINT: return EPF_D32_FLOAT_S8_UINT;
			case VK_FORMAT_D32_SFLOAT: return EPF_D32_FLOAT;
			case VK_FORMAT_D24_UNORM_S8_UINT: return EPF_D24_UNORM_S8_UINT;
			case VK_FORMAT_D16_UNORM: return EPF_D16_UNORM;
			}

			assert(false);

			return EPF_END;
		}

		//EPixelFormat转换成VkFormat
		inline VkFormat PixelFormat2VkFormat(EPixelFormat pf)
		{
			CHECK_ENUM(0, EPF_RGBA32_FLOAT);
			CHECK_ENUM(1, EPF_RGBA32_UINT);
			CHECK_ENUM(2, EPF_RGBA32_SINT);

			CHECK_ENUM(3, EPF_RG32_FLOAT);
			CHECK_ENUM(4, EPF_RG32_UINT);
			CHECK_ENUM(5, EPF_RG32_SINT);

			CHECK_ENUM(6, EPF_R32_FLOAT);
			CHECK_ENUM(7, EPF_R32_UINT);
			CHECK_ENUM(8, EPF_R32_SINT);

			CHECK_ENUM(9, EPF_RGBA16_FLOAT);
			CHECK_ENUM(10, EPF_RGBA16_UINT);
			CHECK_ENUM(11, EPF_RGBA16_SINT);

			CHECK_ENUM(12, EPF_RG16_FLOAT);
			CHECK_ENUM(13, EPF_RG16_UINT);
			CHECK_ENUM(14, EPF_RG16_SINT);

			CHECK_ENUM(15, EPF_R16_FLOAT);
			CHECK_ENUM(16, EPF_R16_UINT);
			CHECK_ENUM(17, EPF_R16_SINT);

			CHECK_ENUM(18, EPF_RGBA8_UNORM);
			CHECK_ENUM(19, EPF_RGBA8_SNORM);
			CHECK_ENUM(20, EPF_RGBA8_UINT);
			CHECK_ENUM(21, EPF_RGBA8_SINT);

			CHECK_ENUM(22, EPF_RG8_UNORM);
			CHECK_ENUM(23, EPF_RG8_SNORM);
			CHECK_ENUM(24, EPF_RG8_UINT);
			CHECK_ENUM(25, EPF_RG8_SINT);

			CHECK_ENUM(26, EPF_R8_UNORM);
			CHECK_ENUM(27, EPF_R8_SNORM);
			CHECK_ENUM(28, EPF_R8_UINT);
			CHECK_ENUM(29, EPF_R8_SINT);

			CHECK_ENUM(30, EPF_A8_UNORM);

			CHECK_ENUM(31, EPF_BGRA8_UNORM);
			CHECK_ENUM(32, EPF_BGRA8_UNORM_SRGB);

			CHECK_ENUM(33, EPF_D32_FLOAT_S8_UINT);
			CHECK_ENUM(34, EPF_D32_FLOAT);
			CHECK_ENUM(35, EPF_D24_UNORM_S8_UINT);
			CHECK_ENUM(36, EPF_D16_UNORM);

			const static VkFormat formats[] =
			{					
				VK_FORMAT_R32G32B32A32_SFLOAT, //EPF_RGBA32_FLOAT;
				VK_FORMAT_R32G32B32A32_UINT, //EPF_RGBA32_UINT;
				VK_FORMAT_R32G32B32A32_SINT, //EPF_RGBA32_SINT;

				VK_FORMAT_R32G32_SFLOAT, //EPF_RG32_FLOAT;
				VK_FORMAT_R32G32_UINT, //EPF_RG32_UINT;
				VK_FORMAT_R32G32_SINT, //EPF_RG32_SINT;

				VK_FORMAT_R32_SFLOAT, //EPF_R32_FLOAT;
				VK_FORMAT_R32_UINT, //EPF_R32_UINT;
				VK_FORMAT_R32_SINT, //EPF_R32_SINT;

				VK_FORMAT_R16G16B16A16_SFLOAT, //EPF_RGBA16_FLOAT;
				VK_FORMAT_R16G16B16A16_UINT, //EPF_RGBA16_UINT;
				VK_FORMAT_R16G16B16A16_SINT, //EPF_RGBA16_SINT;

				VK_FORMAT_R16G16_SFLOAT, //EPF_RG16_FLOAT;
				VK_FORMAT_R16G16_UINT, //EPF_RG16_UINT;
				VK_FORMAT_R16G16_SINT, //EPF_RG16_SINT;

				VK_FORMAT_R16_SFLOAT, //EPF_R16_FLOAT;
				VK_FORMAT_R16_UINT, //EPF_R16_UINT;
				VK_FORMAT_R16_SINT, //EPF_R16_SINT;
						
				VK_FORMAT_R8G8B8A8_UNORM, //EPF_RGBA8_UNORM;
				VK_FORMAT_R8G8B8A8_SNORM, //EPF_RGBA8_SNORM;
				VK_FORMAT_R8G8B8A8_UINT, //EPF_RGBA8_UINT;
				VK_FORMAT_R8G8B8A8_SINT, //EPF_RGBA8_SINT;

				VK_FORMAT_R8G8_UNORM, //EPF_RG8_UNORM;
				VK_FORMAT_R8G8_SNORM, //EPF_RG8_SNORM;
				VK_FORMAT_R8G8_UINT, //EPF_RG8_UINT;
				VK_FORMAT_R8G8_SINT, //EPF_RG8_SINT;

				VK_FORMAT_R8_UNORM, //EPF_R8_UNORM;
				VK_FORMAT_R8_SNORM, //EPF_R8_SNORM;
				VK_FORMAT_R8_UINT, //EPF_R8_UINT;
				VK_FORMAT_R8_SINT, //EPF_R8_SINT;

				VK_FORMAT_R8_UNORM, //EPF_A8_UNORM;	//Vulkan 没有A8_UNORM

				VK_FORMAT_B8G8R8A8_UNORM, //EPF_BGRA8_UNORM;
				VK_FORMAT_B8G8R8A8_SRGB, //EPF_BGRA8_UNORM_SRGB;
						
				VK_FORMAT_D32_SFLOAT_S8_UINT, //EPF_D32_FLOAT_S8_UINT;
				VK_FORMAT_D32_SFLOAT, //EPF_D32_FLOAT;
				VK_FORMAT_D24_UNORM_S8_UINT, //EPF_D24_UNORM_S8_UINT;
				VK_FORMAT_D16_UNORM, //EPF_D16_UNORM;
			};

			return formats[pf];
		}

		//获取EPixelFormat数据大小
		inline uint32 GetPFSize(EPixelFormat pf)
		{
			switch (pf)
			{
			case EPF_RGBA32_FLOAT:
			case EPF_RGBA32_UINT:
			case EPF_RGBA32_SINT:
				return 16;

			case EPF_RG32_FLOAT:
			case EPF_RG32_UINT:
			case EPF_RG32_SINT:
				return 8;

			case EPF_R32_FLOAT:
			case EPF_R32_UINT:
			case EPF_R32_SINT:
				return 4;

			case EPF_RGBA16_FLOAT:
			case EPF_RGBA16_UINT:
			case EPF_RGBA16_SINT:
				return 8;

			case EPF_RG16_FLOAT:
			case EPF_RG16_UINT:
			case EPF_RG16_SINT:
				return 4;

			case EPF_R16_FLOAT:
			case EPF_R16_UINT:
			case EPF_R16_SINT:
				return 2;

			case EPF_RGBA8_UNORM:
			case EPF_RGBA8_SNORM:
			case EPF_RGBA8_UINT:
			case EPF_RGBA8_SINT:
				return 4;

			case EPF_RG8_UNORM:
			case EPF_RG8_SNORM:
			case EPF_RG8_UINT:
			case EPF_RG8_SINT:
				return 2;

			case EPF_R8_UNORM:
			case EPF_R8_SNORM:
			case EPF_R8_UINT:
			case EPF_R8_SINT:
				return 1;

			case EPF_A8_UNORM:
				return 1;

			case EPF_BGRA8_UNORM:
				return 4;

			case EPF_D32_FLOAT: return 4;
			case EPF_D24_UNORM_S8_UINT: return 4;
			case EPF_D16_UNORM: return 2;
			}

			//assert(false);

			return 0;
		}
		
		inline VkPipelineColorBlendStateCreateInfo GetVulkanBlendState(const BlendStateDesc& desc, VkPipelineColorBlendAttachmentState* attachState)
        {
			const static VkBlendFactor BLEND_FUNC_MAP[] = {
				VK_BLEND_FACTOR_ZERO,
				VK_BLEND_FACTOR_ONE,
				VK_BLEND_FACTOR_SRC_COLOR,
				VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
				VK_BLEND_FACTOR_SRC_ALPHA,
				VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
				VK_BLEND_FACTOR_DST_COLOR,
				VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
				VK_BLEND_FACTOR_DST_ALPHA,
				VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA};
				
			const static VkBlendOp BLEND_OP_MAP[] = {
				VK_BLEND_OP_ADD,
				VK_BLEND_OP_SUBTRACT,
				VK_BLEND_OP_REVERSE_SUBTRACT,
				VK_BLEND_OP_MIN,
				VK_BLEND_OP_MAX};
					
			attachState[0].colorWriteMask = 0xf;
			attachState[0].blendEnable = desc.blendEnable ? VK_TRUE : VK_FALSE;
            attachState[0].alphaBlendOp = BLEND_OP_MAP[desc.blendOpAlpha];
            attachState[0].colorBlendOp = BLEND_OP_MAP[desc.blendOp];
            attachState[0].srcColorBlendFactor = BLEND_FUNC_MAP[desc.srcBlend];
            attachState[0].dstColorBlendFactor = BLEND_FUNC_MAP[desc.destBlend];
            attachState[0].srcAlphaBlendFactor = BLEND_FUNC_MAP[desc.srcBlendAlpha];
            attachState[0].dstAlphaBlendFactor = BLEND_FUNC_MAP[desc.destBlendAlpha];
			
			VkPipelineColorBlendStateCreateInfo blendState;
			blendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			blendState.flags = 0;
			blendState.pNext = NULL;
			blendState.attachmentCount = 1;
			blendState.pAttachments = attachState;
			blendState.logicOpEnable = VK_FALSE;
			blendState.logicOp = VK_LOGIC_OP_NO_OP;
			blendState.blendConstants[0] = 1.0f;
			blendState.blendConstants[1] = 1.0f;
			blendState.blendConstants[2] = 1.0f;
			blendState.blendConstants[3] = 1.0f;

			return blendState;			
		}

		inline VkPipelineDepthStencilStateCreateInfo GetVulkanDepthStencilState(const DepthStencilStateDesc& desc)
		{
			const static VkCompareOp compareOps[] = {
				VK_COMPARE_OP_NEVER, //COMPARISON_NEVER
				VK_COMPARE_OP_LESS, //COMPARISON_LESS
				VK_COMPARE_OP_EQUAL, //COMPARISON_EQUAL
				VK_COMPARE_OP_LESS_OR_EQUAL, //COMPARISON_LESS_EQUAL
				VK_COMPARE_OP_GREATER, //COMPARISON_GREATER
				VK_COMPARE_OP_NOT_EQUAL, //COMPARISON_NOT_EQUAL
				VK_COMPARE_OP_GREATER_OR_EQUAL, //COMPARISON_GREATER_EQUAL
				VK_COMPARE_OP_ALWAYS, //COMPARISON_ALWAYS
			};

			const static VkStencilOp stencilOps[] = {
				VK_STENCIL_OP_KEEP, //STENCIL_OP_KEEP
				VK_STENCIL_OP_ZERO, //STENCIL_OP_ZERO
				VK_STENCIL_OP_REPLACE, //STENCIL_OP_REPLACE
				VK_STENCIL_OP_INCREMENT_AND_CLAMP, //STENCIL_OP_INCR_SAT
				VK_STENCIL_OP_DECREMENT_AND_CLAMP, //STENCIL_OP_DECR_SAT
				VK_STENCIL_OP_INVERT, //STENCIL_OP_INVERT
				VK_STENCIL_OP_INCREMENT_AND_WRAP, //STENCIL_OP_INCR
				VK_STENCIL_OP_DECREMENT_AND_WRAP, //STENCIL_OP_DECR
			};

			VkPipelineDepthStencilStateCreateInfo	depthStencilState;    
			depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencilState.pNext = NULL;
			depthStencilState.flags = 0;
			depthStencilState.depthTestEnable = desc.depthEnable ? VK_TRUE : VK_FALSE;
			depthStencilState.depthWriteEnable = VK_TRUE;
			depthStencilState.depthCompareOp = compareOps[desc.depthFunc];
			depthStencilState.depthBoundsTestEnable = VK_FALSE;

			depthStencilState.stencilTestEnable = desc.stencilEnable ? VK_TRUE : VK_FALSE;
			depthStencilState.back.failOp = stencilOps[desc.backStencilFailOp];
			depthStencilState.back.passOp = stencilOps[desc.backStencilPassOp];
			depthStencilState.back.compareOp = compareOps[desc.backStencilFunc];
			depthStencilState.back.compareMask = 0;
			depthStencilState.back.reference = 0;
			depthStencilState.back.depthFailOp = stencilOps[desc.backStencilDepthFailOp];
			depthStencilState.back.writeMask = 0;

			depthStencilState.front.failOp = stencilOps[desc.frontStencilFailOp];
			depthStencilState.front.passOp = stencilOps[desc.frontStencilPassOp];
			depthStencilState.front.compareOp = compareOps[desc.frontStencilFunc];
			depthStencilState.front.compareMask = 0;
			depthStencilState.front.reference = 0;
			depthStencilState.front.depthFailOp = stencilOps[desc.frontStencilDepthFailOp];
			depthStencilState.front.writeMask = 0;

			depthStencilState.minDepthBounds = 0;
			depthStencilState.maxDepthBounds = 0;

			return depthStencilState;
		}

		inline VkPipelineRasterizationStateCreateInfo GetVulkanRasterizationState(const RasterizerStateDesc& desc)
		{
			const static VkPolygonMode polygonMode[] = {
					VK_POLYGON_MODE_LINE, //FILL_WIREFRAME
					VK_POLYGON_MODE_FILL, //FILL_SOLID
			};

			const static VkCullModeFlagBits cullMode[] = {
					VK_CULL_MODE_NONE, //CULL_NONE
					VK_CULL_MODE_FRONT_BIT, //CULL_FRONT
					VK_CULL_MODE_BACK_BIT, //CULL_BACK
			};

			VkPipelineRasterizationStateCreateInfo rasterState;			
			rasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterState.pNext = NULL;
			rasterState.flags = 0;
			rasterState.polygonMode = polygonMode[desc.fillMode];
			rasterState.cullMode = cullMode[desc.cullMode];
			rasterState.frontFace = desc.frontCounterClockwise ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
			rasterState.depthClampEnable = VK_FALSE;
			rasterState.rasterizerDiscardEnable = VK_FALSE;
			rasterState.depthBiasEnable = VK_FALSE;
			rasterState.depthBiasConstantFactor = 0;
			rasterState.depthBiasClamp = 0;
			rasterState.depthBiasSlopeFactor = 0;
			rasterState.lineWidth = 1.0f;

			return rasterState;

		}
    }
}
