#pragma once

namespace Pharos
{
	namespace Core
	{
		enum ResType
		{
			ERT_FONT,
			ERT_IMAGE,
			ERT_XML,
		};

		class ResBase
		{
		protected:
			ResBase();
			virtual ~ResBase();

			friend class ResourceManager;

		protected:
			File*		m_resFile;
			string		m_strFilePath;
			string		m_strFileType;

			ResType		m_resType;			

		protected:
			virtual bool Open(const char8* path) = 0;
			virtual bool Save(const char8* path) = 0;			

		public:
			virtual const char8* GetResFileType();
			virtual const char8* GetResFilePath();

			virtual ResType GetResType();
		};

	}
}
