#include "PreCompile.h"
#include "Pharos.h"

RenderMgr::RenderMgr()
{
    m_fps = 0;
    m_renderCount = 0;

    m_renderer = nullptr;

    m_defaultTarget = nullptr;
    m_defaultCommand = nullptr;

    m_finalResourceSet = nullptr;
    m_finalProgram = nullptr;
    m_finalPipeline = nullptr;
    m_finalTarget = nullptr;
    m_finalColorTexture = nullptr;
    m_finalDepthTexture = nullptr;

    m_quadVertBuf = nullptr;

	m_renderCallback = nullptr;

    //m_blockCount = 0;
}

RenderMgr::~RenderMgr()
{

}

bool RenderMgr::Init()
{
	m_renderer = MakeRenderer();
	if (!m_renderer->Init()) return false;
	
	//quad vertex buffer
	InitQuadBuffer();

	m_defaultTarget = m_renderer->GetDefaultRenderTarget();	
	m_defaultCommand = m_renderer->GenerateRenderCommand();
	m_defaultCommand->SetDebugLabel("default render");

	//Load Effect file
	sMaterialMgr->LoadEffectFile("Shader/Sprite3D.fxml");
	sMaterialMgr->LoadEffectFile("Shader/Skeletal.fxml");
    sMaterialMgr->LoadEffectFile("Shader/BorderShine.fxml");
	
	VertLayoutDesc desc[] =
	{
		{ VET_FLOAT32, 3, VAL_POSITION, 0, 0 },
		{ VET_FLOAT32, 2, VAL_TEXCOORD0, 12, 0 },
	};
	
	m_finalPipeline = m_renderer->GenerateRenderPipeline();
	m_finalPipeline->SetInputLayoutDesc(desc, 2);

	m_finalProgram = m_renderer->GenerateRenderProgram();
	m_finalProgram->SetLibraryWithPath("Shader/Copy.lib");
	m_finalProgram->CompileVertexFunctionWithName("CopyVS");
	m_finalProgram->CompileFragmentFunctionWithName("CopyPS");	
	m_finalPipeline->SetProgramShader(m_finalProgram);
		
	int32 targetWidth = m_defaultTarget->GetWidth();
	int32 targetHeight = m_defaultTarget->GetHeight();
	
	m_finalColorTexture = m_renderer->CreateTargetTexture(targetWidth, targetHeight, EPF_RGBA8_UNORM);
	m_finalDepthTexture = m_renderer->CreateTargetTexture(targetWidth, targetHeight, EPF_D32_FLOAT_S8_UINT);

	m_finalResourceSet = m_renderer->GenerateRenderResuourceSet();
	m_finalResourceSet->SetFragmentTexture(0, m_finalColorTexture);

	m_finalTarget = m_renderer->CreateRenderTarget(targetWidth, targetHeight);
	m_finalTarget->SetColorAttach(0, m_finalColorTexture);
	m_finalTarget->SetDepthStencilAttach(m_finalDepthTexture);
	
	m_timer.Reset();

	return true;
}

void RenderMgr::Destroy()
{
    //SAFE_DELETE(m_defaultTarget);
    SAFE_DELETE(m_defaultCommand);

    SAFE_DELETE(m_finalResourceSet);
    SAFE_DELETE(m_finalProgram);
    SAFE_DELETE(m_finalPipeline);
    SAFE_DELETE(m_finalTarget);
    SAFE_DELETE(m_finalColorTexture);
    SAFE_DELETE(m_finalDepthTexture);

    SAFE_DELETE(m_quadVertBuf);

    sMaterialMgr->Destroy();

    m_renderer->Destroy();
    SAFE_DELETE(m_renderer);
}

bool RenderMgr::StartUp(const RenderParam& param)
{
	DeviceConfig cfg;
	cfg.width = param.width;
	cfg.height = param.height;
	cfg.backColor = param.backColor;
	cfg.sampleType = param.sampleType;
	cfg.sync = param.sync;
	cfg.fullScreen = param.fullScreen;
	//cfg.bufferFormat;
	//cfg.depthFormat;
	//cfg.refreshRate;    
	if (!m_renderer->Create(cfg)) return false;

	m_renderParam = param;
	//m_clearColor = param.backColor;
    
    m_finalTarget->SetClear(cfg.backColor);
	
	return true;
}

