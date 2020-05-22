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
			virtual bool SetLibraryWithPath(const char8* libPath) = 0;
			virtual bool SetSourceWithPath(const char8* sourcePath) = 0;
			virtual bool SetSource(const char8* source) = 0;
			virtual bool CompileVertexFunctionWithName(const char8* name) = 0;
			virtual bool CompileFragmentFunctionWithName(const char8* name) = 0;
		};
	}
}
