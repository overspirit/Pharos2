#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"


RenderBlock::RenderBlock()
{
	m_renderer = nullptr;

	m_blockData = nullptr;
	m_blockDataBuf = nullptr;

	m_tech = nullptr;
	m_layout = nullptr;

	m_drawType = EDT_TRIANGLELIST;

	m_startIndex = 0;
	m_countNum = 0;
}

RenderBlock::~RenderBlock()
{
	SAFE_DELETE(m_blockData);
	//SAFE_DELETE(m_blockDataBuf);
}

void RenderBlock::Init()
{
	m_renderer = sRenderMgr->GetCurrentRenderer();

	m_blockData = m_renderer->CreateShaderData();
	m_blockData->SetDataSize(sizeof(Matrix4));// sizeof(BlockData));
	m_blockDataBuf = (BlockData*)m_blockData->GetDataBufferPointer();
}

void RenderBlock::SetBlockDataWorldMatrix(const Matrix4& world)
{
	m_blockDataBuf->world = world;
}

void RenderBlock::SetBlockDataBoneMatrix(const Matrix4* bones, uint32 boneNum)
{
	uint32 copyNum = Math::minimum(255u, boneNum);

	//memcpy(m_blockDataBuffer->bone, bones, copyNum * sizeof(Matrix4));
}

void RenderBlock::ApplyToDevice()
{
	//m_renderer = sRenderMgr->GetCurrentRenderer();
	if (m_renderer == nullptr)
	{
		assert(false);
		return;
	}

	if (m_layout == nullptr || m_tech == nullptr)
	{
		assert(false);
		return;
	}

	if (m_countNum == 0)
	{
		m_startIndex = 0;

		if (m_layout->IsUseIndexBuffer())
		{
			m_countNum = m_layout->GetIndexNum();
		}
		else
		{
			m_countNum = m_layout->GetVertNum();
		}
	}

	if (m_blockData != nullptr)
	{
		m_renderer->BindShaderData(1, m_blockData);
	}

	m_tech->ApplyToDevice();

	m_renderer->BindLayout(m_layout);

	for (uint32 i = 0; i < m_tech->GetPassNum(); i++)
	{
		RenderPass* pass = m_tech->GetPass(i);
		if (pass == nullptr)
			break;

		pass->ApplyToDevice();

		m_renderer->DrawImmediate(m_drawType, m_startIndex, m_countNum);
	}
}

/*
RenderBlock::RenderBlock()
{
	m_renderer = nullptr;

	m_blockData = nullptr;
	m_blockDataBuffer = nullptr;
}

RenderBlock::~RenderBlock()
{

}

void RenderBlock::Init()
{
	m_renderer = sRenderMgr->GetCurrentRenderer();

	//m_blockData = m_renderer->CreateShaderData();
	//m_blockData->SetDataSize(sizeof(Matrix4));// sizeof(BlockData));
	//m_blockDataBuffer = (BlockData*)m_blockData->GetDataBufferPointer();
}



void RenderBlock::SetBlockPatchSize(uint32 size)
{
	m_blockPatchList.resize(size);
}

void RenderBlock::SetRenderBlockPatch(uint32 patchIndex, RenderLayout* layout, RenderTechnique* tech)
{
	if (patchIndex >= m_blockPatchList.size()) return;

	uint32 countNum = 0;

	if (layout->IsUseIndexBuffer())
	{
		countNum = layout->GetIndexNum();
	}
	else
	{
		countNum = layout->GetVertNum();
	}

	m_blockPatchList[patchIndex].layout = layout;
	m_blockPatchList[patchIndex].tech = tech;
	m_blockPatchList[patchIndex].drawType = EDT_TRIANGLELIST;
	m_blockPatchList[patchIndex].startIndex = 0;
	m_blockPatchList[patchIndex].countNum = countNum;
}

void RenderBlock::SetPatchDrawRange(uint32 patchIndex, DrawType type, uint32 start, uint32 count)
{
	if (patchIndex >= m_blockPatchList.size()) return;

	BlockPatch& blockPatch = m_blockPatchList[patchIndex];

	blockPatch.drawType = type;
	blockPatch.startIndex = start;
	blockPatch.countNum = count;
}

void RenderBlock::ApplyDevice()
{
	//if (m_blockData != nullptr)
	//{
		//m_renderer->BindShaderData(1, m_blockData);
	//}

	for (uint32 i = 0; i < m_blockPatchList.size(); i++)
	{
		BlockPatch& blockPatch = m_blockPatchList[i];

		m_renderer->BindLayout(blockPatch.layout);

		//blockPatch.tech->ApplyDevice();

		for (uint32 j = 0; j < blockPatch.tech->GetPassNum(); j++)
		{
			RenderPass* renderPass = blockPatch.tech->GetPass(j);
			if (renderPass == nullptr)
				break;

			//renderPass->ApplyDevice();

			//m_renderer->DrawImmediate(blockPatch.drawType, blockPatch.startIndex, blockPatch.countNum);
		}
	}
}

*/