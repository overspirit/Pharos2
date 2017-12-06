#pragma once

#define CHECK_ENUM_STR(v, i, e, str)	assert(i == e);	v.push_back(Utils::xor32(str))

namespace Pharos
{
	namespace Render
	{
		template<typename T, typename U>
		class StateEnumDefine : public Utils::Singleton < T >
		{
		public:
			StateEnumDefine()
			{

			}

		protected:
			vector<uint32> m_hashList;

		public:
			U FromStr(const char8* value)
			{
				uint32 checksum = Utils::xor32(value);
				for (uint32 i = 0; i < m_hashList.size(); i++)
				{
					if (m_hashList[i] == checksum)
					{
						return static_cast<U>(i);
					}
				}

				assert(false);
				return static_cast<U>(0xFFFFFFFF);
			}
		};

		class BoolDefine : public Utils::Singleton < BoolDefine >
		{
		public:
			BoolDefine()
			{
				m_hashList.push_back(Utils::xor32("false"));
				m_hashList.push_back(Utils::xor32("true"));
			}

		protected:
			vector<uint32> m_hashList;

		public:
			bool FromStr(const char8* value)
			{
				uint32 checksum = Utils::xor32(value);

				if (checksum == m_hashList[0])
				{
					return false;
				}
				else if (checksum == m_hashList[1])
				{
					return true;
				}

				assert(false);
				return false;
			}
		};

		class BlendFuncDefine : public StateEnumDefine < BlendFuncDefine, EBlendFunc >
		{
		public:
			BlendFuncDefine()
			{
				CHECK_ENUM_STR(m_hashList, 0, BLEND_ZERO, "zero");
				CHECK_ENUM_STR(m_hashList, 1, BLEND_ONE, "one");
				CHECK_ENUM_STR(m_hashList, 2, BLEND_SRC_COLOR, "src_color");
				CHECK_ENUM_STR(m_hashList, 3, BLEND_INV_SRC_COLOR, "inv_src_color");
				CHECK_ENUM_STR(m_hashList, 4, BLEND_SRC_ALPHA, "src_alpha");
				CHECK_ENUM_STR(m_hashList, 5, BLEND_INV_SRC_ALPHA, "inv_src_alpha");
				CHECK_ENUM_STR(m_hashList, 6, BLEND_DEST_COLOR, "dest_color");
				CHECK_ENUM_STR(m_hashList, 7, BLEND_INV_DEST_COLOR, "inv_dest_color");
				CHECK_ENUM_STR(m_hashList, 8, BLEND_DEST_ALPHA, "dest_alpha");
				CHECK_ENUM_STR(m_hashList, 9, BLEND_INV_DEST_ALPHA, "inv_dest_alpha");
			}
		};

		class BlendOPDefine : public StateEnumDefine < BlendOPDefine, EBlendOP >
		{
		public:
			BlendOPDefine()
			{
				CHECK_ENUM_STR(m_hashList, 0, BLEND_OP_ADD, "add");
				CHECK_ENUM_STR(m_hashList, 1, BLEND_OP_SUBTRACT, "sub");
				CHECK_ENUM_STR(m_hashList, 2, BLEND_OP_REV_SUBTRACT, "rev_sub");
				CHECK_ENUM_STR(m_hashList, 3, BLEND_OP_MIN, "min");
				CHECK_ENUM_STR(m_hashList, 4, BLEND_OP_MAX, "max");
			}
		};

		class DepthWriteTypeDefine : public StateEnumDefine < DepthWriteTypeDefine, EDepthWriteType >
		{
		public:
			DepthWriteTypeDefine()
			{
				CHECK_ENUM_STR(m_hashList, 0, DEPTH_WRITE_ZERO, "zero");
				CHECK_ENUM_STR(m_hashList, 1, DEPTH_WRITE_ALL, "all");
			}
		};

		class StencilOpDefine : public StateEnumDefine < StencilOpDefine, EStencilOp >
		{
		public:
			StencilOpDefine()
			{
				CHECK_ENUM_STR(m_hashList, 0, STENCIL_OP_KEEP, "keep");
				CHECK_ENUM_STR(m_hashList, 1, STENCIL_OP_ZERO, "zero");
				CHECK_ENUM_STR(m_hashList, 2, STENCIL_OP_REPLACE, "replace");
				CHECK_ENUM_STR(m_hashList, 3, STENCIL_OP_INCR_SAT, "incr_sat");
				CHECK_ENUM_STR(m_hashList, 4, STENCIL_OP_DECR_SAT, "decr_sat");
				CHECK_ENUM_STR(m_hashList, 5, STENCIL_OP_INVERT, "invert");
				CHECK_ENUM_STR(m_hashList, 6, STENCIL_OP_INCR, "incr");
				CHECK_ENUM_STR(m_hashList, 7, STENCIL_OP_DECR, "decr");
			}
		};

