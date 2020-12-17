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
//	SAFE_DELETE(m_imageTex);
//	SAFE_DELETE(m_imageTech);
//	SAFE_DELETE(m_grayImageTech);
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

    float drawWidth = (float32)drawRect.GetWidth();
    float drawHeight = (float32)drawRect.GetHeight();

	DrawUniform uniform;
	uniform.round = round;
	uniform.scale = drawWidth / drawHeight;
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
    else {
    	//现有RenderSprite对Color 和 Texture 进行了分类处理，以至于在同时有Color 和Texture的情况下，
    	// Color 和 Texture 有互相遮蔽不了的问题(先画所有的Color,后画所有的Texture，Texture就覆盖了Color)，
    	// 所以暂时都以ImageTex来渲染

    	m_imageTex = sRenderer->CreateTexture(1, 1, EPF_RGBA8_UNORM);
    	m_imageTex->CopyFromData(&m_color, sizeof(m_color));

		m_resSet->SetFragmentTexture(0, m_imageTex);
    }

    m_resSet->UpdateSet();

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
    else {
        //现有RenderSprite对Color 和 Texture 进行了分类处理，以至于在同时有Color 和Texture的情况下，
        // Color 和 Texture 有互相遮蔽不了的问题(先画所有的Color,后画所有的Texture，Texture就覆盖了Color)，
        // 所以暂时都以ImageTex来渲染

        m_imageTex = sRenderer->CreateTexture(1, 1, EPF_RGBA8_UNORM);
        m_imageTex->CopyFromData(&m_color, sizeof(m_color));

        m_resSet->SetFragmentTexture(0, m_imageTex);
    }

    m_resSet->UpdateSet();

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
