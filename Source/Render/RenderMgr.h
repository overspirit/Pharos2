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
			struct GlobalData
			{
				Matrix4		viewMatrix;
				Matrix4		projMatrix;
				Vector4Df	eyePosition;
			};

		private:
			Timer				m_timer;
			uint32				m_fps;
			uint32				m_renderCount;

			RenderParam				m_renderParam;
			Renderer*				m_renderer;

			RenderTarget*			m_defaultTarget;
			RenderPipeline*			m_defaultPipeline;
			RenderProgram*			m_defaultProgram;
			RenderCommand*			m_defaultCommand;
			
			RenderProgram*			m_finalProgram;
			RenderPipeline*			m_finalPipeline;
			RenderTarget*			m_finalTarget;
			RenderCommand*			m_finalCommand;
			RenderTexture*			m_finalTexture;
			
			RenderBuffer*			m_quadVertBuf;

			IRenderCallback*		m_renderCallback;

			map<string, RenderTechnique*>	m_techList;

			//RenderShaderData*		m_globalShaderData;

			//使用blockCount主要是为了优化，防止blockList反复变换大小
			vector<RenderBlock*>		m_blockList;
			uint32						m_blockCount;
			
		public:
			virtual bool Init();
			virtual void Destroy();

			virtual bool StartUp(const RenderParam& param);

			virtual void DoRender(RenderBlock* block);

			virtual Renderer* GetCurrentRenderer() { return m_renderer; }
			virtual RenderTarget* GetDefaultRenderTarget() { return m_renderer->GetDefaultRenderTarget(); }
			virtual const char8* GetAdapterName() { return m_renderer->GetAdapterName(); }
			virtual uint32 GetAdapterMemorySize() { return m_renderer->GetAdapterMemorySize(); }
			virtual uint32 GetFramesPerSecond() { return m_fps; }
			virtual Vector2Df GetPosFromWindowPos(int32 x, int32 y);
			virtual Vector2Df GetSizeFromWindowSize(int32 width, int32 height);

			virtual bool LoadEffectFile(const char8* szPath);
			virtual RenderTechnique* GenerateRenderTechnique(const char8* tech);
			virtual RenderBlock* GenerateRenderBlock();
			virtual RenderObject* GenerateRenderObject();
			virtual RenderObject* GetRenderObject(const char8* name);

			virtual void SetGlobalRenderViewMatrix(const Matrix4& viewMatrix);
			virtual void SetGlobalRenderProjMatrix(const Matrix4& projMatrix);
			virtual void SetGlobalRenderEyePostion(const Vector3Df& eyePos);

			virtual void SetDefaultClearParam(Color4 color, float32 depth, uint32 stencil);

			virtual void RegisterRenderCallback(IRenderCallback* callback);

			virtual RenderBuffer* GetQuadVertBuffer() { return m_quadVertBuf; }
			virtual void DrawFullScreenQuad(RenderTexture* tex);

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);
		};
	}
}
