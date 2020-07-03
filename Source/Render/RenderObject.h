#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderObject
		{
		public:
			RenderObject(RenderCommand* renderCmd);
			virtual ~RenderObject();

		private:
			struct SceneMat
			{
				Matrix4		view;
				Matrix4		proj;
			};

			struct RenderBlock
			{
				RenderBuffer*           		vertBuf;
				RenderBuffer*           		indexBuf;
				RenderPipeline*					pipeline;				
				vector<RenderTexture*>			texList;

				DrawType	drawType;
			};


		private:
			RenderCommand*					m_renderCmd;

			vector<RenderBuffer*>           m_uniformBufs;

			vector<RenderBlock>				m_blockList;

			vector<RenderResourceSet*>		m_resSetList;

			SceneMat		m_sceneMat;

		public:
			virtual bool Init();
			
			// virtual bool SetParameterValue(const char8* valueName, float32 value);
			// virtual bool SetParameterValue(const char8* valueName, const Vector2Df& value);
			// virtual bool SetParameterValue(const char8* valueName, const Vector3Df& value);
			// virtual bool SetParameterValue(const char8* valueName, const Vector4Df& value);
			virtual bool SetParameterValue(const char8* valueName, const Matrix4& value);
			// virtual bool SetParameterValue(const char8* valueName, RenderTexture* value);

			virtual uint32 AddRenderBlock(RenderBuffer* vertBuf, RenderPipeline* pipeline);
			virtual void SetBlockIndexBuffer(uint32 blockIndex, RenderBuffer* indexBuffer);
			virtual void SetBlockTexture(uint32 blockIndex, uint32 slot, RenderTexture* tex);
			virtual void SetBlockDrawType(uint32 blockIndex, DrawType drawType) { m_blockList[blockIndex].drawType = drawType; }
			virtual uint32 GetRenderBlockNum() { return m_blockList.size(); }

			virtual void Apply();
			
			virtual void Draw();
		};
	}
}
