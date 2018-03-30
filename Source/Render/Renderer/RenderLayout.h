#pragma once

namespace Pharos
{
	namespace Render
	{
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

		class RenderLayout
		{
		public:
			RenderLayout()
			{
				m_indexBufType = EB_Static;
				m_indexNum = 0;

				m_vertBufType = EB_Static;
				m_vertStride = 0;
				m_vertBufSize = 0;
			}

			virtual ~RenderLayout(){}

		protected:
			EBufferType				m_indexBufType;
			uint32					m_indexNum;

			EBufferType				m_vertBufType;
			uint32					m_vertStride;
			uint32					m_vertBufSize;

			vector<VertLayoutDesc>	m_layoutDesc;

		public:
			virtual bool CreateIndexBuffer(uint32 nNumIndex, MemoryBuffer* data = nullptr) = 0;
			virtual bool SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum) = 0;			

			virtual void CopyVertexBuffer(MemoryBuffer* data, uint32 dataSize, uint32 nOffsetInBytes = 0, bool bNoOverwrite = true) = 0;
			virtual void CopyIndexBuffer(MemoryBuffer* data, uint32 dataSize, uint32 nOffsetInBytes = 0, bool bNoOverwrite = true) = 0;
			
			virtual uint32 GetVertStride() const { return m_vertStride; }
			virtual bool IsVertBufferDynamic() const { return (m_vertBufType == EB_Dynamic); }
			virtual uint32 GetVertBufferSize() const { return m_vertBufSize; }
			virtual uint32 GetVertNum() const { return m_vertBufSize / m_vertStride; }

			virtual uint32 GetIndexNum() const { return m_indexNum; }
			virtual bool IsIndexBufferDynamic() const { return (m_indexBufType == EB_Dynamic); }

			virtual bool IsUseIndexBuffer() = 0;
		};
	}
}