void RenderMgr::InitQuadBuffer()
{
	float scale = 1.0f;

	DecalVertex vertData[] =
	{
		{ Vector3Df(-scale, scale, 0), Vector2Df(0, 0) },
		{ Vector3Df(scale, scale, 0), Vector2Df(1.0f, 0) },
		{ Vector3Df(scale, -scale, 0), Vector2Df(1.0f, 1.0f) },
		
		{ Vector3Df(scale, -scale, 0), Vector2Df(1.0f, 1.0f) },
		{ Vector3Df(-scale, -scale, 0), Vector2Df(0, 1.0f) },
		{ Vector3Df(-scale, scale, 0), Vector2Df(0, 0) },
	};
	
	MemoryBuffer vertDataBuf;
	vertDataBuf.CopyFrom(vertData, sizeof(vertData));
	m_quadVertBuf = m_renderer->GenerateRenderBuffer(VERTEX_BUFFER);
	m_quadVertBuf->Allocate(sizeof(vertData), &vertDataBuf);
}

void RenderMgr::DoRender(RenderObject* obj)
{
	if (obj == nullptr) return;
	
	m_renderObjList.push_back(obj);
}

RenderObject* RenderMgr::GenerateRenderObject()
{
	return new RenderObject(m_defaultCommand);
}

void RenderMgr::RegisterRenderCallback(IRenderCallback* callback)
{
	m_renderCallback = callback;
}

Vector2Df RenderMgr::GetPosFromWindowPos(int32 x, int32 y)
{
	Vector2Df pos;
	const Size2Di& wndSize = sKernel->GetWindowSize();
	pos.x = (float32)x / wndSize.width * 2.0f - 1.0f;
	pos.y = 1.0f - (float32)y / wndSize.height * 2.0f;

	//还要进行视口变换
	//...

	return pos;
}

Vector2Df RenderMgr::GetSizeFromWindowSize(int32 width, int32 height)
{
	Vector2Df size;
	const Size2Di& wndSize = sKernel->GetWindowSize();
	size.x = (float32)width / wndSize.width * 2.0f;
	size.y = (float32)height / wndSize.height * 2.0f;

	//还要进行视口变换
	//...

	return size;
}

void RenderMgr::DrawFullScreenQuad(RenderTexture* tex)
{
	
}

void RenderMgr::Render(float32 fElapsed)
{	
	m_defaultCommand->BeginCommand();

	int32 width = m_finalTarget->GetWidth();
    int32 height = m_finalTarget->GetHeight();

	Rect2Di viewRect;
	viewRect.left = 0;
	viewRect.top = 0;
	viewRect.right = width;
	viewRect.bottom = height;
	m_defaultCommand->SetViewport(viewRect, 0, 1.0f);

	Rect2Di scissorRect;
	scissorRect.left = 0;
	scissorRect.top = 0;
	scissorRect.right = width;
	scissorRect.bottom = height;
	m_defaultCommand->SetScissorRect(scissorRect);

//	m_defaultCommand->BeginRenderTarget(m_finalTarget);
//
//	for(RenderObject* obj : m_renderObjList)
//	{
//		obj->Draw();
//	}
//
//	m_defaultCommand->EndRenderTarget();
    
    
    
    if (m_defaultCommand->BeginRenderTarget(m_defaultTarget))
	{
        width = m_defaultTarget->GetWidth();
        height = m_defaultTarget->GetHeight();

        viewRect.right = width;
        viewRect.bottom = height;
        m_defaultCommand->SetViewport(viewRect, 0, 1.0f);

        scissorRect.right = width;
        scissorRect.bottom = height;
        m_defaultCommand->SetScissorRect(scissorRect);
        
		//m_defaultCommand->SetVertexBuffer(m_quadVertBuf);
		//m_defaultCommand->SetRenderStaging(m_finalResourceSet, m_finalPipeline);

        for(RenderObject* obj : m_renderObjList)
        {
            obj->Draw();
        }

		//m_defaultCommand->DrawPrimitives(0, 6);

		m_defaultCommand->EndRenderTarget();
		m_defaultCommand->EndCommand();

		m_renderer->Commit();
	}

	m_renderObjList.clear();

	m_renderCount++;
	if (m_timer.GetTime() >= 1.0f)
	{
		m_timer.Reset();
		m_fps = m_renderCount;
		m_renderCount = 0;

		//char8 buf[32];
		//sprintf(buf, "FPS:%d\n", m_fps);
		//OutputDebugStringA(buf);

		printf("FPS:%d\n", m_fps);

		//LOGV("FPS:%d\n", m_fps);
	}
}

void RenderMgr::SaveRenderTarget(const char8* path)
{
	m_finalColorTexture->Save(path);
}
