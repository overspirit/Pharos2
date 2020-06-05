#pragma once

namespace Pharos
{
	namespace Render
	{
		class IMetalCommandFactory
		{
		public:
			virtual ~IMetalCommandFactory(void){}
		
		public:
			virtual id<MTLCommandBuffer> MakeMetalCommandBuffer() = 0;
		};
	}
}
