#include "PreCompile.h"
#include "Pharos.h"

RenderMgr::RenderMgr()
{
	m_fps = 0;
	m_renderCount = 0;

	m_renderer = nullptr;

	m_defaultFrameBuf = nullptr;

	m_finalFrameBuf = nullptr;
	m_finalTargetTex = nullptr;

	m_hdrPostProcess = nullptr;

	m_postProcessTech = nullptr;
	m_postProcessShader = nullptr;
	m_gammaCorrection = nullptr;

	m_quadLayout = nullptr;

	m_clearColor = 0xFF000000;// 0xFF3F3F3F;//R=43,G=147,B=223
	m_clearDepth = 1.0f;
	m_clearStencil = 0;

	m_blockCount = 0;

	m_renderCallback = nullptr;

	m_globalShaderData = nullptr;
}

RenderMgr::~RenderMgr()
{

}

bool RenderMgr::Init()
{
	m_renderer = MakeRenderer();
	if (!m_renderer->Init()) return false;

	this->LoadEffectFile("Shader/Sprite3D.fxml");
	this->LoadEffectFile("Shader/Sprite2D.fxml");
	this->LoadEffectFile("Shader/Font.fxml");
	this->LoadEffectFile("Shader/Skeletal.fxml");
	this->LoadEffectFile("Shader/Copy.fxml");
	this->LoadEffectFile("Shader/PostToneMapping.fxml");
	this->LoadEffectFile("Shader/SumLum.fxml");
	this->LoadEffectFile("Shader/ToneMapping.fxml");
	this->LoadEffectFile("Shader/AtmosphericScattering.fxml");

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
	m_quadLayout = m_renderer->GenerateRenderLayout(sizeof(vertData), &vertDataBuf);

	VertLayoutDesc desc[] =
	{
		{ VET_FLOAT32, 3, "POSITION", 0, 0 },
		{ VET_FLOAT32, 2, "TEXCOORD", 0, 12 },
	};
	m_quadLayout->SetInputLayoutDesc(desc, 2);

	m_globalShaderData = m_renderer->CreateShaderData();

	m_timer.Reset();

	return true;
}

void RenderMgr::Destroy()
{
	SAFE_DELETE(m_hdrPostProcess);

	SAFE_DELETE(m_postProcessTech);
	SAFE_DELETE(m_gammaCorrection);

	SAFE_DELETE(m_quadLayout);

	SAFE_DELETE(m_finalFrameBuf);

	SAFE_DELETE(m_globalShaderData);

	for (auto iter : m_techList)
	{
		SAFE_DELETE(iter.second);
	}

	m_renderer->Destroy();
	SAFE_DELETE(m_renderer);
}

bool RenderMgr::StartUp(const RenderParam& param)
{
	DeviceConfig cfg;
	cfg.wndHandle = sKernel->GetWindowHandle();
	cfg.width = param.width;
	cfg.height = param.height;
	cfg.sampleType = param.sampleType;
	cfg.sync = param.sync;
	cfg.fullScreen = param.fullScreen;
	//cfg.bufferFormat;
	//cfg.depthFormat;
	//cfg.refreshRate;    
	if (!m_renderer->Create(cfg)) return false;

	m_renderParam = param;
	m_clearColor = param.backColor;

	m_defaultFrameBuf = m_renderer->GetDefaultFrameBuffer();	

	uint32 finalBufferWidth = m_defaultFrameBuf->GetWidth();
	uint32 finalBufferHeight = m_defaultFrameBuf->GetHeight();
	m_finalFrameBuf = m_renderer->CreateFrameBuffer(finalBufferWidth, finalBufferHeight);
	m_finalTargetTex = m_finalFrameBuf->CreateRenderTexture(0, EPF_RGBA8_UNORM);

//    m_postProcessTech = this->GenerateRenderTechnique(m_renderParam.gammaEnabled ? "GammaCorrection" : "Copy");
//    m_postProcessShader = m_postProcessTech->GetPass(0)->GetShaderProgram();
//
//    m_gammaCorrection = new PostProcess();
//    m_gammaCorrection->InitWithTech("GammaCorrection");
//    m_gammaCorrection->SetInputPin(0, m_finalTargetTex);
	//m_gammaCorrection->SetOutputPin();

	//m_hdrPostProcess = new HDRPostProcess();
	//m_hdrPostProcess->Init();
	//m_hdrPostProcess->SetInputPin(0, m_finalTargetTex);

	return true;
}

void RenderMgr::SetGlobalRenderViewMatrix(const Matrix4& viewMatrix)
{
	if (m_globalShaderData != nullptr)
	{
		m_globalShaderData->CopyData(&viewMatrix, sizeof(viewMatrix), offsetof(GlobalData, viewMatrix));
	}
}

void RenderMgr::SetGlobalRenderProjMatrix(const Matrix4& projMatrix)
{
	if (m_globalShaderData != nullptr)
	{
		m_globalShaderData->CopyData(&projMatrix, sizeof(projMatrix), offsetof(GlobalData, projMatrix));
	}
}

