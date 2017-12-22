#pragma once

namespace Pharos
{
	namespace Core
	{
		class ResBase
		{
		public:
			ResBase()
			{
				m_resFile = nullptr;
			}

			virtual ~ResBase()
			{
				SAFE_DELETE(m_resFile);
			}

		protected:
			File*		m_resFile;

			string		m_strResType;
			string		m_strFilePath;
			string		m_strFileType;

		public:
			virtual bool Open(const char8* path) = 0;

			virtual const char8* GetResType()
			{
				return m_strResType.c_str();
			}

			virtual const char8* GetResFileType()
			{
				if (m_strFileType.empty())
				{
					Utils::Path path(m_strFilePath.c_str());
					m_strFileType = path.GetExtension();
				}

				return m_strFileType.c_str();
			}

			virtual const char8* GetResFilePath()
			{
				return m_strFilePath.c_str();
			}
		};
	}
}
