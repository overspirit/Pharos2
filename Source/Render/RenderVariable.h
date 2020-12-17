#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderVariable
		{
		public:
			RenderVariable(uint32 slot, RenderResourceSet* resSet)
			{		
				m_dataBuf = nullptr;
				m_dataOffset = 0;

				m_slot = slot;
				m_tex = nullptr;
				m_resSet = resSet;	
			}

			RenderVariable(MemoryBuffer* membuf, uint32 offset)
			{
				m_dataBuf = membuf;
				m_dataOffset = offset;
				
				m_slot = 0;
				m_tex = nullptr;
				m_resSet = nullptr;
			}

			virtual ~RenderVariable()
			{
				SAFE_DELETE(m_tex);
			}

		private:
			MemoryBuffer*		m_dataBuf;
			uint32				m_dataOffset;

			uint32					m_slot;
			RenderTexture*			m_tex;
			RenderResourceSet*		m_resSet;

		public:
			template<class T>
			void SetValue(const T& value)
			{
				uint32 dataSize = sizeof(T); 
				const void* valueData = &value;				
				m_dataBuf->Insert(m_dataOffset, valueData, dataSize);
			}

			template<class T>
			void SetValue(const T* value, uint32 valueNum)
			{
				uint32 dataSize = sizeof(T) * valueNum; 
				m_dataBuf->Insert(m_dataOffset, value, dataSize);
			}

			void SetValue(RenderTexture* tex)
			{
				m_tex = tex;
				m_resSet->SetFragmentTexture(m_slot, tex);
			}
		};
	}
}