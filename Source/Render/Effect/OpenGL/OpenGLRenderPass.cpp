#include "PreCompile.h"
#include "RenderGlobal.h"

OpenGLRenderPass::OpenGLRenderPass(void)
{
    m_programId = 0;
}

OpenGLRenderPass::~OpenGLRenderPass(void)
{
    glDeleteProgram(m_programId);
}

bool OpenGLRenderPass::Create(const PassInfo& passInfo, const PassBindInfo& bindInfo)
{
    GLuint vertId = CompileShader(GL_VERTEX_SHADER, passInfo.vertShader.c_str());
    GLuint fragId = CompileShader(GL_FRAGMENT_SHADER, passInfo.pixelShader.c_str());
    if (vertId == 0 || fragId == 0) { return false; }
    
    m_programId = glCreateProgram();
    
    glAttachShader(m_programId, vertId);
    glAttachShader(m_programId, fragId);
    
    for (int32 i = 0; i < bindInfo.attrInfos.size(); i++)
    {
        string name = bindInfo.attrInfos[i].first;
        uint32 bind = bindInfo.attrInfos[i].second;
        glBindAttribLocation(m_programId, bind, name.c_str());
    }
    
    glLinkProgram(m_programId);
    
    if (vertId != 0)
    {
        glDetachShader(m_programId, vertId);
        glDeleteShader(vertId);
    }
    
    if (fragId != 0)
    {
        glDetachShader(m_programId, fragId);
        glDeleteShader(fragId);
    }
    
    glUseProgram(m_programId);
    
    for (uint32 i = 0; i < bindInfo.uniformInfos.size(); i++)
    {
        string name = bindInfo.uniformInfos[i].name;
        uint32 bind = bindInfo.uniformInfos[i].bind;
        
        GLint texLoc = glGetUniformLocation(m_programId, name.c_str());
        glUniform1i(texLoc, bind);
    }
    
    glUseProgram(0);
    
    for (uint32 i = 0; i < bindInfo.blockInfos.size(); i++)
    {
        string name = bindInfo.blockInfos[i].name;
        uint32 bind = bindInfo.blockInfos[i].bind;
        
        GLuint bufIndex = glGetUniformBlockIndex(m_programId, name.c_str());
        if (bufIndex == GL_INVALID_INDEX) continue;
        
        glUniformBlockBinding(m_programId, bufIndex, bind);
        
        //GLint buffSize = 0;
        //glGetActiveUniformBlockiv(m_programId, bufIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &buffSize);
        //buffSize = buffSize;
    }
    
    IRendererPtr renderer = g_pRenderMgr->GetCurrentRenderer();
    m_depthState = static_pointer_cast<OpenGLDepthStencilState>(renderer->CreateDepthStencilState(passInfo.depthDesc));
    m_rasterizerState = static_pointer_cast<OpenGLRasterizerState>(renderer->CreateRasterizerState(passInfo.rasterDesc));
    m_blendState = static_pointer_cast<OpenGLBlendState>(renderer->CreateBlendState(passInfo.blendDesc));
    
    return true;
}

GLuint OpenGLRenderPass::CompileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar* log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, log);
        printf("compile error:\n%s\n", log);
        free(log);
    }
    
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

OpenGLRenderPassPtr OpenGLRenderPass::Clone()
{
    OpenGLRenderPassPtr pass = MakeSharedPtr<OpenGLRenderPass>();
    
    pass->m_programId = m_programId;
    
#pragma message("Clone pass another var!!!")
    //....
    
    if(m_blendState != nullptr) pass->m_blendState = static_pointer_cast<OpenGLBlendState>(m_blendState->Clone());
    if (m_depthState != nullptr) pass->m_depthState = static_pointer_cast<OpenGLDepthStencilState>(m_depthState->Clone());
    if (m_rasterizerState != nullptr) pass->m_rasterizerState = static_pointer_cast<OpenGLRasterizerState>(m_rasterizerState->Clone());
    
    //if(m_frameBuf != nullptr) pass->m_frameBuf = m_frameBuf->Clone();
    
    return pass;
}

void OpenGLRenderPass::BindFrameBuffer(IRenderFrameBufferPtr frameBuf)
{
#pragma message("OpenGLRenderPass::BindFrameBuffer is empty!!!")
}

void OpenGLRenderPass::BindRasterizerState(IRenderRasterizerStatePtr state)
{
    m_rasterizerState = static_pointer_cast<OpenGLRasterizerState>(state);
}

void OpenGLRenderPass::BindDepthStencilState(IRenderDepthStencilStatePtr state)
{
    m_depthState = static_pointer_cast<OpenGLDepthStencilState>(state);
}

void OpenGLRenderPass::BindBlendState(IRenderBlendStatePtr state)
{
    m_blendState = static_pointer_cast<OpenGLBlendState>(state);
}

void OpenGLRenderPass::ApplyToDevice()
{
    m_depthState->ApplyDevice();
    m_rasterizerState->ApplyDevice();
    m_blendState->ApplyDevice();
    
    glUseProgram(m_programId);
}
