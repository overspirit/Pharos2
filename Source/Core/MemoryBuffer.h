#pragma once

namespace Pharos
{
	namespace Core
	{
		class MemoryBuffer
		{
		public:
			MemoryBuffer(void);
			MemoryBuffer(uint32 size);
			MemoryBuffer(const uint8* bytes, uint32 length);
			MemoryBuffer(const MemoryBuffer& other);
			~MemoryBuffer(void);

		public:
			uint8* m_buffer;
			uint32 m_buf_len;

		public:
			void Alloc(uint32 size);
			void Dalloc();

			virtual MemoryBuffer Clone() const;

			virtual void ChangeSize(uint32 newSize);

			virtual void CopyFrom(const void* bytes, uint32 length);
			virtual void CopyFrom(const MemoryBuffer& other);

			virtual void Insert(uint32 startIndex, const void* bytes, uint32 length);
			virtual void Insert(uint32 startIndex, const MemoryBuffer& other);

			MemoryBuffer& operator = (const MemoryBuffer& other);

			virtual void* GetPointer(uint32 offset = 0) const { return m_buffer + offset; }
			virtual uint32 GetLength() const { return m_buf_len; }
			virtual bool IsEmpty() const { return m_buffer == nullptr || m_buf_len == 0; }
		};
	}
}