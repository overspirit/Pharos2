#include "PreCompile.h"
#include "Pharos.h"

#if defined(_USE_DLMALLOC_)

#define MALLOC(s) dlmalloc(s) //malloc(s)
#define FREE(p) dlfree(p) //free(p)

#else

#define MALLOC(s) malloc(s)
#define FREE(p) free(p)

#endif

MemoryBuffer::MemoryBuffer(void)
{
	m_buffer = nullptr;
	m_buf_len = 0;
}

MemoryBuffer::MemoryBuffer(uint32 size)
{
	m_buffer = nullptr;
	m_buf_len = 0;
	this->Alloc(size);
}

MemoryBuffer::MemoryBuffer(const uint8* bytes, uint32 length)
{
	m_buffer = nullptr;
	m_buf_len = 0;
	this->CopyFrom(bytes, length);
}

MemoryBuffer::MemoryBuffer(const MemoryBuffer& other)
{
	m_buffer = nullptr;
	m_buf_len = 0;
	*this = other;
}

MemoryBuffer::~MemoryBuffer(void)
{
	this->Dalloc();
}

void MemoryBuffer::Alloc(uint32 size)
{
	m_buffer = (uint8*)MALLOC(size);
	m_buf_len = size;
}

void MemoryBuffer::Dalloc()
{
	if (m_buffer != NULL) FREE(m_buffer);
	m_buffer = nullptr;
	m_buf_len = 0;
}

MemoryBuffer MemoryBuffer::Clone() const
{
	return MemoryBuffer(*this);
}

void MemoryBuffer::ChangeSize(uint32 newSize)
{
	uint8* buffer = m_buffer;
	uint32 buf_len = m_buf_len;
	uint32 new_buf_size = Math::minimum(newSize, buf_len);

	this->Alloc(newSize);

	memcpy(m_buffer, buffer, new_buf_size);

	FREE(buffer);
}

void MemoryBuffer::CopyFrom(const void* bytes, uint32 length)
{
	if (bytes == nullptr || length == 0) return;

	if (this->IsEmpty())
	{
		this->Alloc(length);
		if (this->IsEmpty()) return;
	}

	uint32 copy_len = Math::minimum(m_buf_len, length);
	memcpy(m_buffer, bytes, copy_len);
}

void MemoryBuffer::CopyFrom(const MemoryBuffer& other)
{
	if (other.IsEmpty()) return;

	if (this->IsEmpty())
	{
		this->Alloc(other.GetLength());
		if (this->IsEmpty()) return;
	}

	uint32 copy_len = Math::minimum(m_buf_len, other.GetLength());
	memcpy(m_buffer, other.GetPointer(), copy_len);
}

void MemoryBuffer::Insert(uint32 startIndex, const void* bytes, uint32 length)
{
	if (bytes == nullptr || length == 0) return;
	if (this->IsEmpty()) return;
	if (startIndex >= m_buf_len) return;
	uint32 copy_len = Math::minimum(length, m_buf_len - startIndex);
	memcpy(m_buffer + startIndex, bytes, copy_len);
}

void MemoryBuffer::Insert(uint32 startIndex, const MemoryBuffer& other)
{
	if (other.IsEmpty()) return;
	if (this->IsEmpty()) return;
	if (startIndex >= m_buf_len) return;
	uint32 copy_len = Math::minimum(other.GetLength(), m_buf_len - startIndex);
	memcpy(m_buffer + startIndex, other.GetPointer(), copy_len);
}

MemoryBuffer& MemoryBuffer::operator = (const MemoryBuffer& other)
{
	if (!other.IsEmpty())
	{
		if (!this->IsEmpty()) this->Dalloc();

		this->Alloc(other.m_buf_len);
		if (!this->IsEmpty())
		{
			memcpy(m_buffer, other.m_buffer, m_buf_len);
		}
	}

	return *this;
}
