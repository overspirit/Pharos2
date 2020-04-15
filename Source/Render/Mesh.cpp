#include "PreCompile.h"
#include "Pharos.h"

Mesh::Mesh()
{
	m_material = nullptr;

	m_renderLayout = nullptr;

	m_drawType = EDT_TRIANGLELIST;
}

Mesh::~Mesh()
{
	SAFE_DELETE(m_renderLayout);

	SAFE_DELETE(m_material);
}

void Mesh::SetMeshData(MemoryBuffer* vertexData, const vector<VertLayoutDesc>& vertDesc, MemoryBuffer* indexData)
{
	Renderer* renderer = sRenderMgr->GetCurrentRenderer();

	uint32 vertSize = vertexData->GetLength();
	m_renderLayout = renderer->GenerateRenderLayout(vertSize, vertexData);

	m_renderLayout->SetInputLayoutDesc(&*vertDesc.begin(), (uint32)vertDesc.size());

	if (indexData != nullptr)
	{
		uint32 indexSize = indexData->GetLength();
		m_renderLayout->CreateIndexBuffer(indexSize / sizeof(uint32), indexData);
	}
}

void Mesh::SetAttachMaterial(Material* material)
{
	m_material = material;
}
