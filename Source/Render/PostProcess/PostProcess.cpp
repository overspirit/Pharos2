#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

PostProcess::PostProcess()
{

}

PostProcess::~PostProcess()
{

}

bool PostProcess::Init()
{
	return false;
}

void PostProcess::SetInputPin(uint32 index, RenderTexture* tex)
{
	if (index >= m_inputPins.size())
	{
		m_inputPins.resize(index + 1);
	}

	m_inputPins[index] = tex;
}

RenderTexture* PostProcess::GetOutputPin(uint32 index)
{
	if (index >= m_outputPins.size()) return nullptr;

	return m_outputPins[index];
}