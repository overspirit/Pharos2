#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalShaderProgram : public RenderProgram
		{
		public:
			MetalShaderProgram(id<MTLDevice> device);
			virtual ~MetalShaderProgram();

		private:
			id<MTLDevice>		m_device;
			id<MTLLibrary>		m_library;
			id<MTLFunction>		m_vertexFunc;
			id<MTLFunction>		m_fragmentFunc;

		public:
			id<MTLFunction> GetVertexFunction() { return m_vertexFunc; }
			id<MTLFunction> GetFragmentFunction() { return m_fragmentFunc; }
			
		public:
			virtual bool SetLibraryWithPath(const char8* libPath);
			virtual bool SetSourceWithPath(const char8* sourcePath);
			virtual bool SetSource(const char8* source);
			virtual bool CompileVertexFunctionWithName(const char8* name);
			virtual bool CompileFragmentFunctionWithName(const char8* name);
		};
	}
}