		class FillModeDefine : public StateEnumDefine < FillModeDefine, EFillMode >
		{
		public:
			FillModeDefine()
			{
				CHECK_ENUM_STR(m_hashList, 0, FILL_WIREFRAME, "wireframe");
				CHECK_ENUM_STR(m_hashList, 1, FILL_SOLID, "solid");
			}
		};

		class CullModeDefine : public StateEnumDefine < CullModeDefine, ECullMode >
		{
		public:
			CullModeDefine()
			{
				CHECK_ENUM_STR(m_hashList, 0, CULL_NONE, "none");
				CHECK_ENUM_STR(m_hashList, 1, CULL_FRONT, "front");
				CHECK_ENUM_STR(m_hashList, 2, CULL_BACK, "back");
			}
		};

		class FilterTypeDefine : public StateEnumDefine < FilterTypeDefine, EFilterType >
		{
		public:
			FilterTypeDefine()
			{
				CHECK_ENUM_STR(m_hashList, 0, FILTER_MIN_MAG_MIP_POINT, "min_mag_mip_point");
				CHECK_ENUM_STR(m_hashList, 1, FILTER_MIN_MAG_POINT_MIP_LINEAR, "min_mag_point_mip_linear");
				CHECK_ENUM_STR(m_hashList, 2, FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT, "min_point_mag_linear_mip_point");
				CHECK_ENUM_STR(m_hashList, 3, FILTER_MIN_POINT_MAG_MIP_LINEAR, "min_point_mag_mip_linear");
				CHECK_ENUM_STR(m_hashList, 4, FILTER_MIN_LINEAR_MAG_MIP_POINT, "min_linear_mag_mip_point");
				CHECK_ENUM_STR(m_hashList, 5, FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, "min_linear_mag_point_mip_linear");
				CHECK_ENUM_STR(m_hashList, 6, FILTER_MIN_MAG_LINEAR_MIP_POINT, "min_mag_linear_mip_point");
				CHECK_ENUM_STR(m_hashList, 7, FILTER_MIN_MAG_MIP_LINEAR, "min_mag_mip_linear");
				CHECK_ENUM_STR(m_hashList, 8, FILTER_ANISOTROPIC, "anisotropic");
			}
		};

		class TextureAddressModeDefine : public StateEnumDefine < TextureAddressModeDefine, ETextureAddressMode >
		{
		public:
			TextureAddressModeDefine()
			{
				CHECK_ENUM_STR(m_hashList, 0, TEXTURE_ADDRESS_WRAP, "wrap");
				CHECK_ENUM_STR(m_hashList, 1, TEXTURE_ADDRESS_MIRROR, "mirror");
				CHECK_ENUM_STR(m_hashList, 2, TEXTURE_ADDRESS_CLAMP, "clamp");
				CHECK_ENUM_STR(m_hashList, 3, TEXTURE_ADDRESS_BORDER, "border");
			}
		};

		class ComparisonFuncDefine : public StateEnumDefine < ComparisonFuncDefine, EComparisonFunc >
		{
		public:
			ComparisonFuncDefine()
			{
				CHECK_ENUM_STR(m_hashList, 0, COMPARISON_NEVER, "never");
				CHECK_ENUM_STR(m_hashList, 1, COMPARISON_LESS, "less");
				CHECK_ENUM_STR(m_hashList, 2, COMPARISON_EQUAL, "equal");
				CHECK_ENUM_STR(m_hashList, 3, COMPARISON_LESS_EQUAL, "less_equal");
				CHECK_ENUM_STR(m_hashList, 4, COMPARISON_GREATER, "greater");
				CHECK_ENUM_STR(m_hashList, 5, COMPARISON_NOT_EQUAL, "not_equal");
				CHECK_ENUM_STR(m_hashList, 6, COMPARISON_GREATER_EQUAL, "greater_equal");
				CHECK_ENUM_STR(m_hashList, 7, COMPARISON_ALWAYS, "always");
			}
		};

		class RenderEffectLoader
		{
		public:
			virtual ~RenderEffectLoader(){}

		protected:
			template<typename E, typename T>
			static void GetEnumFromStr(E* enumValue, T* enumDefine, const char8* strValue)
			{
				*enumValue = enumDefine->FromStr(strValue);
			}

			static bool GetBoolFromStr(const char8* strValue)
			{
				return BoolDefine::Inst()->FromStr(strValue);
			}

			static int32 GetIntFromStr(const char8* strValue)
			{
				return (int32)strtol(strValue, nullptr, 10);
			}

			static float32 GetFloatFromStr(const char8* strValue)
			{
				return (float32)strtod(strValue, nullptr);
			}

