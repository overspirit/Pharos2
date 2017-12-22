#pragma once

namespace Pharos
{
    namespace Core
    {
		enum FILESEEKTYPE
		{
			EFST_BEGIN,
			EFST_CURR,
			EFST_END
		};

		//???不支持大文件的操作
		//fseek和ftell函数在不同平台上的64位函数不一样
        class File //: public IFile
        {
        public:
            File(void);
            virtual ~File(void);
            
        private:
            FILE*   m_fileHandel;

			string	m_filePath;

		private:
			bool DoOpen(const char8* path);
			bool DoCreate(const char8* path, bool truncate);

        public:
            virtual bool Create(const char8* path, bool truncate);
			virtual bool Open(const char8* path);

            virtual uint32	Write(const void* lpData, uint32 size);
            virtual uint32	Read(void* lpData, uint32 size);
            virtual bool	Seek(int32 offset, FILESEEKTYPE seekType = EFST_BEGIN);
            virtual uint32	Tell();
			virtual bool	IsEndOfFile();
            virtual uint32	GetSize();
			virtual const char8* GetPath() { return m_filePath.c_str(); }
        };
    }
}
