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
			struct Vertex
			{
				Vertex(const Vector3Df& pos, const Vector2Df& tex)
				{
					this->pos = pos;
					this->tex = tex;
				}
				Vector3Df	pos;
				Vector2Df	tex;
			};

		private:
			Timer				m_timer;
			uint32				m_fps;
			uint32				m_renderCount;

			RenderParam				m_renderParam;
			Renderer*				m_renderer;
			RenderEffectLoader*		m_effectLoader;

			RenderFrameBuffer*		m_defaultFrameBuf;

			RenderFrameBuffer*		m_finalFrameBuf;
			RenderTexture*			m_finalTargetTex;
			
			RenderTechnique*		m_copyTech;
			RenderProgram*			m_copyShader;
			RenderLayout*			m_copyLayout;

			Color4				m_clearColor;
			float32				m_clearDepth;
			uint32				m_clearStencil;

			list<RenderBlock*>		m_blockList;
			
			IRenderCallback*		m_renderCallback;

			map<string, RenderTechnique*>	m_techList;

			map<string, RenderValue>	m_globalValueList;

		public:
			virtual bool Init();
			virtual void Destroy();

			virtual bool StartUp(const RenderParam& param);

			virtual void DoRender(RenderBlock* block);

			virtual Renderer* GetCurrentRenderer(){ return m_renderer;}
			virtual RenderFrameBuffer* GetDefaultFrameBuffer() { return m_renderer->GetDefaultFrameBuffer(); }
			virtual const char8* GetAdapterName() { return m_renderer->GetAdapterName(); }
			virtual uint32 GetAdapterMemorySize() { return m_renderer->GetAdapterMemorySize(); }
			virtual uint32 GetFramesPerSecond() { return m_fps; }
			virtual Vector2Df GetPosFromWindowPos(int32 x, int32 y);	

			virtual bool LoadEffectFile(const char8* szPath);
			virtual RenderTechnique* GenerateRenderTechnique(const char8* tech);
			virtual RenderBlock* GenerateRenderBlock();
			//virtual RenderFontPtr GenerateRenderFont(IFontPtr font);
			//virtual RenderImagePtr GenerateRenderImage(IImagePtr image);			
			//virtual RenderImagePtr GenerateRenderImage(Color4 color);

			//virtual ModelPtr GenerateModelObject();
			//virtual MeshPtr GenerateMeshObject();
			//virtual MaterialPtr GenerateMaterialObject();

			virtual void SetGlobalRenderValue(const char8* name, const RenderValue& value);
			virtual const RenderValue& GetGlobalRenderValue(const string& name) const;

			virtual void SetDefaultClearParam(Color4 color, float32 depth, uint32 stencil);
			
			virtual void RegisterRenderCallback(IRenderCallback* callback);

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);
		};
	}
}