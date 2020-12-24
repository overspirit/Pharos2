#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalResourceSet : public RenderResourceSet
		{
		public:
			MetalResourceSet(id<MTLDevice> device);
			virtual ~MetalResourceSet();

		private:
			id<MTLDevice>		m_device;
			
			vector<MetalRenderBuffer*>		m_vertUniformBufList;
			vector<MetalRenderBuffer*>		m_fragUniformBufList;
			vector<MetalRenderTexture*>		m_vertTexList;
			vector<MetalRenderTexture*>		m_fragTexList;
			
		public:
			virtual bool SetVertexUniformBuffer(uint32 slot, RenderBuffer* buffer);
			virtual bool SetFragmentUniformBuffer(uint32 slot, RenderBuffer* buffer);

			virtual bool SetVertexTexture(uint32 slot, RenderTexture* tex);
			virtual bool SetFragmentTexture(uint32 slot, RenderTexture* tex);
						
			virtual void ApplyResourceSet(id <MTLRenderCommandEncoder> renderEncoder);
		};
	}
}
