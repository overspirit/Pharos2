#pragma once

namespace Pharos
{
	namespace Render
	{
		enum EBufferType
		{
			EB_Static,
			EB_Dynamic,
		};

		enum VertElementType
		{
			VET_FLOAT32,
			VET_FLOAT16,

			VET_SINT32,
			VET_SINT16,
			VET_SINT8,

			VET_UINT32,
			VET_UINT16,
			VET_UINT8,

			VET_SNORM16,
			VET_SNORM8,

			VET_UNORM16,
			VET_UNORM8,
		};

		enum IndexElementType
		{
			IET_UINT32,
			IET_UINT16,
		};

		struct VertLayoutDesc
		{
			VertElementType		elementType;
			uint32				elementNum;
			string				elementSemantic;
			uint32				semanticIndex;
			uint32				elementOffset;
		};

		//???
		//添加不同对象对同一个vertex buffer的引用，
		//clone之后,含有不同的offset,location等
		class D3D11RenderLayout //: public IRenderLayout
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
			EBufferType				m_indexBufType;
			uint32					m_indexNum;			

			D3D11_BUFFER_DESC		m_vertBufDesc;
			EBufferType				m_vertBufType;
			uint32					m_vertStride;
			uint32					m_vertBufSize;

			vector<VertLayoutDesc>	m_layoutDesc;

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

			virtual void ApplyToDevice();

			virtual uint32 GetVertStride() const { return m_vertStride; }
			virtual bool IsVertBufferDynamic() const { return (m_vertBufType == EB_Dynamic); }
			virtual uint32 GetVertBufferSize() const { return m_vertBufSize; }
			virtual uint32 GetVertNum() const { return m_vertBufSize / m_vertStride; }

			virtual uint32 GetIndexNum() const { return m_indexNum; }
			virtual bool IsIndexBufferDynamic() const { return (m_indexBufType == EB_Dynamic); }

			virtual bool IsUseIndexBuffer(){ return (m_vertBuf != nullptr) && (m_indexBuf != nullptr); }
		};
	}

	typedef shared_ptr<Render::D3D11RenderLayout> D3D11RenderLayoutPtr;
}