void RenderMgr::SetGlobalRenderEyePostion(const Vector3Df& eyePos)
{
	if (m_globalShaderData != nullptr)
	{
		Vector4Df eyePosition = Vector4Df(eyePos.x, eyePos.y, eyePos.z, 1.0f);
		m_globalShaderData->CopyData(&eyePosition, sizeof(eyePosition), offsetof(GlobalData, eyePosition));
	}
}

void RenderMgr::DoRender(RenderBlock* block)
{
	if (block == nullptr) return;

	if (m_blockCount <= m_blockList.size())
	{
		m_blockList.resize(m_blockCount + 1);
	}	

	m_blockList[m_blockCount] = block;

	m_blockCount++;
}

bool RenderMgr::LoadEffectFile(const char8* szPath)
{
	RenderEffectLoader* effectLoader = MakeEffectLoader();
	if (!effectLoader->Load(szPath))
	{
		SAFE_DELETE(effectLoader);
		return false;
	}	

	//按照渲染模块的要求处理RenderTechInfo
	for (uint32 i = 0; i < effectLoader->GetTechniqueInfoNum(); i++)
	{
		RenderTechInfo* techInfo = effectLoader->GetTechniqueInfo(i);

		//slot为0的ShaderData渲染模块给了观察和投影矩阵，所以禁用读取的TechInfo中的Slot为0的ShaderData
		techInfo->SetShaderDataValid(0, false);

		//slot为1的ShaderData渲染模块给了世界矩阵和骨骼矩阵，所以禁用读取的TechInfo中的Slot为1的ShaderData
		techInfo->SetShaderDataValid(1, false);
	}

	for (uint32 i = 0; i < effectLoader->GetTechniqueInfoNum(); i++)
	{
		RenderTechInfo* techInfo = effectLoader->GetTechniqueInfo(i);

		RenderTechnique* renderTech = MakeRenderTechnique();
		
		if (renderTech->Create(techInfo))
		{
			string techName = renderTech->GetTechName();
			m_techList[techName] = renderTech;
		}
	}

	SAFE_DELETE(effectLoader);

	return true;
}

RenderTechnique* RenderMgr::GenerateRenderTechnique(const char8* tech)
{
	auto iter = m_techList.find(tech);
	if (iter != m_techList.end())
	{
		RenderTechnique* tech = iter->second;
		return tech->Clone();
	}

	return nullptr;
}

RenderBlock* RenderMgr::GenerateRenderBlock()
{
	RenderBlock* renderBlock = new RenderBlock();
	renderBlock->Init();
	return renderBlock;
}

void RenderMgr::SetDefaultClearParam(Color4 color, float32 depth, uint32 stencil)
{
	m_clearColor = color;
	m_clearDepth = depth;
	m_clearStencil = stencil;
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
	m_renderer->BindTexture(0, tex);
	m_renderer->BindLayout(m_quadLayout);
	m_renderer->DrawImmediate(Render::EDT_TRIANGLELIST, 0, 6);
}

void RenderMgr::Update(float32 fElapsed)
{

}

void RenderMgr::Render(float32 fElapsed)
{
    if (m_defaultFrameBuf == nullptr) return;
    
	m_defaultFrameBuf->ClearFrameBuffer();

 	m_renderer->BindFrameBuffer(m_finalFrameBuf);
 	m_finalFrameBuf->ClearFrameBuffer(m_clearColor, m_clearDepth, m_clearStencil);
	
	if (m_globalShaderData != nullptr)
	{
		//m_globalShaderData->CopyData(&m_globalDataBuffer, sizeof(m_globalDataBuffer));
		m_renderer->BindShaderData(0, m_globalShaderData);
	}

	for (uint32 i = 0; i < m_blockList.size(); i++)
	{
		RenderBlock* block = m_blockList[i];
		if (block != nullptr)
		{
			block->ApplyToDevice();
		}

		m_blockList[i] = nullptr;
	}

	if (m_renderCallback != nullptr)
	{
		m_renderCallback->onRender(fElapsed);
	}

	RenderTexture* finalTargetTex = m_finalTargetTex;
	
	if (m_renderParam.hdrEnabled)
	{
		//m_hdrPostProcess->Apply();

		//finalTargetTex = m_hdrPostProcess->GetOutputPin(0);
	}

	m_renderer->BindFrameBuffer(nullptr);		
	m_renderer->BindProgram(m_postProcessShader);
	DrawFullScreenQuad(finalTargetTex);

	m_renderer->Present();

	m_blockCount = 0;

	//sRenderSpirite->Resume();

	m_renderCount++;
	if (m_timer.GetTime() >= 1.0f)
	{
		m_timer.Reset();
		m_fps = m_renderCount;
		m_renderCount = 0;

		//char8 buf[32];
		//sprintf(buf, "FPS:%d\n", m_fps);
		//OutputDebugStringA(buf);
	}
}
