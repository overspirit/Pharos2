#include "PreCompile.h"
#include "Pharos.h"

RenderObject::RenderObject(RenderCommand* renderCmd)
{
	m_renderCmd = renderCmd;
    
    // m_vertBuf = nullptr;
    // m_indexBuf = nullptr;
    // m_pipeline = nullptr;
}

RenderObject::~RenderObject()
{

}

uint32 RenderObject::AddRenderBlock(RenderBuffer* vertBuf, RenderPipeline* pipeline)
{
    uint32 blockIndex = m_blockList.size();
    m_blockList.resize(blockIndex + 1);

    RenderBlock& block = m_blockList[blockIndex];
    block.vertBuf = vertBuf;
    block.pipeline = pipeline;

    return blockIndex;
}

void RenderObject::SetBlockDrawInfo(uint32 blockIndex, DrawType drawType, uint32 itemCount, uint32 itemOffset)
{
    m_blockList.resize(blockIndex + 1);

    RenderBlock& block = m_blockList[blockIndex];
    block.drawType = drawType;
    block.itemCount = itemCount;
    block.itemOffset = itemOffset;
}

void RenderObject::SetBlockIndexBuffer(uint32 blockIndex, RenderBuffer* indexBuffer)
{
    m_blockList.resize(blockIndex + 1);

    RenderBlock& block = m_blockList[blockIndex];
    block.indexBuf = indexBuffer;
}

void RenderObject::SetBlockResourceSet(uint32 blockIndex, RenderResourceSet* resSet)
{
    m_blockList.resize(blockIndex + 1);

   RenderBlock& block = m_blockList[blockIndex];
   block.resSet = resSet;
}

void RenderObject::Draw()
{
    for (RenderBlock& renderBlock : m_blockList)
    {
        renderBlock.pipeline->SetDrawType(renderBlock.drawType);

        m_renderCmd->SetRenderStaging(renderBlock.resSet, renderBlock.pipeline);

        m_renderCmd->SetVertexBuffer(renderBlock.vertBuf);	    

        if (renderBlock.indexBuf != nullptr)
        {            
            m_renderCmd->SetIndexBuffer(renderBlock.indexBuf, IET_UINT32);
            
            m_renderCmd->DrawIndexedPrimitives(renderBlock.itemOffset, renderBlock.itemCount);
        }
        else
        {
            m_renderCmd->DrawPrimitives(renderBlock.itemOffset, renderBlock.itemCount);
        }        
    }

   m_blockList.clear();
}
