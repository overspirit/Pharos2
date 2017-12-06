#include "PreCompile.h"
#include "RenderGlobal.h"

OpenGLRenderBlock::OpenGLRenderBlock()
{
    m_drawType = EDT_TRIANGLELIST;
    
    m_startIndex = 0;
    m_countNum = 0;
}

OpenGLRenderBlock::~OpenGLRenderBlock()
{
    
}

bool OpenGLRenderBlock::Init(IRenderLayoutPtr layout, IRenderTechniquePtr tech)
{
    OpenGLRenderTechniquePtr openglTech = static_pointer_cast<OpenGLRenderTechnique>(tech);
    OpenGLRenderLayoutPtr openglLayout = static_pointer_cast<OpenGLRenderLayout>(layout);
    if (openglLayout == nullptr || openglTech == nullptr) return false;
    
    m_layout = openglLayout;
    m_tech = openglTech;
    
    return true;
}

void OpenGLRenderBlock::SetLayoutRange(uint32 start, uint32 count)
{
    m_startIndex = start;
    m_countNum = count;
}

GLenum OpenGLRenderBlock::GetOpenGLDrowMode()
{
    CHECK_ENUM(0, EDT_POINTLIST);
    CHECK_ENUM(1, EDT_LINELIST);
    CHECK_ENUM(2, EDT_LINESTRIP);
    CHECK_ENUM(3, EDT_TRIANGLELIST);
    CHECK_ENUM(4, EDT_TRIANGLESTRIP);
    
    const static GLenum prims[] =
    {
        GL_POINTS,          //EDraw_PointList
        GL_LINES,           //EDraw_LineList
        GL_LINE_STRIP,      //EDraw_LineStrip
        GL_TRIANGLES,       //EDraw_TriangleList
        GL_TRIANGLE_STRIP,  //EDraw_TriangleStrip
    };
    
    return prims[m_drawType];
}

void OpenGLRenderBlock::ApplyToDevice()
{
    if (m_layout == nullptr || m_tech == nullptr) return;
    
    if (m_countNum == 0)
    {
        m_startIndex = 0;
        
        if (m_layout->IsUseIndexBuffer())
        {
            m_countNum = m_layout->GetIndexNum();
        }
        else
        {
            m_countNum = m_layout->GetVertNum();
        }
    }
    
    if (!m_layout->IsBuildInput())
    {
        m_layout->BuildInput();
    }
    
    m_tech->ApplyToDevice();
    m_layout->ApplyToDevice();
    
    GLenum drawMode = GetOpenGLDrowMode();
    
    for (uint32 i = 0; i < m_tech->GetPassNum(); i++)
    {
        OpenGLRenderPassPtr openglpass = static_pointer_cast<OpenGLRenderPass>(m_tech->GetPass(i));
        if (openglpass == nullptr) break;
        
        openglpass->ApplyToDevice();
        
        if (m_layout->IsUseIndexBuffer())
        {
            glDrawElements(drawMode, m_countNum, GL_UNSIGNED_INT, BUFFER_OFFSET(m_startIndex));
        }
        else
        {
            glDrawArrays(drawMode, m_startIndex, m_countNum);
        }
    }
    
}

