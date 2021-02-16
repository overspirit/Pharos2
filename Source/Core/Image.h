#pragma once

namespace Pharos
{
	namespace Core
	{
		class Image : public ResBase
		{
		private:
			Image();
			virtual ~Image();

			friend class ResourceManager;

		private:
			FIBITMAP*		m_dib;

		public:
            virtual bool Create(File* file);
			virtual bool Open(File* file);

		public:
			virtual bool SetImageSize(int32 width, int32 height);			
			virtual bool Save(const char8* path = NULL);

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
