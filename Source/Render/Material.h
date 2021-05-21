#pragma once

namespace Pharos
{
	namespace Render
	{
		//shader, 状态， 参数， 纹理， 目标
		class Material
		{
        private:
            Material(const char8* materialName);
			virtual ~Material();

            friend class RenderHelper;
            
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
                Vector4Df camerPos;
            };
            
            struct PerModel
            {
                Matrix4 world;
                Matrix4 bone[255];
            };
            
            struct SceneLight
            {
                Vector4Df lightDir;
                Color4f lightColor;
            };
            
            struct PerMaterial
            {
                Color4f materialColor;
                Color4f ambientColor;
                Color4f diffuseColor;
                Color4f specularColor;
                Vector4Df albedoPow;
            };
            
        private:
            static RenderTexture*   s_unitTexture;
            
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

            RenderBuffer*   m_materialParamBuf;
            PerMaterial      m_materialParam;
            
            RenderTexture*  m_colorTexture;
            RenderTexture*  m_bumpTexture;

		public:

		public:
            virtual bool InitWithShaderProgram(RenderProgram* renderProgram);
            
			virtual void SetViewParamValue(const Matrix4& viewMat, const Vector3Df& cameraPos);
			virtual void SetProjParamValue(const Matrix4& projMat);
			virtual void SetWorldParamValue(const Matrix4& worldMat);
			virtual void SetBoneParamValue(const Matrix4* boneMat, uint32 boneSize);
            
			virtual void SetLightDirectionParamValue(const Vector3Df& lightDir);
			virtual void SetLightColorParamValue(Color4 lightColor);
            
            virtual void SetMaterialColorParamValue(Color4 color);
            virtual void SetAbmbinetColorParamValue(Color4 color);
            virtual void SetDiffuseColorParamValue(Color4 color);
            virtual void SetSpecularColorParamValue(Color4 color);
            virtual void SetAbmbinetRatioParamValue(float32 ratio);
            virtual void SetDiffuseRatioParamValue(float32 ratio);
            virtual void SetSpecularRatioParamValue(float32 ratio);
            virtual void SetSpecularPowParamValue(float32 pow);
            
            virtual const Vector3Df& GetLightDirectionParamValue() { return *(Vector3Df*)((float*)m_lightParam.lightDir); }
            virtual Color4 GetLightColorParamValue() { return m_lightParam.lightColor; }
            virtual Color4 GetMaterialColorParamValue() { return m_materialParam.materialColor; }
            virtual Color4 GetAbmbinetColorParamValue() { return m_materialParam.ambientColor; }
            virtual Color4 GetDiffuseColorParamValue() { return m_materialParam.diffuseColor; }
            virtual Color4 GetSpecularColorParamValue() { return m_materialParam.specularColor; }
            virtual float32 GetAbmbinetRatioParamValue() { return m_materialParam.albedoPow.x; }
            virtual float32 GetDiffuseRatioParamValue() { return m_materialParam.albedoPow.y; }
            virtual float32 GetSpecularRatioParamValue() { return m_materialParam.albedoPow.z; }
            virtual float32 GetSpecularPowParamValue() { return m_materialParam.albedoPow.w; }
            
			virtual void SetColorTextureParamValue(RenderTexture* texture);
			virtual void SetBumpTextureParamValue(RenderTexture* texture);

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
