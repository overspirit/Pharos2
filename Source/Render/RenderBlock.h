#pragma once

namespace Pharos
{
	namespace Render
	{
		//!!!注意,Block中的数据排列方式是
		//按照第一个位置是World Matrix,之后的位置是骨骼矩阵, 这样的排列方式设置数据的
		//即下列排列方式
		//struct
		//{
		//	Matrix4 world;
		//	Matrix4 bone[255];
		//}

		class RenderBlock
		{
		public:
			RenderBlock();
			virtual ~RenderBlock();
			
		private:
			struct BlockPatch
			{
				RenderTechnique*	tech;
				RenderLayout*		layout;

				DrawType		drawType;

				uint32			startIndex;
				uint32			countNum;
			};

			struct BlockData
			{
				Matrix4 world;
				Matrix4 bone[255];
			};

		private:
			Renderer*			m_renderer;

			RenderShaderData*	m_blockData;

			vector<BlockPatch>	m_blockPatchList;
			uint32				m_blockPatchNum;

		public:
			virtual void Init();

			virtual void SetBlockDataWorldMatrix(const Matrix4& world);
			virtual void SetBlockDataBoneMatrix(const Matrix4* bones, uint32 boneNum);

			virtual uint32 AddRenderBlockPatch(RenderLayout* layout, RenderTechnique* tech);
			virtual void SetBlockPatchDrawType(uint32 patchIndex, DrawType type);
			virtual void SetBlockPatchDrawRange(uint32 patchIndex, uint32 start, uint32 count);
			
			virtual uint32 GetRenderBlockPatchNum() { return m_blockPatchNum; }
			virtual RenderTechnique* GetBlockPatchTechnique(uint32 patchIndex);
			virtual RenderLayout* GetBlockPatchLayout(uint32 patchIndex);
			virtual DrawType GetBlockPatchDrawType(uint32 patchIndex);
			virtual uint32 GetBlockPatchDrawStart(uint32 patchIndex);
			virtual uint32 GetBlockPatchDrawCount(uint32 patchIndex);

			virtual void ApplyToDevice();
		};
	}
}