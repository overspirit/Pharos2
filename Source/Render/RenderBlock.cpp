#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"


RenderBlock::RenderBlock()
{
	m_renderer = nullptr;

	m_blockData = nullptr;

	m_blockPatchNum = 0;
}

RenderBlock::~RenderBlock()
{
	SAFE_DELETE(m_blockData);
}

void RenderBlock::Init()
{
	m_renderer = sRenderMgr->GetCurrentRenderer();

	m_blockData = m_renderer->CreateShaderData();
	m_blockData->SetDataSize(sizeof(Matrix4));		//先初始化一个Matrix大小
}

void RenderBlock::SetBlockDataWorldMatrix(const Matrix4& world)
{
	if (m_blockData == nullptr) return;
	m_blockData->CopyData(&world, sizeof(Matrix4));
}

void RenderBlock::SetBlockDataBoneMatrix(const Matrix4* bones, uint32 boneNum)
{
	if (m_blockData == nullptr) return;

	//强制规定骨骼矩阵在世界矩阵的后面，所以偏移一个矩阵的位置拷贝骨骼矩阵
	m_blockData->CopyData(bones, Math::minimum(255u, boneNum) * sizeof(Matrix4), sizeof(Matrix4));
}

uint32 RenderBlock::AddRenderBlockPatch(RenderLayout* layout, RenderTechnique* tech)
{
	if (layout == nullptr || tech == nullptr) return 0xFFFFFFFF;

	uint32 countNum = 0;

	if (layout->IsUseIndexBuffer())
	{
		countNum = layout->GetIndexNum();
	}
	else
	{
		countNum = layout->GetVertNum();
	}

	uint32 patchIndex = m_blockPatchNum;

	if (patchIndex >= m_blockPatchList.size())
	{
		m_blockPatchList.resize(patchIndex + 1);
	}

	BlockPatch& blockPatch = m_blockPatchList[patchIndex];
	blockPatch.layout = layout;
	blockPatch.tech = tech;
	blockPatch.drawType = EDT_TRIANGLELIST;
	blockPatch.startIndex = 0;
	blockPatch.countNum = countNum;	

	m_blockPatchNum++;

	return patchIndex;
}

void RenderBlock::SetBlockPatchDrawType(uint32 patchIndex, DrawType type)
{
	if (patchIndex >= m_blockPatchNum) return;

	m_blockPatchList[patchIndex].drawType = type;
}

void RenderBlock::SetBlockPatchDrawRange(uint32 patchIndex, uint32 start, uint32 count)
{
	if (patchIndex >= m_blockPatchNum) return;

	BlockPatch& blockPatch = m_blockPatchList[patchIndex];
	blockPatch.startIndex = start;
	blockPatch.countNum = count;
}

RenderTechnique* RenderBlock::GetBlockPatchTechnique(uint32 patchIndex)
{
	if (patchIndex >= m_blockPatchNum) return nullptr;
	return m_blockPatchList[patchIndex].tech;
}

RenderLayout* RenderBlock::GetBlockPatchLayout(uint32 patchIndex)
{
	if (patchIndex >= m_blockPatchNum) return nullptr;
	return m_blockPatchList[patchIndex].layout;
}

DrawType RenderBlock::GetBlockPatchDrawType(uint32 patchIndex)
{
	if (patchIndex >= m_blockPatchNum) return ((DrawType)0xFFFFFFFF);
	return m_blockPatchList[patchIndex].drawType;
}

uint32 RenderBlock::GetBlockPatchDrawStart(uint32 patchIndex)
{
	if (patchIndex >= m_blockPatchNum) return 0xFFFFFFFF;
	return m_blockPatchList[patchIndex].startIndex;
}

uint32 RenderBlock::GetBlockPatchDrawCount(uint32 patchIndex)
{
	if (patchIndex >= m_blockPatchNum) return 0xFFFFFFFF;
	return m_blockPatchList[patchIndex].countNum;
}

void RenderBlock::ApplyToDevice()
{
	if (m_renderer == nullptr)
	{
		assert(false);
		return;
	}

	if (m_blockData != nullptr)
	{
		m_renderer->BindShaderData(1, m_blockData);
	}

	for (uint32 i = 0; i < m_blockPatchNum; i++)
	{
		BlockPatch& blockPatch = m_blockPatchList[i];

		if (blockPatch.layout == nullptr || blockPatch.tech == nullptr)
		{
			assert(false);
			continue;
		}

		blockPatch.tech->ApplyToDevice();

		m_renderer->BindLayout(blockPatch.layout);

		for (uint32 i = 0; i < blockPatch.tech->GetPassNum(); i++)
		{
			RenderPass* pass = blockPatch.tech->GetPass(i);
			if (pass == nullptr)
				break;

			pass->ApplyToDevice();

			m_renderer->DrawImmediate(blockPatch.drawType, blockPatch.startIndex, blockPatch.countNum);
		}
	}

	m_blockPatchNum = 0;
}