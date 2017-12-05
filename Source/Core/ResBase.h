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

			}

			virtual ~ResBase()
			{

			}

		protected:
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
