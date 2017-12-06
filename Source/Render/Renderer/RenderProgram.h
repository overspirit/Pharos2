#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderProgram
		{
		public:
			virtual ~RenderProgram() {}

		public:
			virtual bool CompileVertexShader(const char8* shader, const char8* enter) = 0;
			virtual bool CompilePixelShader(const char8* shader, const char8* enter) = 0;

			virtual RenderProgram* Clone() = 0;
		};
	}
}
