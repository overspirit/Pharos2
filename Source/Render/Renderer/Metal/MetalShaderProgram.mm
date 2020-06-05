#include "PreCompile.h"
#include "Pharos.h"

MetalShaderProgram::MetalShaderProgram(id<MTLDevice> device)
{
	m_device = device;
}

MetalShaderProgram::~MetalShaderProgram()
{
}

bool MetalShaderProgram::SetLibraryWithPath(const char8* libPath)
{
	//m_library = [m_device newDefaultLibrary];
	
	NSString* bundlePath = [NSString stringWithUTF8String:sKernel->GetBundleDirectoryPath()];
	NSString* fullPath = [bundlePath stringByAppendingString:[NSString stringWithUTF8String:libPath]];
	
	NSError* error = nil;
	m_library = [m_device newLibraryWithFile:fullPath error:&error];
	
	return true;
}

bool MetalShaderProgram::SetSourceWithPath(const char8* sourcePath)
{
	return true;
}

bool MetalShaderProgram::SetSource(const char8* source)
{
	return true;
}

bool MetalShaderProgram::CompileVertexFunctionWithName(const char8* name)
{
	m_vertexFunc = [m_library newFunctionWithName:[NSString stringWithUTF8String:name]];
	
	return true;
}

bool MetalShaderProgram::CompileFragmentFunctionWithName(const char8* name)
{
	m_fragmentFunc = [m_library newFunctionWithName:[NSString stringWithUTF8String:name]];
	
	return true;
}
