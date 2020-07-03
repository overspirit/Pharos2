#pragma once

namespace Pharos
{
	namespace Render
	{
		class Material
		{
		public:
			Material();
			virtual ~Material();

		private:
			string	m_techName;

			RenderPipeline*		m_renderPipeline;
			RenderResourceSet*	m_renderSet;

			vector<RenderTexture*>		m_texList;

		public:
			virtual bool SetRenderTechnique(const char8* techName);
			virtual RenderPipeline* GetRenderPipeline() { return m_renderPipeline; }			

			virtual bool SetTexture(const char8* valueName, RenderTexture* texture);
			virtual RenderTexture* GetTexture(const char8* valueName) { return m_texList[1]; }		
		};
	}
}
