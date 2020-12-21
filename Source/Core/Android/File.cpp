#include "PreCompile.h"
#include "Pharos.h"

File::File(void)
{
	m_assetMgr = sPlatform->GetAssetManager();

	m_file = NULL;

	m_asset = NULL;
}

File::~File(void)
{
    Close();
}

bool File::Create(const char8* path, bool truncate)
{
	//如果该文件存在
	if (access(path, 0) == 0)
	{
		if (truncate)
		{
			//如果该文件存在且要求打开时清空
			m_file = fopen(path, "wb+");
		}
		else
		{
			//rb+和rb都必须保证文件存在，rb+会试图获得读写权限，rb只获得读权限
			m_file = fopen(path, "rb+");
			if (m_file == nullptr)
			{
				m_file = fopen(path, "rb");
			}
		}
	}
	else
	{
		//wb+会试图获得读写权限，如果文件存在，长度会清0
		m_file = fopen(path, "wb+");
	}

	m_filePath = path;

	return (m_file != nullptr);
}

bool File::Open(const char8* path)
{
	if (m_asset != NULL || m_file != NULL) return false;

	m_asset = AAssetManager_open(m_assetMgr, path, AASSET_MODE_UNKNOWN);
	if (m_asset != NULL) return true;

	//如果该文件存在
	if (access(path, 0) != 0) return false;

	//rb+和rb都必须保证文件存在，rb+会试图获得读写权限，rb只获得读权限
	m_file = fopen(path, "rb+");
	if (m_file == nullptr)
	{
		m_file = fopen(path, "rb");
	}

	m_filePath = path;

	return (m_file != nullptr);
}

uint32 File::Write(const void* lpData, uint32 dwSize)
{
	if (m_file != NULL)
	{
		return fwrite(lpData, 1, dwSize, m_file);
	}

	return 0;
}

uint32 File::Read(void* lpData, uint32 dwSize)
{
	if (m_asset != NULL)
	{
		return AAsset_read(m_asset, lpData, dwSize);
	}
	else if (m_file != NULL)
	{
		return fread(lpData, 1, dwSize, m_file);
	}

	return 0;
}

bool File::Seek(int32 nOffset, FILESEEKTYPE emSeekType)
{
	if (m_asset != NULL)
	{
		switch (emSeekType)
		{
			case EFST_BEGIN: return (AAsset_seek(m_asset, nOffset, SEEK_SET) >= 0);
			case EFST_CURR: return (AAsset_seek(m_asset, nOffset, SEEK_CUR) >= 0);
			case EFST_END: return (AAsset_seek(m_asset, nOffset, SEEK_END) >= 0);
		};
	}
	else if (m_file != NULL)
	{
		switch (emSeekType)
		{
			case EFST_BEGIN: return (fseek(m_file, nOffset, SEEK_SET) == 0);
			case EFST_CURR: return (fseek(m_file, nOffset, SEEK_CUR) == 0);
			case EFST_END: return (fseek(m_file, nOffset, SEEK_END) == 0);
		}
	}

	return false;
}

uint32 File::Tell()
{
	if (m_asset != NULL)
	{
		return (AAsset_getLength(m_asset) - AAsset_getRemainingLength(m_asset));
	}
	else if (m_file != NULL)
	{
		return (uint32)ftell(m_file);
	}

	return 0;
}

uint32 File::GetSize()
{
	if (m_asset != NULL)
	{
		return AAsset_getLength(m_asset);
	}
	else if (m_file != NULL)
	{
		uint32 offset = ftell(m_file);

		fseek(m_file, 0L, SEEK_END);
		uint32 size = ftell(m_file);

		fseek(m_file, offset, SEEK_SET);

		return size;
	}

	return 0;
}

bool File::IsEndOfFile()
{
	if (m_file == nullptr) return true;

	return Tell() >= GetSize();
}

void File::Flush()
{
	fflush(m_file);
}

void File::Close()
{
    if(m_asset != NULL)
    {
        AAsset_close(m_asset);
        m_asset = nullptr;
    }

    if(m_file != NULL)
    {
        fclose(m_file);
        m_file = nullptr;
    }
}