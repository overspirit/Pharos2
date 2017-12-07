﻿#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"

#define GRID_SIZE 10
#define SMALL_GRID_SIZE 1
#define SMALL_GRID_SLICE 5
#define GRID_BOARD_COLOR 0xFF5B5B5B
#define GRID_COLOR 0xFF6E6E6E

GridNode::GridNode()
{
	m_gridModel = nullptr;
}

GridNode::~GridNode()
{
	SAFE_DELETE(m_gridModel);
}

bool GridNode::BuildGridData()
{
	vector<VertLayoutDesc> vertDesc;
	vertDesc.push_back({ VertElementType::VET_FLOAT32, 3, "POSITION", 0, 0 });
	vertDesc.push_back({ VertElementType::VET_UNORM8, 4, "COLOR", 0, 12 });

	uint32 gridNum = GRID_SIZE * SMALL_GRID_SLICE + 1;
	MemoryBuffer vertexData;
	vertexData.Alloc(gridNum * 4 * sizeof(Vertex));

	float32 halfGridSize = GRID_SIZE / 2.0f;
	float32 smallGridSize = (float32)SMALL_GRID_SIZE / SMALL_GRID_SLICE;

	for (uint32 i = 0; i < gridNum; i++)
	{
		Color4 color = ((i % SMALL_GRID_SLICE) == 0) ? GRID_BOARD_COLOR : GRID_COLOR;

		Vertex vt1 = { Vector3Df(-halfGridSize, 0, halfGridSize - i * smallGridSize), color };
		Vertex vt2 = { Vector3Df(halfGridSize, 0, halfGridSize - i * smallGridSize), color };
		Vertex vt3 = { Vector3Df(halfGridSize - i * smallGridSize, 0, -halfGridSize), color };
		Vertex vt4 = { Vector3Df(halfGridSize - i * smallGridSize, 0, halfGridSize), color };
		
		vertexData.Insert((i * 4 + 0) * sizeof(Vertex), &vt1, sizeof(Vertex));
		vertexData.Insert((i * 4 + 1) * sizeof(Vertex), &vt2, sizeof(Vertex));
		vertexData.Insert((i * 4 + 2) * sizeof(Vertex), &vt3, sizeof(Vertex));
		vertexData.Insert((i * 4 + 3) * sizeof(Vertex), &vt4, sizeof(Vertex));
	}

// 	MeshData gridMeshData;
// 	gridMeshData.vertDesc = vertDesc;
// 	gridMeshData.vertexData = vertexData;
// 	gridMeshData.drawType = Render::EDT_LINELIST;
// 		
// 	ModelData modelData;
// 	modelData.meshDataList.push_back(gridMeshData);
// 	m_gridModel = MakeSharedPtr<Model>();
// 	m_gridModel->InitData(modelData);

	//m_renderHelper = sSceneMgr->GenerateRenderHelper("Sprite3DColor");

	//m_gridModel->BuildResource(m_renderHelper);

	return true;
}

void GridNode::Update(float32 fElapsed)
{
	OctreeScene* currScene = sSceneMgr->GetCurrScene();
	SceneCamera* camera = currScene->GetSceneCamera();
	const Matrix4& view = camera->GetViewMatrix();
	const Matrix4& proj = camera->GetProjMatrix();
	//m_renderHelper->SetViewProj(view, proj);	
	
	//m_gridModel->ProcessRenderHelper(m_renderHelper);

	//sSceneMgr->DispatchRenderHelper(m_renderHelper);
}