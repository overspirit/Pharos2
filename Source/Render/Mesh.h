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
			Material*				m_material;

			RenderLayout*			m_renderLayout;	

			DrawType				m_drawType;

		public:
			//virtual bool InitData(const MeshData& meshData);
			
			virtual void SetMeshData(MemoryBuffer* vertexData, const vector<VertLayoutDesc>& vertDesc, MemoryBuffer* indexData = nullptr);

			virtual DrawType GetDrawType() { return m_drawType; }
			virtual void SetDrawType(DrawType type) { m_drawType = type; }
			virtual RenderLayout* GetRenderLayout() { return m_renderLayout; }
			virtual void SetAttachMaterial(Material* material);
			virtual Material* GetAttachMaterial() { return m_material; }

		};
	}
}