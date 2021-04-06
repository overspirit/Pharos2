#include "PreCompile.h"
#include "Pharos.h"

Mesh::Mesh(const char8* name)
{
    m_meshName = name;
    
	m_vertBuf = nullptr;
	m_indexBuf = nullptr;

	m_drawType = EDT_TRIANGLELIST;
	m_vertNum = 0;
	m_faceNum = 0;
}

Mesh::~Mesh()
{
	SAFE_DELETE(m_vertBuf);
	SAFE_DELETE(m_indexBuf);
}

void Mesh::SetMeshVertexDesc(const vector<VertLayoutDesc>& vertDesc)
{
	m_vertDesc = vertDesc;
}

void Mesh::SetMeshVertexData(MemoryBuffer* vertexData, uint32 vertNum)
{
	if (m_vertBuf == NULL)
	{
		m_vertBuf = sRenderer->GenerateRenderBuffer(VERTEX_BUFFER);
		m_vertBuf->Allocate(vertexData->GetLength(), vertexData);
	}
	else
	{
		m_vertBuf->CopyData(*vertexData, 0);
	}

	m_vertNum = vertNum;
}

void Mesh::SetMeshIndexData(MemoryBuffer* indexData, uint32 faceNum)
{
	if (m_indexBuf == NULL)
	{
		m_indexBuf = sRenderer->GenerateRenderBuffer(INDICES_BUFFER);
		m_indexBuf->Allocate(indexData->GetLength(), indexData);
	}
	else
	{
		m_indexBuf->CopyData(*indexData, 0);
	}

	m_faceNum = faceNum;
}
