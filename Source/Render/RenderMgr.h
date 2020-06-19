#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderMgr : public Utils::Singleton<RenderMgr>
		{
		public:
			RenderMgr();
			virtual ~RenderMgr();

		private:
			Timer				m_timer;
			uint32				m_fps;
			uint32				m_renderCount;

			RenderParam				m_renderParam;
			Renderer*				m_renderer;

			RenderTarget*			m_defaultTarget;
			RenderCommand*			m_defaultCommand;
			
//			RenderTarget*			m_finalTarget;
//			RenderCommand*			m_finalCommand;
//			RenderTexture*			m_finalTexture;
			
			RenderBuffer*			m_quadVertBuf;

			map<string, RenderProgram*>		m_programList;
			map<string, RenderPipeline*>	m_pipelineList;

			//使用blockCount主要是为了优化，防止blockList反复变换大小
			vector<RenderObject*>		m_renderObjList;
			//uint32						m_blockCount;
			
		public:
			virtual bool Init();
			virtual void Destroy();

			virtual bool StartUp(const RenderParam& param);

			virtual void DoRender(RenderObject* obj);

			virtual Renderer* GetCurrentRenderer() { return m_renderer; }
			
			virtual uint32 GetFramesPerSecond() { return m_fps; }
			virtual Vector2Df GetPosFromWindowPos(int32 x, int32 y);
			virtual Vector2Df GetSizeFromWindowSize(int32 width, int32 height);

			virtual RenderPipeline* LoadEffectFile(const char8* szPath);
			//virtual RenderTechnique* GenerateRenderTechnique(const char8* tech);
			virtual RenderObject* GenerateRenderObject();
			virtual RenderObject* GetRenderObject(const char8* name);
			
			virtual void SetDefaultClearParam(Color4 color, float32 depth, uint32 stencil);

			virtual void RegisterRenderCallback(IRenderCallback* callback);

			virtual RenderBuffer* GetQuadVertBuffer() { return m_quadVertBuf; }
			virtual void DrawFullScreenQuad(RenderTexture* tex);

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);
		};
	}
}
