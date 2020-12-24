#pragma once

namespace Pharos
{
	namespace Render
	{
		/*
		 //Metal渲染中，苹果建议app持有一个或多个CommandQueue，而CommandQueues生成的CommandBuffer,
		 //以及CommandBuffer 生成的 encoder 都是临时对象，在调用 endEncoder 之后 Encoder 会失效，
		 //CommandBuffer 在 Commit 之后也会失效。所以在IMetalCommandFoctory中会重新生成CommandBuffer
		 // IMetalCommandFoctory 现在使用同一个CommandBuffer 对象，该CommandBuffer对象在没完成之前，即状态
		 //未提交之前，IMetalCommandFactory 一直返回同一个CommandBuffer。而Encoder对象自己管理，在调用End之后
		 //将Encoder对象设为nil。IMetalCommandFoctory 视情况是否实现多线程或者多块CommandBuffer，每个CommandBuffer
		 //对应一组渲染块。
		 */
		
		class MetalRenderCommand : public RenderCommand
		{
		public:
			MetalRenderCommand(IMetalCommandFactory* cmdFactory);
			virtual ~MetalRenderCommand();

		private:
			IMetalCommandFactory*		m_cmdFactory;
			
            id<MTLCommandBuffer>            m_commandBuffer;
			id<MTLRenderCommandEncoder>    m_renderEncoder;			
			
            MetalRenderTarget*      m_metalTarget;
            
            //DrawIndexedPrimitives 需要IndexBuffer
			id<MTLBuffer>   	m_indexBuffer;
			MTLIndexType		m_indexBufType;
			
            //DrawType 存储在Pipeline对象中
			DrawType		m_currDrawType;
			
			string			m_debugLabel;
			
		public:
            virtual void SetDebugLabel(const char8* label) { m_debugLabel = label; }
            
			virtual void BeginCommand();
            virtual void EndCommand();
            
            virtual bool BeginRenderTarget(RenderTarget* target);
            virtual void EndRenderTarget();
            
            virtual void SetVertexBuffer(RenderBuffer* buffer);
			virtual void SetIndexBuffer(RenderBuffer* indexBuffer, IndexElementType indexType);
            
			virtual void SetViewport(const Rect2Di& viewRect, float32 minDepth, float32 maxDepth);
			virtual void SetScissorRect(const Rect2Di& scissorRect);

            virtual void SetRenderStaging(RenderResourceSet* resSet, RenderPipeline* pipeline);
            
			virtual void DrawPrimitives(uint32 start, uint32 count);
			virtual void DrawIndexedPrimitives(uint32 start, uint32 count);

			
		};
	}
}
