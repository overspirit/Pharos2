#pragma once

namespace Pharos
{
	typedef unsigned char			uint8;
	typedef signed char				int8;
	typedef unsigned short			uint16;
	typedef signed short			int16;
	typedef unsigned int			uint32;
	typedef signed int				int32;
	typedef unsigned long long		uint64;
	typedef long long				int64;

	typedef char			char8;
	//typedef wchar_t			char16;			//wchar_t 在不同平台上占用的字节数不同，所以不能定义成char16
    
    typedef float		float32;
	typedef double		float64;
}
