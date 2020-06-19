#include "PreCompile.h"
#include "Pharos.h"

RenderSprite::RenderSprite(void)
{
//	m_renderTech = nullptr;
}

RenderSprite::~RenderSprite(void)
{
//	SAFE_DELETE(m_renderTech);
}

void RenderSprite::RenderRect(Color4 color, const Rect2Di& drawRect)
{
	//计算在屏幕的位置
	Vector2Df leftTop = sRenderMgr->GetPosFromWindowPos(drawRect.left, drawRect.top);
	Vector2Df rightBottom = sRenderMgr->GetPosFromWindowPos(drawRect.right, drawRect.bottom);

	//建立顶点数据
	DecalColorVertex vt[] =
	{
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(rightBottom.x,	leftTop.y,		0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(leftTop.x,		rightBottom.y,	0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		color,		Vector2Df(0, 0) },
	};
	
//	if (m_renderTech == nullptr)
//	{
//		m_renderTech = sRenderMgr->GenerateRenderTechnique("Sprite2DColor");
//	}
//
//	sDesktopMgr->PushRenderPatch(vt, 6, m_renderTech);
}

void RenderSprite::RenderWireFrame(Color4 color, const Rect2Di& drawRect)
{
	//计算在屏幕的位置
	Vector2Df leftTop = sRenderMgr->GetPosFromWindowPos(drawRect.left, drawRect.top);
	Vector2Df rightBottom = sRenderMgr->GetPosFromWindowPos(drawRect.right, drawRect.bottom);

	//建立顶点数据
	DecalColorVertex vt[] =
	{
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(rightBottom.x,	leftTop.y,		0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(rightBottom.x,	leftTop.y,		0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(leftTop.x,		rightBottom.y,	0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(leftTop.x,		rightBottom.y,	0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		color,		Vector2Df(0, 0) },
	};
	
//	if (m_renderTech == nullptr)
//	{
//		m_renderTech = sRenderMgr->GenerateRenderTechnique("Sprite2DColor");
//	}
//
//	sDesktopMgr->PushRenderPatch(vt, 8, m_renderTech, EDT_LINELIST);
}
