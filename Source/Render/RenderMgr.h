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

		private:
			Timer				m_timer;
			uint32				m_fps;
			uint32				m_renderCount;

			RenderParam				m_renderParam;
			Renderer*				m_renderer;

			RenderTarget*			m_defaultTarget;
			RenderCommand*			m_defaultCommand;			

			RenderResourceSet*		m_finalResourceSet;
			RenderProgram*			m_finalProgram;
			RenderPipeline*			m_finalPipeline;
			RenderTarget*			m_finalTarget;
			RenderTexture*			m_finalColorTexture;
			RenderTexture*			m_finalDepthTexture;

			RenderBuffer*			m_quadVertBuf;

			//使用blockCount主要是为了优化，防止blockList反复变换大小
			vector<RenderObject*>		m_renderObjList;
			//uint32						m_blockCount;

			IRenderCallback*		m_renderCallback;

		private:
			void InitQuadBuffer();
			
		public:
			virtual bool Init();
			virtual void Destroy();

			virtual bool StartUp(const RenderParam& param);

			virtual void DoRender(RenderObject* obj);

			virtual Renderer* GetCurrentRenderer() { return m_renderer; }
			
			virtual uint32 GetFramesPerSecond() { return m_fps; }
			virtual Vector2Df GetPosFromWindowPos(int32 x, int32 y);
			virtual Vector2Df GetSizeFromWindowSize(int32 width, int32 height);

			virtual RenderObject* GenerateRenderObject();
			
			virtual void RegisterRenderCallback(IRenderCallback* callback);

			virtual RenderBuffer* GetQuadVertBuffer() { return m_quadVertBuf; }
			virtual void DrawFullScreenQuad(RenderTexture* tex);

			virtual void Render(float32 fElapsed);

			virtual void SaveRenderTarget(const char8* path);
		};
	}
}
