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

			//RenderTechnique*		m_renderTech;

			vector<RenderTexture*>		m_texList;

			//map<string, RenderVariable*>	m_variableList;

		public:
			virtual bool SetRenderTechnique(const char8* techName);

			virtual bool SetParameterValue(const char8* valueName, float32 vlaue);
			virtual bool SetParameterValue(const char8* valueName, const Vector2Df& vlaue);
			virtual bool SetParameterValue(const char8* valueName, const Vector3Df& vlaue);
			virtual bool SetParameterValue(const char8* valueName, const Vector4Df& vlaue);
			virtual bool SetParameterValue(const char8* valueName, const Matrix4& vlaue);
			virtual bool SetParameterValue(const char8* valueName, RenderTexture* texture);
			
			//virtual RenderTechnique* GetMaterialTechnique() { return m_renderTech; }
		};
	}
}
