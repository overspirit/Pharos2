#include "PreCompile.h"
#include "Pharos.h"

Package::Package()
{
    m_resType = ERT_PACKAGE;

    m_zlibFileFuncDef.zopen64_file = OpenPackage;
    m_zlibFileFuncDef.zread_file = ReadPackage;
    m_zlibFileFuncDef.zwrite_file = WritePackage;
    m_zlibFileFuncDef.ztell64_file = TellPackage;
    m_zlibFileFuncDef.zseek64_file = SeekPackage;
    m_zlibFileFuncDef.zclose_file = ClosePackage;
    m_zlibFileFuncDef.zerror_file = TestErrorPackage;
    m_zlibFileFuncDef.opaque = nullptr;
}

Package::~Package()
{
	for(auto iter : m_membufList)
	{
		SAFE_DELETE(iter.second);
	}
}

bool Package::Open(File* file)
{
	m_zlibFileFuncDef.opaque = file;

	const char8* path = file->GetPath();

	unzFile zipFile = unzOpen2_64(path, &m_zlibFileFuncDef);
    if (zipFile == nullptr) return false;

	unz_global_info64 global_info;
	if (unzGetGlobalInfo64(zipFile, &global_info) == UNZ_OK)
	{
		for (int i = 0; i < global_info.number_entry; i++)
		{
			unz_file_info64 file_info;
			char file_name[MAX_PATH] = {0};
			if(unzGetCurrentFileInfo64(zipFile, &file_info, file_name, sizeof(file_name), NULL, 0, NULL, 0) != UNZ_OK)
			{
				break;
			}

			if(unzOpenCurrentFile(zipFile) != UNZ_OK) break;
			
			MemoryBuffer* membuf = new MemoryBuffer(file_info.uncompressed_size);
			void* buf = membuf->GetPointer();
			unzReadCurrentFile(zipFile, buf, file_info.uncompressed_size);

			m_membufList[file_name] = membuf;

			unzCloseCurrentFile(zipFile);
			unzGoToNextFile(zipFile);
		}
	}

	unzClose(zipFile);

	return true;
}

MemoryBuffer* Package::GetPackageFileBuffer(const char8* packagePath)
{
	auto iter = m_membufList.find(packagePath);
	if (iter != m_membufList.end())
	{
		return iter->second;
	}

	return nullptr;
}

voidpf Package::OpenPackage(voidpf opaque, const void* filename, int mode)
{
	File* file = (File*)opaque;

	if(!file->Open((const char8*)filename))
	{
		return nullptr;
	}

	return file;
}

uLong Package::ReadPackage(voidpf opaque, voidpf stream, void* buf, uLong size)
{
	File* file = (File*)opaque;

	return file->Read(buf, size);
}

uLong Package::WritePackage(voidpf opaque, voidpf stream, const void* buf, uLong size)
{
	File* file = (File*)opaque;

	return file->Write(buf, size);
}

int Package::ClosePackage(voidpf opaque, voidpf stream)
{
	File* file = (File*)opaque;

	file->Close();

	return 0;
}

int Package::TestErrorPackage(voidpf opaque, voidpf stream)
{
	return 0;
}

ZPOS64_T Package::TellPackage(voidpf opaque, voidpf stream)
{
	File* file = (File*)opaque;

	return file->Tell();
}

long Package::SeekPackage(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin)
{
	File* file = (File*)opaque;

	FILESEEKTYPE seekType;
    switch (origin)
    {
    case ZLIB_FILEFUNC_SEEK_CUR :
        seekType = EFST_CURR;
        break;
    case ZLIB_FILEFUNC_SEEK_END :
        seekType = EFST_END;
        break;
    case ZLIB_FILEFUNC_SEEK_SET :
        seekType = EFST_BEGIN;
        break;
    default: return -1;
    }

	file->Seek(offset, seekType);

	return 0;
}