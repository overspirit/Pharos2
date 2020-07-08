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
			ERT_PACKAGE,
		};

		class ResBase
		{
		protected:
			virtual ~ResBase() { }

			friend class ResourceManager;

		protected:
			string		m_strFilePath;
			string		m_strFileType;

			ResType		m_resType;			

		protected:
			virtual bool Open(File* file) = 0;	

		public:
			virtual const char8* GetResFileType() { return m_strFileType.c_str(); }
			virtual const char8* GetResFilePath() { return m_strFilePath.c_str();}

			virtual ResType GetResType() { return m_resType; }
		};

	}
}
