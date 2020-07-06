﻿#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderVariable
		{
		public:
			RenderVariable(uint32 slot, RenderResourceSet* resSet)
			{				
				m_slot = slot;
				m_resSet = resSet;

				m_dataBuf = nullptr;
				m_dataOffset = 0;
			}

			RenderVariable(MemoryBuffer* membuf, uint32 offset)
			{
				m_dataBuf = membuf;
				m_dataOffset = offset;
				
				m_slot = 0;
				m_resSet = nullptr;
			}

			virtual ~RenderVariable()
			{
			}

		private:
			MemoryBuffer*		m_dataBuf;
			uint32				m_dataOffset;

			uint32					m_slot;
			RenderResourceSet*		m_resSet;

		public:
			template<class T>
			void SetValue(const T& value)
			{
				uint32 dataSize = sizeof(value); 
				const void* valueData = &value;				
				m_dataBuf->Insert(m_dataOffset, valueData, dataSize);
			}

			void SetValue(RenderTexture* tex)
			{
				m_resSet->SetFragmentTexture(m_slot, tex);
			}
		};
	}
}