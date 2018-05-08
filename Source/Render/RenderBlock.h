#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderBlock
		{
		public:
			RenderBlock();
			virtual ~RenderBlock();
			
		protected:
			Renderer*			m_renderer;

			RenderShaderData*	m_blockData;

			RenderTechnique*	m_tech;
			RenderLayout*		m_layout;

			DrawType		m_drawType;

			uint32			m_startIndex;
			uint32			m_countNum;

		public:
			virtual void Init();

			virtual void SetBlockDataWorldMatrix(const Matrix4& world);
			virtual void SetBlockDataBoneMatrix(const Matrix4* bones, uint32 boneNum);

			virtual void BindLayout(RenderLayout* layout) { m_layout = layout; }
			virtual void BindTechnique(RenderTechnique* tech) { m_tech = tech; }

			virtual RenderTechnique* GetTechnique() { return m_tech; }
			virtual RenderLayout* GetLayout() { return m_layout; }
			virtual void SetLayoutRange(uint32 start, uint32 count) { m_startIndex = start; m_countNum = count; }
			virtual void SetDrawType(DrawType type) { m_drawType = type; }
			virtual uint32 GetLayoutDrawStart() { return m_startIndex; }
			virtual uint32 GetLayoutDrawCount() { return m_countNum; }

			virtual void ApplyToDevice();
		};
	}
}