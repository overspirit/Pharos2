#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalShaderProgram : public RenderProgram
		{
		public:
			MetalShaderProgram();
			virtual ~MetalShaderProgram();

		private:

		public:
			virtual bool CompileVertexShader(const char8* shader, const char8* enter);
			virtual bool CompilePixelShader(const char8* shader, const char8* enter);

			virtual RenderProgram* Clone();

			virtual void ApplyDevice();
		};
	}
}
