#pragma once

namespace Pharos
{
	namespace Render
	{
		// 		enum RENDER_VAR_TYPE
		// 		{
		// 			TYPE_CONCRETE,
		// 			TYPE_TEXTURE,
		// 			TYPE_END,
		// 		};

// 		class RenderValue
// 		{
// 		public:
// 			RenderValue() { m_dataBuf = nullptr; m_dataSize = 0; }
// 			template<class T> RenderValue(const T& value) { m_dataSize = sizeof(value); m_dataBuf = &value; }
// 			virtual ~RenderValue() {	}
// 
// 		private:
// 			const void*			m_dataBuf;
// 			uint32				m_dataSize;
// 
// 		public:
// 			uint32 GetDataSize() const { return m_dataSize; }
// 			const void* GetDataPoint() const { return m_dataBuf; }
// 		};

		class RenderVariable
		{
		public:
			RenderVariable(const char8* name, uint32 slot)
			{
				m_name = name;
				m_slot = slot;
				//m_type = TYPE_END;

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

// 			void SetValue(const RenderValue& value)
// 			{
// 				uint32 dataSize = value.GetDataSize();
// 				const void* valueData = value.GetDataPoint();
// 
// 				m_dataBuf.CopyFrom(valueData, dataSize);
// 				m_dataSize = dataSize;
// 				m_dataChange = true;
// 			}

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