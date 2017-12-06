#pragma once

namespace Pharos
{
    namespace Render
    {
        struct PassBindInfo
        {
            vector<pair<string, uint32>>        attrInfos;
            vector<UniformInfo>                 uniformInfos;
            vector<BlockInfo>                   blockInfos;
        };
        
        class OpenGLRenderPass : public IRenderPass
        {
        public:
			OpenGLRenderPass(void);
            virtual ~OpenGLRenderPass(void);
  
		private:
			GLuint  m_programId;

            OpenGLBlendStatePtr             m_blendState;
            OpenGLDepthStencilStatePtr      m_depthState;
            OpenGLRasterizerStatePtr		m_rasterizerState;

		private:
			GLuint CompileShader(GLenum type, const char* source);

		public:
			virtual bool Create(const PassInfo& passInfo, const PassBindInfo& bindInfo);

			virtual shared_ptr<OpenGLRenderPass> Clone();

			virtual void BindFrameBuffer(IRenderFrameBufferPtr frameBuf);
			virtual void BindRasterizerState(IRenderRasterizerStatePtr state);
			virtual void BindDepthStencilState(IRenderDepthStencilStatePtr state);
			virtual void BindBlendState(IRenderBlendStatePtr state);

			virtual void ApplyToDevice();
        };
    }
    
    typedef std::shared_ptr<Render::OpenGLRenderPass> OpenGLRenderPassPtr;
}
