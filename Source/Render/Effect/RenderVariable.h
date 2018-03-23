#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderVariable
		{
		public:
			RenderVariable(const char8* name, uint32 slot)
			{
				m_name = name;
				m_slot = slot;

				m_dataSize = 0;
				m_dataChange = false;

				m_texture = nullptr;
			}

			virtual ~RenderVariable()
			{
			}

		private:
			string				m_name;
			uint32				m_slot;

			MemoryBuffer		m_dataBuf;
			uint32				m_dataSize;		//申请占用的内存大小，可能跟实际数据大小不同			
			bool				m_dataChange;

			RenderTexture*		m_texture;

		public:
			template<class T>
			void SetValue(const T& value)
			{
				uint32 dataSize = sizeof(value); 
				const void* valueData = &value;
				
				m_dataBuf.CopyFrom(valueData, dataSize);
				m_dataSize = dataSize;
				m_dataChange = true;
			}

			void SetValue(RenderTexture* tex)
			{
				m_texture = tex;
				m_dataSize = 0;
				m_dataChange = true;
			}

			void SetDataSize(uint32 dataSize) { m_dataSize = dataSize; }
			uint32 GetDataSize() { return m_dataSize; }

			const char8* GetName() { return m_name.c_str(); }
			uint32 GetSlot() { return m_slot; }

			bool IsDataChange() { return m_dataChange; }
			void SetDataChange(bool changed) { m_dataChange = changed; }

			MemoryBuffer& GetMemoryData() { return m_dataBuf; }
			RenderTexture* GetTexture() { return m_texture; }
		};
	}
}