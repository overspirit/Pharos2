#pragma once

namespace Pharos
{
	namespace Core
	{
		class Image : public ResBase
		{
		public:
			Image();
			virtual ~Image();

		private:
			File			m_file;
			FIBITMAP*		m_dib;

		public:
			virtual bool CreateImage(int32 width, int32 height);
			virtual bool Open(const char8* path);
			virtual bool Save(const char8* path);

			virtual bool LoadImageFile();

			virtual uint32 GetWidth() const;
			virtual uint32 GetHeight() const;
			virtual uint32 GetBPP() const;
			virtual const void* GetDataPointer() const;
			virtual uint32 GetPitch() const;
			virtual bool ConvertTo32Bits();
			virtual bool ConvertTo24Bits();
		};
	}
}
