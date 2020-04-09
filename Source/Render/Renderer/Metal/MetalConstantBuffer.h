#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalConstantBuffer : public RenderShaderData
		{
		public:
			MetalConstantBuffer();
			virtual ~MetalConstantBuffer();

		private:

		private:
			bool CreateBuffer(uint32 bufSize);

		public:
			virtual void CopyData(const MemoryBuffer& data, uint32 offset = 0);
			virtual void CopyData(const void* data, uint32 len, uint32 offset = 0);

			virtual void ApplyDevice(uint32 slot);
		};
	}
}
