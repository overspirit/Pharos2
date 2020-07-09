#include "PreCompile.h"
#include "Pharos.h"

RenderMgr::RenderMgr()
{
	m_fps = 0;
	m_renderCount = 0;

	m_renderer = nullptr;
	
	m_defaultTarget = nullptr;
	m_defaultCommand = nullptr;
	
	m_quadVertBuf = nullptr;

	//m_blockCount = 0;

	//m_renderCallback = nullptr;
}

RenderMgr::~RenderMgr()
{

}

bool RenderMgr::Init()
{
	m_renderer = MakeRenderer();
	if (!m_renderer->Init()) return false;
	
	m_timer.Reset();

	return true;
}

void RenderMgr::Destroy()
{
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
	
	//quad vertex buffer
	InitQuadBuffer();

	//Load Effect file
	sMaterialMgr->LoadEffectFile("Shader/Sprite3D.fxml");
	sMaterialMgr->LoadEffectFile("Shader/Skeletal.fxml");	
	
	//
	///////////////////////////////////////////////////////////////////////////////////
	VertLayoutDesc desc[] =
	{
		{ VET_FLOAT32, 3, VAL_POSITION, 0, 0 },
		{ VET_FLOAT32, 2, VAL_TEXCOORD0, 12, 0 },
	};
	
	//RenderPipeline* pipeline = m_renderer->GenerateRenderPipeline();
	//pipeline->SetInputLayoutDesc(desc, 2);
	//pipeline->SetProgramShader(defaultProgram);
	///////////////////////////////////////////////////////////////////////////////////
	
	m_defaultTarget = m_renderer->GetDefaultRenderTarget();	
	m_defaultCommand = m_renderer->GenerateRenderCommand(m_defaultTarget);
	m_defaultCommand->SetDebugLabel("default render");
		
//	m_finalTarget = m_renderer->CreateRenderTarget(1280, 720);
//	m_finalTexture = m_finalTarget->GenerateColorAttach(0, EPF_RGBA8_UNORM);
//
//	m_finalCommand = m_renderer->GenerateRenderCommand(m_finalTarget);
//	m_finalCommand->SetDebugLabel("final render");	
	
	return true;
}

void RenderMgr::InitQuadBuffer()
{
	DecalVertex vertData[] =
	{
		{ Vector3Df(-1.0f,  1.0f, 0), Vector2Df(0, 0) },
		{ Vector3Df(1.0f, 1.0f, 0), Vector2Df(1.0f, 0) },
		{ Vector3Df(1.0f, -1.0f, 0), Vector2Df(1.0f, 1.0f) },
		
		{ Vector3Df(1.0f, -1.0f, 0), Vector2Df(1.0f, 1.0f) },
		{ Vector3Df(-1.0f, -1.0f, 0), Vector2Df(0, 1.0f) },
		{ Vector3Df(-1.0f, 1.0f, 0), Vector2Df(0, 0) },
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

void RenderMgr::SetDefaultClearParam(Color4 color, float32 depth, uint32 stencil)
{
//	m_clearColor = color;
//	m_clearDepth = depth;
//	m_clearStencil = stencil;
}

void RenderMgr::RegisterRenderCallback(IRenderCallback* callback)
{
	//m_renderCallback = callback;
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

void RenderMgr::Update(float32 fElapsed)
{

}

void RenderMgr::Render(float32 fElapsed)
{	
	//m_finalCommand->BeginCommand();
	
	/*
	 for(RenderObject* obj : m_renderObjList)
	 {
	 obj->Draw();
	 }
	 */
	
	//m_finalCommand->EndCommand();
	
	
	//m_finalTexture = xxx;
	
	// for(RenderObject* obj : m_renderObjList)
	// {
	// 	obj->PreDraw();
	// }

	m_defaultCommand->BeginCommand();

	int32 width = m_defaultTarget->GetWidth();
    int32 height = m_defaultTarget->GetHeight();

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

	for(RenderObject* obj : m_renderObjList)
	{
		obj->Draw();
	}
	
	//输入finalTexture
	//对每个后处理渲染
	//...
	
	
	m_defaultCommand->EndCommand();
	
	m_renderer->Commit();

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

		LOGV("FPS:%d\n", m_fps);
	}
}
