#include "PreCompile.h"
#include "Pharos.h"

Mesh::Mesh()
{	
	m_vertBuf = nullptr;
	m_indexBuf = nullptr;

	m_drawType = EDT_TRIANGLELIST;
}

Mesh::~Mesh()
{
	SAFE_DELETE(m_vertBuf);
	SAFE_DELETE(m_indexBuf);
}

void Mesh::SetMeshVertexData(MemoryBuffer* vertexData, const vector<VertLayoutDesc>& vertDesc)
{
	m_vertBuf = sRenderer->GenerateRenderBuffer(VERTEX_BUFFER);
	m_vertBuf->Allocate(vertexData->GetLength(), vertexData);

	m_vertDesc = vertDesc;
}

void Mesh::SetMeshIndexData(MemoryBuffer* indexData)
{
	m_indexBuf = sRenderer->GenerateRenderBuffer(INDICES_BUFFER);
	m_indexBuf->Allocate(indexData->GetLength(), indexData);
}
