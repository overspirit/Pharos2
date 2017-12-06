#pragma once

namespace Pharos
{
    namespace Render
    {
        class OpenGLRenderBlock : public IRenderBlock
        {
        public:
			OpenGLRenderBlock(void);
            virtual ~OpenGLRenderBlock(void);
            
        private:
            OpenGLRenderTechniquePtr		m_tech;
            OpenGLRenderLayoutPtr           m_layout;
            
            DrawType				m_drawType;
            
            uint32			m_startIndex;
            uint32			m_countNum;

        public:
            virtual GLenum GetOpenGLDrowMode();
            
		public:
            virtual bool Init(IRenderLayoutPtr layout, IRenderTechniquePtr tech);
            virtual IRenderTechniquePtr GetTechnique() { return m_tech; }
            virtual IRenderLayoutPtr GetLayout() { return m_layout; }
            virtual void SetLayoutRange(uint32 start, uint32 count);
            virtual void SetDrawType(DrawType type) { m_drawType = type; }
            virtual uint32 GetLayoutDrawStart() { return m_startIndex; }
            virtual uint32 GetLayoutDrawCount() { return m_countNum; }
            virtual void ApplyToDevice();
        };
    }
    
    typedef std::shared_ptr<Render::OpenGLRenderBlock> OpenGLRenderBlockPtr;
}

