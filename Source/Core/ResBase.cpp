#include "PreCompile.h"
#include "CoreGlobal.h"

ResBase::ResBase()
{
	m_resFile = nullptr;
}

ResBase::~ResBase()
{
	SAFE_DELETE(m_resFile);
}

const char8* ResBase::GetResFileType()
{
	if (m_strFileType.empty())
	{
		Utils::Path path(m_strFilePath.c_str());
		m_strFileType = path.GetExtension();
	}

	return m_strFileType.c_str();
}

const char8* ResBase::GetResFilePath()
{
	if (m_resFile != nullptr)
	{
		m_strFilePath = m_resFile->GetPath();
	}

	return m_strFilePath.c_str();
}

ResType ResBase::GetResType()
{
	return m_resType;
}