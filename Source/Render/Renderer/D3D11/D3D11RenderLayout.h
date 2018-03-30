#pragma once

namespace Pharos
{
	namespace Render
	{
		//???
		//添加不同对象对同一个vertex buffer的引用，
		//clone之后,含有不同的offset,location等
		class D3D11RenderLayout : public RenderLayout
		{
		public:
			D3D11RenderLayout(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
			virtual ~D3D11RenderLayout();

		private:
			ID3D11Device*			m_pDevice;
			ID3D11DeviceContext*	m_pContext;

			ID3D11Buffer*			m_indexBuf;
			ID3D11Buffer*			m_vertBuf;
			ID3D11InputLayout*		m_layout;

			D3D11_BUFFER_DESC		m_indexBufDesc;

			D3D11_BUFFER_DESC		m_vertBufDesc;

		private:
			DXGI_FORMAT GetD3D11Format(VertElementType elementType, uint32 elementNum);
			uint32 GetVertElementSize(VertElementType elementType, uint32 elementNum);

		public:
			bool IsInputLayoutValid(){ return m_layout != NULL; }
			bool CreateD3D11InputLayout(ID3D10Blob* vs_blob);

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

			virtual bool IsUseIndexBuffer(){ return (m_vertBuf != nullptr) && (m_indexBuf != nullptr); }
		};
	}
}