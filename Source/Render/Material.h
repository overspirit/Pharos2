#pragma once

namespace Pharos
{
	namespace Render
	{
		//shader, 状态， 参数， 纹理， 目标
		class Material
		{
		public:
            Material(const char8* materialName);
			virtual ~Material();

		private:
			struct UniformSet
			{
				RenderBuffer*	uniformBuf;
				MemoryBuffer*	memberBuf;
				uint32			useSize;
			};

            struct PerScene
            {
                Matrix4 view;
                Matrix4 proj;
            };
            
            struct PerModel
            {
                Matrix4 world;
                Matrix4 bone[255];
            };
            
            struct SceneLight
            {
                Color4f lightColor;
                Color4f environmentColor;
                Vector4Df lightDir;
            };
            
		private:
            string      m_materialName;
			
			RenderPipeline*		m_renderPipeline;
			RenderResourceSet*	m_renderSet;

            BlendStateDesc          m_blendState;
            RasterizerStateDesc     m_rasterizerState;
            DepthStencilStateDesc   m_depthState;
                        
            RenderBuffer*   m_sceneParamBuf;
            PerScene        m_sceneParam;
            
            RenderBuffer*   m_modelParamBuf;
            PerModel        m_modelParam;
            
            RenderBuffer*   m_lightParamBuf;
            SceneLight      m_lightParam;

		public:
            virtual bool InitWithShaderProgram(RenderProgram* renderProgram);
            
			virtual void SetViewParamValue(const Matrix4& viewMat);
			virtual void SetProjParamValue(const Matrix4& projMat);
			virtual void SetWorldParamValue(const Matrix4& worldMat);
			virtual void SetBoneParamValue(const Matrix4* boneMat, uint32 boneSize);
			virtual void SetLightDirectionParamValue(const Vector3Df& lightDir);
			virtual void SetEnvironmentColorParamValue(Color4 envColor);
			virtual void SetLightColorParamValue(Color4 lightColor);
			virtual void SetTextureParamValue(RenderTexture* texture);
			
			virtual void SetTransparentEnabled(bool enabled);
            virtual void SetForceDepthWrite(bool force);
            virtual void SetCullBackFace(bool cull);
            virtual void SetClockwiseFrontFace(bool clockwise);

            virtual void UpdateParamValue();

			virtual RenderResourceSet* GetRenderResourceSet() { return m_renderSet; }
			virtual RenderPipeline* GetRenderPipeline() { return m_renderPipeline; }
		};
	}
}
