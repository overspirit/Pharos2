#pragma once

namespace Pharos
{
	namespace Core
	{
		class Package : public ResBase
		{
		private:
			Package();
			virtual ~Package();

			friend class ResourceManager;

		private:
			zlib_filefunc64_def		m_zlibFileFuncDef;			

			map<string, MemoryBuffer*>		m_membufList;

		private:
			static voidpf   OpenPackage(voidpf opaque, const void* filename, int mode);
			static uLong    ReadPackage(voidpf opaque, voidpf stream, void* buf, uLong size);
			static uLong    WritePackage(voidpf opaque, voidpf stream, const void* buf, uLong size);
			static int      ClosePackage(voidpf opaque, voidpf stream);
			static int      TestErrorPackage(voidpf opaque, voidpf stream);
			static ZPOS64_T TellPackage(voidpf opaque, voidpf stream);
			static long     SeekPackage(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin);

		public:
            virtual bool Create(File* file);
			virtual bool Open(File* file);

		public:
			virtual MemoryBuffer* GetPackageFileBuffer(const char8* packagePath);
		};
	}
}
