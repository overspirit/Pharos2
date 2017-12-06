#pragma once

namespace Pharos
{
    namespace Render
    {
        class OpenGLRenderTechnique : public RenderTechnique
        {
        public:
			OpenGLRenderTechnique();
            virtual ~OpenGLRenderTechnique();

		public:
			virtual bool Create(const TechniqueInfo& info);

			virtual RenderTechniquePtr Clone();

			virtual void ApplyToDevice();
        };
    }
    
    typedef std::shared_ptr<Render::OpenGLRenderTechnique> OpenGLRenderTechniquePtr;
}
