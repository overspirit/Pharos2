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

		private:
            string      m_materialName;
			
			RenderPipeline*		m_renderPipeline;
			RenderResourceSet*	m_renderSet;

            BlendStateDesc          m_blendState;
            RasterizerStateDesc     m_rasterizerState;
            DepthStencilStateDesc   m_depthState;
            
			RenderVariable*		m_viewVar;
			RenderVariable*		m_projVar;
			RenderVariable*		m_worldVar;
			RenderVariable*		m_boneVar;
			RenderVariable*		m_lightColorVar;
			RenderVariable*		m_environColorVar;
			RenderVariable*		m_lightDirVar;
			RenderVariable*		m_colorTextureVar;

			vector<UniformSet>		m_uniformSets;

		public:
			virtual void SetViewParamValue(const Matrix4& viewMat);
			virtual void SetProjParamValue(const Matrix4& projMat);
			virtual void SetWorldParamValue(const Matrix4& worldMat);
			virtual void SetBoneParamValue(const Matrix4* boneMat, uint32 boneSize);
			virtual void SetLightDirectionParamValue(const Vector3Df& lightDir);
			virtual void SetEnvironmentColorParamValue(Color4 envColor);
			virtual void SetLightColorParamValue(Color4 lightColor);
			virtual void SetTextureParamValue(const char8* valueName, RenderTexture* texture);	

			virtual void UpdateParamValue();

			virtual void SetShaderProgram(RenderProgram* renderProgram);
			virtual void SetTransparentEnabled(bool enabled);
            virtual void SetForceDepthWrite(bool force);
            virtual void SetCullBackFace(bool cull);
            virtual void SetClockwiseFrontFace(bool clockwise);

			virtual void SetTextureVariable(const char8* name, uint32 slot);
			virtual uint32 AddUniformaVariable(const char8* name, uint32 size, uint32 slot);
			virtual void SetUniformMember(uint32 varIndex, const char8* name, uint32 size);						

			virtual RenderResourceSet* GetRenderResourceSet() { return m_renderSet; }
			virtual RenderPipeline* GetRenderPipeline() { return m_renderPipeline; }
		};
	}
}
