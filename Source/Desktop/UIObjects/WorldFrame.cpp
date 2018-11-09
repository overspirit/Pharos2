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
	SAFE_DELETE(m_renderFont);
}

bool WorldFrame::Init()
{
	m_renderFont = sDesktopMgr->GenerateRenderFont("Font/simhei.ttf");
	
	m_renderFont->SetFontCharSize(32, 32);

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
		UIObjectPtr uiObj = obj.lock();
		uiObj->Update(fElapsed);
	}
}

void WorldFrame::Render(float32 fElapsed)
{
	if (m_bHidden) return;

	char8 buf[255];

	const char8* adapterName = sRenderMgr->GetAdapterName();
	uint32 memorySize = sRenderMgr->GetAdapterMemorySize();
	sprintf(buf, "%s %dMB", adapterName, memorySize);
	m_renderFont->RenderText(buf, -1, 10, 32);

	uint32 fps = sRenderMgr->GetFramesPerSecond();
	sprintf(buf, "FPS:%d", fps);
	m_renderFont->RenderText(buf, -1, 10, 5);
	

 	//const char8 str1[] = u8"话说宝钗湘云二人计议已妥， 一宿无话。湘云次日便请贾母等赏桂花。";
// 	const char8 str2[] = u8"贾母等都说道：“是他有兴头，须要扰他这雅兴。”至午，果然贾母带了王夫人凤姐兼请薛姨妈等进园来。 ";
// 	const char8 str3[] = u8"贾母因问那一处好山坡下两棵桂花开的又好，河里的水又碧清，坐在河当中亭子上岂不敞亮， 看着水眼也清亮。";
	
	//m_renderFont->RenderText(str1, -1, 100, 100);
	
	for (auto obj : m_childList)
	{
		UIObjectPtr uiObj = obj.lock();
		uiObj->Render(fElapsed);
	}
}
