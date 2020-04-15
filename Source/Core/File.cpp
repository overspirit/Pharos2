#include "PreCompile.h"
#include "Pharos.h"

File::File(void)
{
    m_fileHandel = nullptr;
}

File::~File(void)
{
	if (m_fileHandel != nullptr)
		fclose(m_fileHandel);
}

bool File::Create(const char8* path, bool truncate)
{
	//如果该文件存在
	if (access(path, 0) == 0)
	{
		if (truncate)
		{
			//如果该文件存在且要求打开时清空
			m_fileHandel = fopen(path, "wb+");
		}
		else
		{
			//rb+和rb都必须保证文件存在，rb+会试图获得读写权限，rb只获得读权限
			m_fileHandel = fopen(path, "rb+");
			if (m_fileHandel == nullptr)
			{
				m_fileHandel = fopen(path, "rb");
			}
		}
	}
	else
	{
		//wb+会试图获得读写权限，如果文件存在，长度会清0
		m_fileHandel = fopen(path, "wb+");
	}

	m_filePath = path;

	return (m_fileHandel != nullptr);
}

bool File::Open(const char8* path)
{
	//如果该文件存在
	if (access(path, 0) != 0) return false;

	//rb+和rb都必须保证文件存在，rb+会试图获得读写权限，rb只获得读权限
	m_fileHandel = fopen(path, "rb+");
	if (m_fileHandel == nullptr)
	{
		m_fileHandel = fopen(path, "rb");
	}

	m_filePath = path;

	return (m_fileHandel != nullptr);
}

uint32 File::Write(const void* lpData, uint32 size)
{
    if(m_fileHandel == nullptr) return 0;
    
    return (uint32)fwrite(lpData, 1, size, m_fileHandel);
}

uint32 File::Read(void* lpData, uint32 size)
{
    if(m_fileHandel == nullptr) return 0;
    
    return (uint32)fread(lpData, 1, size, m_fileHandel);;
}

bool File::Seek(int32 offset, FILESEEKTYPE seekType)
{
    if(m_fileHandel == nullptr) return false;
    
    int seek = -1;
    switch (seekType)
    {
        case EFST_BEGIN: seek = SEEK_SET; break;
        case EFST_CURR: seek = SEEK_CUR; break;
        case EFST_END: seek = SEEK_END; break;
    }
    
    fseek(m_fileHandel, offset, seek);
    
    return true;
}

uint32 File::Tell()
{
    if(m_fileHandel == nullptr) return 0;
    
    return (uint32)ftell(m_fileHandel);
}

bool File::IsEndOfFile()
{
	if (m_fileHandel == nullptr) return true;

	return Tell() >= GetSize();
}

uint32 File::GetSize()
{
    if(m_fileHandel == nullptr) return 0;
    
    uint32 currPos = Tell();
    Seek(0, EFST_END);
    
    uint32 fileSize = Tell();
    Seek(currPos, EFST_BEGIN);
    
    return fileSize;
}
