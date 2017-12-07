#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

RenderMgr::RenderMgr()
{
	m_fps = 0;
	m_renderCount = 0;

	m_renderCallback = nullptr;

	m_clearColor = 0xFF7F7F7F;//R=43,G=147,B=223
	m_clearDepth = 1.0f;
	m_clearStencil = 0;
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
	SAFE_DELETE(m_copyTech);
	SAFE_DELETE(m_copyLayout);

	SAFE_DELETE(m_finalFrameBuf);

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

	m_defaultFrameBuf = m_renderer->GetDefaultFrameBuffer();

	this->LoadEffectFile("Shader/Sprite3D.fxml");
	this->LoadEffectFile("Shader/Sprite2D.fxml");
	this->LoadEffectFile("Shader/Font.fxml");
	this->LoadEffectFile("Shader/Skeletal.fxml");
	this->LoadEffectFile("Shader/Copy.fxml");

	m_copyTech = this->GenerateRenderTechnique("Copy");
	RenderPass* copyPass = m_copyTech->GetPass(0);
	m_copyShader = copyPass->GetShaderProgram();

	Vertex vertData[] =
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
	m_copyLayout = m_renderer->GenerateRenderLayout(sizeof(vertData), &vertDataBuf);

	VertLayoutDesc desc[] =
	{
		{ VET_FLOAT32, 3, "POSITION", 0, 0 },
		{ VET_FLOAT32, 2, "TEXCOORD", 0, 12 },
	};
	m_copyLayout->SetInputLayoutDesc(desc, 2);

	uint32 finalBufferWidth = m_defaultFrameBuf->GetWidth();
	uint32 finalBufferHeight = m_defaultFrameBuf->GetHeight();
	m_finalFrameBuf = m_renderer->CreateFrameBuffer(finalBufferWidth, finalBufferHeight);
	m_finalTargetTex = m_finalFrameBuf->CreateRenderTexture(0, EPF_RGBA8_UNORM);

	//if (!sRenderSpirite->Init(m_renderer)) return false;

	return true;
}

void RenderMgr::SetGlobalRenderValue(const char8* name, const RenderValue& value)
{
	m_globalValueList[name] = value;
}

const RenderValue& RenderMgr::GetGlobalRenderValue(const char8* name) const
{
	auto iter = m_globalValueList.find(name);
	if (iter != m_globalValueList.end())
	{
		return iter->second;
	}

	static RenderValue value;
	return value;
}

void RenderMgr::DoRender(RenderBlock* block)
{
	if (block == nullptr) return;
	m_blockList.push_back(block);
}

bool RenderMgr::LoadEffectFile(const char8* szPath)
{
	RenderEffectLoader* effectLoader = MakeEffectLoader();
	if (!effectLoader->Load(szPath))
	{
		SAFE_DELETE(effectLoader);
		return false;
	}

	for (uint32 i = 0; i < effectLoader->GetRenderTechniqueNum(); i++)
	{
		RenderTechnique* tech = effectLoader->GetRenderTechnique(i);
		string techName = tech->GetTechName();
		m_techList[techName] = tech;
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

RenderBlock* RenderMgr::GenerateRenderBlock(RenderLayout* layout, RenderTechnique* tech)
{
	D3D11RenderBlock* block = new D3D11RenderBlock();
	if (!block->Init(layout, tech)) return nullptr;

	return block;
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

// IRenderFontPtr RenderMgr::GenerateRenderFont(IFontPtr font)
// {
// 	RenderFontPtr renderFont = MakeSharedPtr<RenderFont>();
// 	if (!renderFont->InitFromFont(font)) return nullptr;
// 	return renderFont;
// }
// 
// IRenderImagePtr RenderMgr::GenerateRenderImage(IImagePtr image)
// {
// 	RenderImagePtr renderImage = MakeSharedPtr<RenderImage>();
// 	if (!renderImage->InitFromImage(image)) return nullptr;
// 	return renderImage;
// }
// 
// IRenderImagePtr RenderMgr::GenerateRenderImage(Color4 color)
// {
// 	RenderImagePtr renderImage = MakeSharedPtr<RenderImage>();
// 	if (!renderImage->InitFromColor(color)) return nullptr;
// 	return renderImage;
// }

void RenderMgr::Update(float32 fElapsed)
{

}

void RenderMgr::Render(float32 fElapsed)
{
	m_defaultFrameBuf->ClearFrameBuffer();

	m_renderer->BindFrameBuffer(m_finalFrameBuf);
	m_finalFrameBuf->ClearFrameBuffer(m_clearColor, m_clearDepth, m_clearStencil);

	for (auto& block : m_blockList)
	{
		block->ApplyToDevice();

		SAFE_DELETE(block);
	}

	m_renderer->BindFrameBuffer(nullptr);
	m_renderer->BindTexture(0, m_finalTargetTex);
	m_renderer->BindLayout(m_copyLayout);
	m_renderer->BindProgram(m_copyShader);
	m_renderer->DrawImmediate(Render::EDT_TRIANGLELIST, 0, 6);

	if (m_renderCallback != nullptr)
	{
		m_renderCallback->onRender(fElapsed);
	}

	m_renderer->Present();

	m_blockList.clear();

	//sRenderSpirite->Resume();

	m_renderCount++;
	if (m_timer.GetTime() >= 1.0f)
	{
		m_timer.Reset();
		m_fps = m_renderCount;
		m_renderCount = 0;

		char8 buf[32];
		sprintf(buf, "FPS:%d\n", m_fps);
		OutputDebugStringA(buf);
	}
}
