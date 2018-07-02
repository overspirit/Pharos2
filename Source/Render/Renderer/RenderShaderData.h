﻿#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderShaderData
		{
		public:
			virtual ~RenderShaderData(){}

		public:
			virtual void CopyData(const MemoryBuffer& data, uint32 offset = 0) = 0;
			virtual void CopyData(const void* data, uint32 len, uint32 offset = 0) = 0;
        };
	}
}