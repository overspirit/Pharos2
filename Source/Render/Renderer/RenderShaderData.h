#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderShaderData
		{
		public:
			virtual ~RenderShaderData(){}

		public:
			virtual void CopyData(MemoryBuffer* data) = 0;
			virtual void CopyData(const void* data, uint32 len) = 0;
        };
	}
}