#include "PreCompile.h"
#include "Pharos.h"

MetalShaderProgram::MetalShaderProgram()
{
}

MetalShaderProgram::~MetalShaderProgram()
{
}

bool MetalShaderProgram::CompileVertexShader(const char8* shader, const char8* enter)
{
	return true;
}

bool MetalShaderProgram::CompilePixelShader(const char8* shader, const char8* enter)
{
	return true;
}

RenderProgram* MetalShaderProgram::Clone()
{
    return NULL;
}

void MetalShaderProgram::ApplyDevice()
{
}
