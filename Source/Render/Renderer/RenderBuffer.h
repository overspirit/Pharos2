#pragma once

namespace Pharos
{
	namespace Render
	{
		enum BufferType
		{
			UNIFORM_BUFFFER = 0,
			VERTEX_BUFFER = 1,
			INDICES_BUFFER = 2,
			BUFFER_TYPE_NUM,
		};

		class RenderBuffer
		{
		public:
			virtual ~RenderBuffer() {}

		protected:
			BufferType		m_type;
			uint32			m_size;

		public:
			virtual BufferType GetBufferType() { return m_type; }

			virtual bool Allocate(uint32 bufSize, MemoryBuffer* buf = nullptr) = 0;

			virtual void CopyData(const MemoryBuffer& data, uint32 offset = 0) = 0;
			virtual void CopyData(const void* data, uint32 len, uint32 offset = 0) = 0;
		};
	}
}
