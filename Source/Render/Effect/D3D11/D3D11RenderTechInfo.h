#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11RenderTechInfo : public RenderTechInfo
		{
		public:
			D3D11RenderTechInfo(void);
			virtual ~D3D11RenderTechInfo(void);

		private:
			struct VariableInfo
			{
				bool valid;

				string name;
				uint32 bind;
			};

			struct ConstantBuffInfo
			{
				bool valid;

				string name;
				uint32 bind;
				vector<string> varList;				
			};

			struct PassInfo
			{
				bool valid;

				string vertEnter;
				string pixelEnter;

				RasterizerStateDesc rasterDesc;
				BlendStateDesc	blendDesc;
				DepthStencilStateDesc depthDesc;				
			};

		private:
			string		m_techName;
			string		m_techText;

			vector<VariableInfo>		m_varInfos;
			vector<ConstantBuffInfo>	m_constantBufInfos;
			vector<PassInfo>			m_passInfos;

		public:
			virtual void SetTechniqueName(const char8* name);
			virtual const char8* GetTechniqueName();

			virtual void SetTechniqueText(const char8* text);
			virtual const char8* GetTechniqueText();

			virtual void AddVariable(const char8* name, uint32 bind);
			virtual uint32 GetVariableNum();
			virtual const char8* GetVariableName(uint32 index);
			virtual uint32 GetVariableSlot(uint32 index);			
			virtual void SetVariableValid(uint32 bind, bool valid);
			virtual bool GetVariableValid(uint32 bind);

			virtual uint32 AddConstantBuffer(const char8* name, uint32 bind);
			virtual void AddConstantBufferVariable(uint32 index, const char8* varName);
			virtual uint32 GetConstantBufferNum();
			virtual const char8* GetConstantBufferName(uint32 index);
			virtual uint32 GetConstantBufferSlot(uint32 index);
			virtual uint32 GetConstantBufferVariableNum(uint32 index);
			virtual const char8* GetConstantBufferVariableName(uint32 index, uint32 varIndex);
			virtual void SetShaderDataValid(uint32 bind, bool valid);
			virtual bool GetShaderDataValid(uint32 bind);

			virtual uint32 AddPass(const char8* vertEnter, const char8* pixelEnter);
			virtual void SetPassState(uint32 index, const RasterizerStateDesc& rasterDesc);
			virtual void SetPassState(uint32 index, const BlendStateDesc& blendDesc);
			virtual void SetPassState(uint32 index, const DepthStencilStateDesc& depthDesc);
			virtual uint32 GetPassNum();
			virtual const char8* GetPassVertexEnter(uint32 index);
			virtual const char8* GetPassPixelEnter(uint32 index);
			virtual const RasterizerStateDesc& GetPassRasterizerState(uint32 index);
			virtual const BlendStateDesc& GetPassBlendState(uint32 index);
			virtual const DepthStencilStateDesc& GetPassDepthStencilState(uint32 index);
			virtual void SetPassValid(uint32 bind, bool valid);
			virtual bool GetPassValid(uint32 bind);
		};
	}
}
