#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalRenderLayout : public RenderLayout
		{
		public:
			MetalRenderLayout();
			virtual ~MetalRenderLayout();

		private:

		public:
			virtual bool CreateVertexBuffer(uint32 sizeInBytes, MemoryBuffer* data = nullptr);
			virtual bool CreateIndexBuffer(uint32 indexNum, MemoryBuffer* data = nullptr);
			virtual bool SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum);

			virtual void CopyVertexBuffer(MemoryBuffer* data, uint32 dataSize, uint32 nOffsetInBytes = 0, bool bNoOverwrite = true);
			virtual void CopyIndexBuffer(MemoryBuffer* data, uint32 dataSize, uint32 nOffsetInBytes = 0, bool bNoOverwrite = true);

			virtual void ApplyDevice();

			virtual uint32 GetVertStride() const { return m_vertStride; }
			virtual bool IsVertBufferDynamic() const { return (m_vertBufType == EB_Dynamic); }
			virtual uint32 GetVertBufferSize() const { return m_vertBufSize; }
			virtual uint32 GetVertNum() const { return m_vertBufSize / m_vertStride; }

			virtual uint32 GetIndexNum() const { return m_indexNum; }
			virtual bool IsIndexBufferDynamic() const { return (m_indexBufType == EB_Dynamic); }

			virtual bool IsUseIndexBuffer(){ return false; }
		};
	}
}
