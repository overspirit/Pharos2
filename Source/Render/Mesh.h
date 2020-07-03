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

		public:
			virtual void SetMeshName(const char8* name) { m_meshName = name; }

			virtual void SetMeshVertexData(MemoryBuffer* vertexData, const vector<VertLayoutDesc>& vertDesc);
			virtual RenderBuffer* GetMeshVertexBuffer() { return m_vertBuf; }
			virtual vector<VertLayoutDesc> GetMeshVertexDesc() { return m_vertDesc; }

			virtual void SetMeshIndexData(MemoryBuffer* indexData);
			virtual RenderBuffer* GetMeshIndexBuffer() { return m_indexBuf; }

			virtual void SetDrawType(DrawType drawType) { m_drawType = drawType; }
			virtual DrawType GetDrawType() { return m_drawType; }
		};
	}
}
