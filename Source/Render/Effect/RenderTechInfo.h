#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderTechInfo
		{
		public:
			virtual ~RenderTechInfo() {}

		public:
			virtual void SetTechniqueName(const char8* name) = 0;
			virtual const char8* GetTechniqueName() = 0;

			virtual void SetShaderDataValid(uint32 bind, bool valid) = 0;
			virtual bool GetShaderDataValid(uint32 bind) = 0;

			virtual void SetVariableValid(uint32 bind, bool valid) = 0;
			virtual bool GetVariableValid(uint32 bind) = 0;

			virtual void SetPassValid(uint32 bind, bool valid) = 0;
			virtual bool GetPassValid(uint32 bind) = 0;
		};
	}
}