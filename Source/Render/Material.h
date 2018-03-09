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

			RenderTechnique*		m_renderTech;

			RenderVariable*			m_worldVar;
			RenderVariable*			m_viewVar;
			RenderVariable*			m_projVar;
			RenderVariable*			m_eyePosVar;

			vector<RenderTexture*>		m_texList;

			map<string, RenderVariable*>	m_variableList;

		public:
			virtual void SetRenderTechnique(const char8* techName);

			virtual void SetParameterValue(const char8* valueName, float32 vlaue);
			virtual void SetParameterValue(const char8* valueName, const Vector2Df& vlaue);
			virtual void SetParameterValue(const char8* valueName, const Vector3Df& vlaue);
			virtual void SetParameterValue(const char8* valueName, const Vector4Df& vlaue);
			virtual void SetParameterValue(const char8* valueName, const Matrix4& vlaue);
			virtual void SetParameterValue(const char8* valueName, RenderTexture* texture);
			
			virtual RenderTechnique* GetMaterialTechnique() { return m_renderTech; }
			virtual RenderVariable* GetTechniqueWorldVariable() { return m_worldVar; }
			virtual RenderVariable* GetTechniqueViewVariable() { return m_viewVar; }
			virtual RenderVariable* GetTechniqueProjVariable() { return m_projVar; }
			virtual RenderVariable* GetTechniqueEyePosVariable() { return m_eyePosVar; }

			virtual void Apply();
		};
	}
}