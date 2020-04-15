#include "PreCompile.h"
#include "Pharos.h"

RenderImage::RenderImage(void)
{
	m_image = nullptr;

	m_imageTex = nullptr;

	m_imageTech = nullptr;
	m_imageTexVar = nullptr;

	m_grayImageTech = nullptr;
	m_grayImageTexVar = nullptr;
}

RenderImage::~RenderImage(void)
{
	SAFE_DELETE(m_imageTex);
	SAFE_DELETE(m_imageTech);
	SAFE_DELETE(m_grayImageTech);
}

bool RenderImage::LoadImage(const char8* imageFilePath)
{
	m_image = sResMgr->GenerateImage(imageFilePath);
	if (m_image == nullptr) return false;

	Renderer* renderer = sRenderMgr->GetCurrentRenderer();
	m_imageTex = renderer->LoadTexture(m_image);
	if (m_imageTex == nullptr) return false;

	m_imageTech = sRenderMgr->GenerateRenderTechnique("Sprite2DImage");
	m_imageTexVar = m_imageTech->GetVariable("g_tex");
	m_imageTexVar->SetValue(m_imageTex);

	m_grayImageTech = sRenderMgr->GenerateRenderTechnique("Sprite2DGray");
	m_grayImageTexVar = m_grayImageTech->GetVariable("g_tex");
	m_grayImageTexVar->SetValue(m_imageTex);

	return true;
}

void RenderImage::RenderImageRect(const Rect2Di& imageRect, Color4 imageColor, const Rect2Di& drawRect)
{
	//计算在屏幕的位置
	Vector2Df leftTop = sRenderMgr->GetPosFromWindowPos(drawRect.left, drawRect.top);
	Vector2Df rightBottom = sRenderMgr->GetPosFromWindowPos(drawRect.right, drawRect.bottom);

	uint32 imageWidth = m_imageTex->GetWidth();
	uint32 imageHeight = m_imageTex->GetHeight();
	float32 texLeft = (float32)imageRect.left / imageWidth;
	float32 texTop = (float32)imageRect.top / imageHeight;
	float32 texRight = (float32)imageRect.right / imageWidth;
	float32 texBottom = (float32)imageRect.bottom / imageHeight;

	//建立顶点数据
	DecalColorVertex vt[] =
	{
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		imageColor,		Vector2Df(texLeft, texTop) },
		{ Vector3Df(rightBottom.x,	leftTop.y,		0),		imageColor,		Vector2Df(texRight, texTop) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		imageColor,		Vector2Df(texRight, texBottom) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		imageColor,		Vector2Df(texRight, texBottom) },
		{ Vector3Df(leftTop.x,		rightBottom.y,	0),		imageColor,		Vector2Df(texLeft, texBottom) },
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		imageColor,		Vector2Df(texLeft, texTop) },
	};

	sDesktopMgr->PushRenderPatch(vt, 6, m_imageTech);
}

void RenderImage::RenderGrayImageRect(const Rect2Di& imageRect, const Rect2Di& drawRect)
{
	//计算在屏幕的位置
	Vector2Df leftTop = sRenderMgr->GetPosFromWindowPos(drawRect.left, drawRect.top);
	Vector2Df rightBottom = sRenderMgr->GetPosFromWindowPos(drawRect.right, drawRect.bottom);

	uint32 imageWidth = m_imageTex->GetWidth();
	uint32 imageHeight = m_imageTex->GetHeight();
	float32 texLeft = (float32)imageRect.left / imageWidth;
	float32 texTop = (float32)imageRect.top / imageHeight;
	float32 texRight = (float32)imageRect.right / imageWidth;
	float32 texBottom = (float32)imageRect.bottom / imageHeight;

	//建立顶点数据
	DecalColorVertex vt[] =
	{
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		0xFFFFFFFF,		Vector2Df(texLeft, texTop) },
		{ Vector3Df(rightBottom.x,	leftTop.y,		0),		0xFFFFFFFF,		Vector2Df(texRight, texTop) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		0xFFFFFFFF,		Vector2Df(texRight, texBottom) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		0xFFFFFFFF,		Vector2Df(texRight, texBottom) },
		{ Vector3Df(leftTop.x,		rightBottom.y,	0),		0xFFFFFFFF,		Vector2Df(texLeft, texBottom) },
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		0xFFFFFFFF,		Vector2Df(texLeft, texTop) },
	};

	sDesktopMgr->PushRenderPatch(vt, 6, m_grayImageTech);
}
