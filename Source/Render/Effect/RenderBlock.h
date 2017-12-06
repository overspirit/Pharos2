#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderBlock
		{
		public:
			RenderBlock()
			{
				m_drawType = EDT_TRIANGLELIST;

				m_startIndex = 0;
				m_countNum = 0;
			}

			virtual ~RenderBlock()
			{

			}

		protected:
			DrawType		m_drawType;

			uint32			m_startIndex;
			uint32			m_countNum;

		public:
			virtual RenderTechnique* GetTechnique() = 0;
			virtual RenderLayout* GetLayout() = 0;
			virtual void SetLayoutRange(uint32 start, uint32 count) { m_startIndex = start; m_countNum = count; }
			virtual void SetDrawType(DrawType type) { m_drawType = type; }
			virtual uint32 GetLayoutDrawStart() { return m_startIndex; }
			virtual uint32 GetLayoutDrawCount() { return m_countNum; }

			virtual void ApplyToDevice() = 0;
		};
	}
}