		protected:
			void FillRasterizerState(RasterizerStateDesc& rasterDesc, const char8* name, const char8* value)
			{
				if (strcmp(name, "fill_mode") == 0) GetEnumFromStr(&rasterDesc.fillMode, FillModeDefine::Inst(), value);
				else if (strcmp(name, "cull_mode") == 0) GetEnumFromStr(&rasterDesc.cullMode, CullModeDefine::Inst(), value);
				else if (strcmp(name, "front_face_ccw") == 0) rasterDesc.frontCounterClockwise = GetBoolFromStr(value);
				else if (strcmp(name, "depthBias") == 0) rasterDesc.depthBias = GetIntFromStr(value);
				else if (strcmp(name, "depthBiasClamp") == 0) rasterDesc.depthBiasClamp = GetFloatFromStr(value);
				else if (strcmp(name, "slopeScaledDepthBias") == 0) rasterDesc.slopeScaledDepthBias = GetFloatFromStr(value);
				else if (strcmp(name, "depth_clip_enable") == 0) rasterDesc.depthClipEnable = GetBoolFromStr(value);
				else if (strcmp(name, "scissor_enable") == 0) rasterDesc.scissorEnable = GetBoolFromStr(value);
				else if (strcmp(name, "multisample_enable") == 0) rasterDesc.multisampleEnable = GetBoolFromStr(value);
			}

			void FillBlendState(BlendStateDesc& blendDesc, const char8* name, const char8* value)
			{
				if (strcmp(name, "alpha_to_coverage_enable") == 0) blendDesc.alphaToCoverageEnable = GetBoolFromStr(value);
				else if (strcmp(name, "blend_enable") == 0) blendDesc.blendEnable = GetBoolFromStr(value);
				else if (strcmp(name, "blend_op") == 0) GetEnumFromStr(&blendDesc.blendOp, BlendOPDefine::Inst(), value);
				else if (strcmp(name, "src_blend") == 0) GetEnumFromStr(&blendDesc.srcBlend, BlendFuncDefine::Inst(), value);
				else if (strcmp(name, "dest_blend") == 0) GetEnumFromStr(&blendDesc.destBlend, BlendFuncDefine::Inst(), value);
				else if (strcmp(name, "blend_op_alpha") == 0) GetEnumFromStr(&blendDesc.blendOpAlpha, BlendOPDefine::Inst(), value);
				else if (strcmp(name, "src_blend_alpha") == 0) GetEnumFromStr(&blendDesc.srcBlendAlpha, BlendFuncDefine::Inst(), value);
				else if (strcmp(name, "dest_blend_alpha") == 0) GetEnumFromStr(&blendDesc.destBlendAlpha, BlendFuncDefine::Inst(), value);
			}

			void FillDepthStencilState(DepthStencilStateDesc& depthDesc, const char8* name, const char8* value)
			{
				if (strcmp(name, "depth_enable") == 0) depthDesc.depthEnable = GetBoolFromStr(value);
				else if (strcmp(name, "depth_write_mask") == 0) GetEnumFromStr(&depthDesc.depthWriteMask, DepthWriteTypeDefine::Inst(), value);
				else if (strcmp(name, "depth_func") == 0) GetEnumFromStr(&depthDesc.depthFunc, ComparisonFuncDefine::Inst(), value);
				else if (strcmp(name, "stencil_enable") == 0) depthDesc.stencilEnable = GetBoolFromStr(value);
				else if (strcmp(name, "front_stencil_ref") == 0) depthDesc.stencilRefValue = (uint32)GetIntFromStr(value);
				else if (strcmp(name, "front_stencil_write_mask") == 0) depthDesc.stencilWriteMask = (uint8)GetIntFromStr(value);
				else if (strcmp(name, "front_stencil_func") == 0) GetEnumFromStr(&depthDesc.frontStencilFunc, ComparisonFuncDefine::Inst(), value);
				else if (strcmp(name, "front_stencil_fail") == 0) GetEnumFromStr(&depthDesc.frontStencilFailOp, StencilOpDefine::Inst(), value);
				else if (strcmp(name, "front_stencil_depth_fail") == 0) GetEnumFromStr(&depthDesc.frontStencilDepthFailOp, StencilOpDefine::Inst(), value);
				else if (strcmp(name, "front_stencil_pass") == 0) GetEnumFromStr(&depthDesc.frontStencilPassOp, StencilOpDefine::Inst(), value);
				else if (strcmp(name, "back_stencil_func") == 0) GetEnumFromStr(&depthDesc.backStencilFunc, ComparisonFuncDefine::Inst(), value);
				else if (strcmp(name, "back_stencil_fail") == 0) GetEnumFromStr(&depthDesc.backStencilFailOp, StencilOpDefine::Inst(), value);
				else if (strcmp(name, "back_stencil_depth_fail") == 0) GetEnumFromStr(&depthDesc.backStencilDepthFailOp, StencilOpDefine::Inst(), value);
				else if (strcmp(name, "back_stencil_pass") == 0) GetEnumFromStr(&depthDesc.backStencilPassOp, StencilOpDefine::Inst(), value);
			}

		public:
			virtual bool Load(const char8* szPath) = 0;
			virtual uint32 GetRenderTechniqueNum() = 0;
			virtual RenderTechnique* GetRenderTechnique(uint32 index) = 0;
		};
	}
}