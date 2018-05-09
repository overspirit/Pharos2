#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "DesktopGlobal.h"

WorldFrame::WorldFrame(void)
{
	m_type = "WorldFrame";
}

WorldFrame::~WorldFrame(void)
{
}

bool WorldFrame::Init()
{
	//Font font;
	//font.Open("Font/simhei.ttf");
	//m_renderFont = g_pRenderMgr->GenerateRenderFont(font);
	//m_renderFont->SetFontSize(32);

	//m_renderer = g_pRenderMgr->GetCurrentRenderer();

	return true;
}

void WorldFrame::SetSize(int32 width, int32 height)
{
	m_size.width = width;
	m_size.height = height;
}

void WorldFrame::Update(float32 fElapsed)
{
	if(m_bHidden) return;
	
	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = m_size.width;
	m_rect.bottom = m_size.height;

	m_center.x = (m_rect.left + m_rect.right) / 2;
	m_center.y = (m_rect.top + m_rect.bottom) / 2;

	for (auto obj : m_childList)
	{
		UIObjectPtr uiObj = obj.lock();
		uiObj->Update(fElapsed);
	}
}

void WorldFrame::Render(float32 fElapsed)
{    
	if(m_bHidden) return;
	
	char8 buf[255];

	const char8* adapterName = sRenderMgr->GetAdapterName();
	uint32 memorySize = sRenderMgr->GetAdapterMemorySize();
	sprintf(buf, "%s %dMB", adapterName, memorySize);
	//m_renderFont->DrawText(buf, 10, 10);

	uint32 fps = sRenderMgr->GetFramesPerSecond();
	sprintf(buf, "FPS:%d", fps);
	//m_renderFont->DrawText(buf, 10, 40);

	for (auto obj : m_childList)
	{
		UIObjectPtr uiObj = obj.lock();
		uiObj->Render(fElapsed);
	}
}
