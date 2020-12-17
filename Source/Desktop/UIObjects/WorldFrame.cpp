#include "PreCompile.h"
#include "Pharos.h"

WorldFrame::WorldFrame(void)
{
	m_type = "WorldFrame";

	m_renderFont = nullptr;
}

WorldFrame::~WorldFrame(void)
{
	SAFE_DELETE(m_renderFont);
}

bool WorldFrame::Init()
{
	m_renderFont = sDesktopMgr->GenerateRenderFont("Font/simhei.ttf");
    if(m_renderFont == nullptr) return false;
	
	m_renderFont->SetFontCharSize(28, 28);

	return true;
}

void WorldFrame::SetSize(int32 width, int32 height)
{
	m_size.width = width;
	m_size.height = height;
}

void WorldFrame::Update(float32 fElapsed)
{
	if (m_bHidden) return;

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = m_size.width;
	m_rect.bottom = m_size.height;

	m_center.x = (m_rect.left + m_rect.right) / 2;
	m_center.y = (m_rect.top + m_rect.bottom) / 2;

	for (auto obj : m_childList)
	{
		obj->Update(fElapsed);
	}
}

void WorldFrame::Render(float32 fElapsed)
{
	if (m_bHidden) return;
    if (m_renderFont == nullptr) return;

	char8 buf[255];

	uint32 fps = sRenderMgr->GetFramesPerSecond();
	sprintf(buf, "FPS:%d", fps);
	m_renderFont->RenderText(buf, -1, 16, 8);
	
	const char8* adapterName = sRenderer->GetAdapterName();
	uint32 memorySize = sRenderer->GetAdapterMemorySize();
	sprintf(buf, "%s %dMB ", adapterName, memorySize);
	m_renderFont->RenderText(buf, -1, 16, 32);

	for (auto obj : m_childList)
	{
		obj->Render(fElapsed);
	}
}
