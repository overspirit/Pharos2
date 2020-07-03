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
    for(auto uniformBuf : m_uniformBufs)
    {
        SAFE_DELETE(uniformBuf);
    }	
}

bool RenderObject::Init()
{
    RenderBuffer* uniformBuf = sRenderer->GenerateRenderBuffer(UNIFORM_BUFFFER);
    uniformBuf->Allocate(sizeof(SceneMat));

    m_uniformBufs.push_back(uniformBuf);

    return true;
}

bool RenderObject::SetParameterValue(const char8* valueName, const Matrix4& value)
{
    if(strcmp(valueName, "g_view") == 0)
    {
        m_sceneMat.view = value;
    }
    else if(strcmp(valueName, "g_proj") == 0)
    {
        m_sceneMat.proj = value;
    }

    return true;
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

void RenderObject::SetBlockIndexBuffer(uint32 blockIndex, RenderBuffer* indexBuffer)
{
    RenderBlock& block = m_blockList[blockIndex];
    block.indexBuf = indexBuffer;
}

void RenderObject::SetBlockTexture(uint32 blockIndex, uint32 slot, RenderTexture* tex)
{
   RenderBlock& block = m_blockList[blockIndex];

   if (slot >= block.texList.size())
   {
       block.texList.resize(slot + 1);
   }

   block.texList[slot] = tex;
}

void RenderObject::Apply()
{
    if (m_blockList.size() >= m_resSetList.size())
    {
        m_resSetList.resize(m_blockList.size());
        for (uint32 i = 0; i < m_resSetList.size(); i++)
        {
            if(m_resSetList[i] == nullptr)
            {
                m_resSetList[i] = sRenderer->GenerateRenderResuourceSet();
            }

            RenderResourceSet* resSet = m_resSetList[i];

            resSet->SetVertexUniformBuffer(0, m_uniformBufs[0]);
            resSet->SetFragmentTexture(1, m_blockList[i].texList[1]);

            resSet->ApplyResourceSet();
        }
    }
}

void RenderObject::Draw()
{
    m_uniformBufs[0]->CopyData(&m_sceneMat, sizeof(SceneMat));
    
    for (int i = 0; i < m_blockList.size(); i++)
    {
        auto& block = m_blockList[i];

        block.pipeline->SetDrawType(block.drawType);

        m_renderCmd->SetPipeline(m_resSetList[i], block.pipeline);

        m_renderCmd->SetVertexBuffer(block.vertBuf);

	    m_renderCmd->SetIndexBuffer(block.indexBuf, IET_UINT32);

        if (block.indexBuf != nullptr)
        {
            uint32 indexBufSize = block.indexBuf->GetBufferSize();            
            
            m_renderCmd->DrawIndexedPrimitives(EDT_TRIANGLELIST, 0, indexBufSize / sizeof(uint32));
        }
        else
        {
            block.vertBuf->GetBufferSize();

//#error("test")
            m_renderCmd->DrawPrimitives(EDT_TRIANGLELIST, 0, 2004);
        }        
    }

   m_blockList.clear();
}
