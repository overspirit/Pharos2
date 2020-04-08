#pragma once

#define MASKBITS 0x3F
#define MASKBYTE 0x80
#define MASK2BYTES 0xC0
#define MASK3BYTES 0xE0

namespace Pharos
{
	namespace Utils
	{
		inline char16 utf8_to_unicode(const char8* szSrc, const char8*& szOut)
		{
			if (*szSrc == 0) return 0;

			char16 word = 0;

			if (!(*szSrc & MASKBYTE))	// 0xxxxxxx
			{
				word = *szSrc;
				szOut = szSrc + 1;
			}
			else if ((*szSrc & MASK3BYTES) == MASK3BYTES)	// 1110xxxx 10xxxxxx 10xxxxxx
			{
				word = ((*szSrc & 0x0F) << 12) | ((*(szSrc + 1) & MASKBITS) << 6) | (*(szSrc + 2) & MASKBITS);
				szOut = szSrc + 3;
			}
			else if ((*szSrc & MASK2BYTES) == MASK2BYTES)	// 110xxxxx 10xxxxxx
			{
				word = ((*szSrc & 0x1F) << 6) | (*(szSrc + 1) & MASKBITS);
				szOut = szSrc + 2;
			}

			return word;
		}

		inline int32 utf8_to_unicode(const char8* szSrc, int32 nSrcLen, char16* strDest, int32 nDestLen)
		{
			if (nDestLen <= 0) return 0;

			int32 nLen = 0;
			while (*szSrc && nSrcLen && nDestLen)
			{
				if (!(*szSrc & MASKBYTE))	// 0xxxxxxx
				{
					*strDest++ = *szSrc++;
					nSrcLen--;
				}
				else if ((*szSrc & MASK3BYTES) == MASK3BYTES)	// 1110xxxx 10xxxxxx 10xxxxxx
				{
					*strDest++ = ((*szSrc & 0x0F) << 12) | ((*(szSrc + 1) & MASKBITS) << 6) | (*(szSrc + 2) & MASKBITS);
					szSrc += 3;
					nSrcLen -= 3;
				}
				else if ((*szSrc & MASK2BYTES) == MASK2BYTES)	// 110xxxxx 10xxxxxx
				{
					*strDest++ = ((*szSrc & 0x1F) << 6) | (*(szSrc + 1) & MASKBITS);
					szSrc += 2;
					nSrcLen -= 2;
				}

				nDestLen--;
				nLen++;
			}

			*strDest = 0; // 加上0结束符
			return nLen;
		}

		inline char8* unicode_to_utf8(char16 word, char8* buf)
		{
			if (word == 0) { *buf = 0; return buf; }

			if (word < MASKBYTE)	// 0xxxxxxx
			{
				*buf++ = (uint8)word;
			}
			else if (word < 0x800)	// 110xxxxx 10xxxxxx
			{
				*buf++ = ((uint8)(MASK2BYTES | word >> 6));
				*buf++ = ((uint8)((MASKBYTE | word) & MASKBITS));
			}
			else	// 1110xxxx 10xxxxxx 10xxxxxx
			{
				*buf++ = ((uint8)(MASK3BYTES | word >> 12));
				*buf++ = ((uint8)((MASKBYTE | word >> 6) & MASKBITS));
				*buf++ = ((uint8)((MASKBYTE | word) & MASKBITS));
			}

			return (char8*)buf;
		}

		inline int32 unicode_to_utf8(const char16* szSrc, int32 nSrcLen, char8* strDest, int32 nDestLen)
		{
			if (nDestLen <= 0) return 0;

			int32 nLen = 0;
			while (*szSrc && nSrcLen && nDestLen)
			{
				if (*szSrc < MASKBYTE)	// 0xxxxxxx
				{
					*strDest++ = (uint8)*szSrc;
					nDestLen--;
					nLen++;
				}
				else if (*szSrc < 0x800)	// 110xxxxx 10xxxxxx
				{
					*strDest++ = ((uint8)(MASK2BYTES | *szSrc >> 6));
					*strDest++ = ((uint8)((MASKBYTE | *szSrc) & MASKBITS));
					nDestLen -= 2;
					nLen += 2;
				}
				else	// 1110xxxx 10xxxxxx 10xxxxxx
				{
					*strDest++ = ((uint8)(MASK3BYTES | *szSrc >> 12));
					*strDest++ = ((uint8)((MASKBYTE | *szSrc >> 6) & MASKBITS));
					*strDest++ = ((uint8)((MASKBYTE | *szSrc) & MASKBITS));
					nDestLen -= 3;
					nLen += 3;
				}

				szSrc++;
				nSrcLen--;
			}

			*strDest = 0; // 加上0结束符
			return nLen;
		}

		inline int32 strlen_utf8(const char8* szSrc)
		{
			int32 nLen = 0;
			while (*szSrc)
			{
				if (!(*szSrc & MASKBYTE))	// 0xxxxxxx
				{
					szSrc++;
				}
				else if ((*szSrc & MASK3BYTES) == MASK3BYTES)	// 1110xxxx 10xxxxxx 10xxxxxx
				{
					szSrc += 3;
				}
				else if ((*szSrc & MASK2BYTES) == MASK2BYTES)	// 110xxxxx 10xxxxxx
				{
					szSrc += 2;
				}

				nLen++;
			}
			return nLen;
		}

		inline uint32 xor32(const char8* data)
		{
			uint32 value = 0;
			for (int32 i = 0; data[i] != 0; i++)
			{
				value ^= ((data[i]) << ((i % 4) * 8));
			}
			return value;
		}

		inline uint16 xor16(const char8* data)
		{
			uint16 value = 0;
			for (int32 i = 0; data[i] != 0; i++)
			{
				value ^= ((data[i]) << ((i % 2) * 8));
			}
			return value;
		}

		inline uint8 xor8(const char8* data)
		{
			uint8 value = 0;
			for (int32 i = 0; data[i] != 0; i++)
			{
				value ^= data[i];
			}
			return value;
		}
	}
	
#define MAKE_SHARED_USE_DL 0

#if MAKE_SHARED_USE_DL
	template<class _Ty>
	class my_allocator
	{
	public:
		typedef _Ty value_type;
		typedef value_type* pointer;
		typedef size_t size_type;

		my_allocator() _THROW0(){}
		my_allocator(const my_allocator<_Ty>&) _THROW0(){}
		template<class _Other>
		my_allocator(const my_allocator<_Other>&) _THROW0(){}

		template<class _Other>
		struct rebind{ typedef my_allocator<_Other> other; };

		template<class _Other>
		my_allocator<_Ty>& operator=(const my_allocator<_Other>&){ return (*this); }

		void deallocate(pointer _Ptr, size_type)
		{
			dlfree(_Ptr);
		}

		pointer allocate(size_type _Count)
		{
			return (pointer)dlmalloc(_Count * sizeof(value_type));
		}

		template<class _Uty>
		void destroy(_Uty* _Ptr)
		{
			_Ptr->~_Uty();
		}
	};
#endif

	template <typename T, typename... Args>
	inline shared_ptr<T> MakeSharedPtr(Args const&... args)
	{
#if MAKE_SHARED_USE_DL
		void* ptr = dlmalloc(sizeof(T));

		struct my_deleter
		{
			void operator()(T* x) const
			{
				x->~T();
				dlfree(x);
			}
		};

		return shared_ptr<T>(new(ptr)T(args...), my_deleter(), my_allocator<T>());
#else
		return make_shared<T>(args...);
#endif
	}
}
