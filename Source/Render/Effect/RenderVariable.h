#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderValue
		{
		public:
			RenderValue() { m_dataBuf = nullptr; m_dataSize = 0; }
			template<class T> RenderValue(const T& value) { m_dataSize = sizeof(value); m_dataBuf = &value; }
			virtual ~RenderValue() {	}

		private:
			const void*			m_dataBuf;
			uint32				m_dataSize;

		public:
			uint32 GetDataSize() const { return m_dataSize; }
			const void* GetDataPoint() const { return m_dataBuf; }
		};

		class RenderVariable
		{
		public:
			RenderVariable(const char8* name, uint32 slot)
			{
				m_name = name;
				m_slot = slot;
				//m_type = TYPE_END;

				m_owner = nullptr;
				m_dataSize = 0;
				m_bufOffset = 0;

				m_texture = nullptr;
			}

			virtual ~RenderVariable()
			{
			}

		private:
			string				m_name;
			uint32				m_slot;

			RenderShaderData*	m_owner;
			uint32				m_dataSize;			
			uint32				m_bufOffset;

			MemoryBuffer		m_tempBuf;

			RenderTexture*		m_texture;
		public:
			template<class T>
			void SetValue(const T& value)
			{
				uint32 dataSize = sizeof(value); 
				const void* valueData = &value;

				if (m_owner == nullptr)
				{
					m_tempBuf.CopyFrom(valueData, dataSize);
				}
				else
				{
					m_owner->CopyData(valueData, dataSize, m_bufOffset);
				}
			}

			void SetValue(const RenderValue& value)
			{
				uint32 dataSize = value.GetDataSize();
				const void* valueData = value.GetDataPoint();

				if (m_owner == nullptr)
				{
					m_tempBuf.CopyFrom(valueData, dataSize);
				}
				else
				{
					m_owner->CopyData(valueData, dataSize, m_bufOffset);
				}
			}

			void SetValue(RenderTexture* tex)
			{
				m_texture = tex;
			}

			void SetDataSize(uint32 dataSize) { m_dataSize = dataSize; }
			uint32 GetDataSize() { return m_dataSize; }
			
			void SetOwner(RenderShaderData* owner, uint32 offset)
			{
				m_owner = owner;
				m_bufOffset = offset;

				uint32 dataSize = m_tempBuf.GetLength();
				const void* valueData = m_tempBuf.GetPointer();
				m_owner->CopyData(valueData, dataSize, m_bufOffset);
			}

			const char8* GetName() { return m_name.c_str(); }
			uint32 GetSlot() { return m_slot; }
			RenderTexture* GetTexture() { return m_texture; }
		};

		
// 		enum RENDER_VAR_TYPE
// 		{
// 			TYPE_CONCRETE,
// 			TYPE_TEXTURE,
// 			TYPE_END,
// 		};



		/*
		class RenderVariable
		{
		public:
			RenderVariable(const char8* name, uint32 slot)
			{
				m_name = name;
				m_slot = slot;
				m_type = TYPE_END;
			}

			virtual ~RenderVariable()
			{
			}

		private:
			MemoryBuffer		m_dataBuf;
			RenderTexture*		m_tex;

			string				m_name;
			uint32				m_slot;
			RENDER_VAR_TYPE		m_type;

		public:
			virtual void SetValue(const RenderValue& value)
			{
				const void* data = value.GetDataPoint();
				uint32 size = value.GetDataSize();
				m_dataBuf.CopyFrom(data, size);
				m_type = TYPE_CONCRETE;
			}

			virtual void SetValue(RenderTexture* tex) 
			{
				m_tex = tex; 
				m_type = TYPE_TEXTURE;
			}

			virtual RENDER_VAR_TYPE GetVariableType() { return m_type; }

			const char8* GetName() { return m_name.c_str(); }
			uint32 GetSlot() { return m_slot; }
			const MemoryBuffer& GetMemoryBuffer() { return m_dataBuf; }
			RenderTexture* GetTexture() { return m_tex; }
		};*/
		
	}
}