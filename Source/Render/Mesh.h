#pragma once

namespace Pharos
{
	namespace Render
	{
		class Mesh
		{
		public:
			Mesh();
			virtual ~Mesh();

		private:
			string		m_meshName;

			vector<VertLayoutDesc>		m_vertDesc;
			RenderBuffer*           	m_vertBuf;
			RenderBuffer*				m_indexBuf;

			DrawType				m_drawType;
			uint32					m_vertNum;
			uint32					m_faceNum;

		public:
			virtual void SetMeshName(const char8* name) { m_meshName = name; }

			virtual void SetMeshVertexData(MemoryBuffer* vertexData, uint32 vertNum, const vector<VertLayoutDesc>& vertDesc);
			virtual RenderBuffer* GetMeshVertexBuffer() { return m_vertBuf; }
			virtual vector<VertLayoutDesc> GetMeshVertexDesc() { return m_vertDesc; }

			virtual void SetMeshIndexData(MemoryBuffer* indexData, uint32 faceNum);
			virtual RenderBuffer* GetMeshIndexBuffer() { return m_indexBuf; }

			virtual void SetDrawType(DrawType drawType) { m_drawType = drawType; }
			virtual DrawType GetDrawType() { return m_drawType; }
			virtual uint32 GetVertNum() { return m_vertNum; }
			virtual uint32 GetFaceNum() { return m_faceNum; }
		};
	}
}
