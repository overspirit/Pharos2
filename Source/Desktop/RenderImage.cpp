#include "PreCompile.h"
#include "Pharos.h"

RenderImage::RenderImage(void)
{
	m_image = nullptr;
	m_color = 0xFFFFFFFF;

	m_imageTex = nullptr;
	m_drawUniform = nullptr;
	m_resSet = nullptr;
}

RenderImage::~RenderImage(void)
{
	SAFE_DELETE(m_imageTex);
	SAFE_DELETE(m_drawUniform);
	SAFE_DELETE(m_resSet);
}

bool RenderImage::Init()
{
	m_drawUniform = sRenderer->GenerateRenderBuffer(UNIFORM_BUFFFER);
	m_drawUniform->Allocate(sizeof(DrawUniform));

	m_resSet = sRenderer->GenerateRenderResuourceSet();
	m_resSet->SetFragmentUniformBuffer(1, m_drawUniform);

	return true;
}

bool RenderImage::LoadImage(const char8* imageFilePath)
{
	m_image = sResMgr->GenerateImage(imageFilePath);
	if (m_image == nullptr) return false;

	m_imageTex = sRenderer->LoadTexture(m_image);
	if (m_imageTex == nullptr) return false;

	m_resSet->SetFragmentTexture(0, m_imageTex);

	return true;
}

void RenderImage::RenderRect(const Rect2Di& imageRect, const Rect2Di& drawRect, float32 round)
{
    //计算在屏幕的位置
    Vector2Df leftTop = sRenderMgr->GetPosFromWindowPos(drawRect.left, drawRect.top);
    Vector2Df rightBottom = sRenderMgr->GetPosFromWindowPos(drawRect.right, drawRect.bottom);

	DrawUniform uniform;
	uniform.round = round;
	uniform.scale = (float32)drawRect.GetWidth() / drawRect.GetHeight();
	m_drawUniform->CopyData(&uniform, sizeof(DrawUniform));

    float32 texLeft = 0;
    float32 texTop = 0;
    float32 texRight = 1.0f;
    float32 texBottom = 1.0f;

    if (m_imageTex != NULL)
    {
        uint32 imageWidth = m_imageTex->GetWidth();
        uint32 imageHeight = m_imageTex->GetHeight();
        texLeft = (float32) imageRect.left / imageWidth;
        texTop = (float32) imageRect.top / imageHeight;
        texRight = (float32) imageRect.right / imageWidth;
        texBottom = (float32) imageRect.bottom / imageHeight;
    }

	//建立顶点数据
	DecalColorVertex vt[] =
	{
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		m_color,		Vector2Df(texLeft, texTop) },
		{ Vector3Df(rightBottom.x,	leftTop.y,		0),		m_color,		Vector2Df(texRight, texTop) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		m_color,		Vector2Df(texRight, texBottom) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		m_color,		Vector2Df(texRight, texBottom) },
		{ Vector3Df(leftTop.x,		rightBottom.y,	0),		m_color,		Vector2Df(texLeft, texBottom) },
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		m_color,		Vector2Df(texLeft, texTop) },
	};

    if (m_imageTex == NULL)
    {
        sRenderSpirite->DrawColor2D(vt, 6, m_resSet);
    }
    else
    {
        sRenderSpirite->DrawTexture2D(vt, 6, m_resSet);
    }
}

void RenderImage::RenderGrayRect(const Rect2Di& imageRect, const Rect2Di& drawRect, float32 round)
{
    //计算在屏幕的位置
    Vector2Df leftTop = sRenderMgr->GetPosFromWindowPos(drawRect.left, drawRect.top);
    Vector2Df rightBottom = sRenderMgr->GetPosFromWindowPos(drawRect.right, drawRect.bottom);

	DrawUniform uniform;
	uniform.round = round;
    uniform.scale = (rightBottom.x - leftTop.x) / (rightBottom.y - leftTop.y);
	m_drawUniform->CopyData(&uniform, sizeof(DrawUniform));

    float32 texLeft = 0;
    float32 texTop = 0;
    float32 texRight = 1.0f;
    float32 texBottom = 1.0f;

    if (m_imageTex != NULL)
    {
        uint32 imageWidth = m_imageTex->GetWidth();
        uint32 imageHeight = m_imageTex->GetHeight();
        texLeft = (float32) imageRect.left / imageWidth;
        texTop = (float32) imageRect.top / imageHeight;
        texRight = (float32) imageRect.right / imageWidth;
        texBottom = (float32) imageRect.bottom / imageHeight;
    }

	//建立顶点数据
	DecalColorVertex vt[] =
	{
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		m_color,		Vector2Df(texLeft, texTop) },
		{ Vector3Df(rightBottom.x,	leftTop.y,		0),		m_color,		Vector2Df(texRight, texTop) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		m_color,		Vector2Df(texRight, texBottom) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		m_color,		Vector2Df(texRight, texBottom) },
		{ Vector3Df(leftTop.x,		rightBottom.y,	0),		m_color,		Vector2Df(texLeft, texBottom) },
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		m_color,		Vector2Df(texLeft, texTop) },
	};
	
	//sRenderSpirite->DrawTexture2D(vt, 6, m_grayImageTech);
}
