#pragma once

namespace Pharos
{
	namespace Render
	{
		enum RENDER_VAR_TYPE
		{
			TYPE_CONCRETE,
			TYPE_TEXTURE,
			TYPE_END,
		};

		class RenderValue
		{
		public:
			RenderValue() { m_dataBuf = nullptr; m_dataSize = 0; }
			template<class T> RenderValue(const T& value) { m_dataSize = sizeof(value); m_dataBuf = &value; }
			virtual ~RenderValue(){	}

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
		};
	}
}