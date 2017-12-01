#pragma once

namespace Pharos
{
	namespace Utils
	{
		template<typename T>
		class TPath
		{
		public:
			TPath(const T* szPath)
			{
				m_szDrive = NULL;
				m_szDir = NULL;
				m_szFullDir = NULL;
				m_szFileName = NULL;
				m_szFileExtension = NULL;
				m_szFileFullName = NULL;

				if (szPath == NULL) return;

				const T slash = (T)'\\';

				ZeroMemory(m_pBuffer, 256 * sizeof(T));

				const T* szTemp = szPath;

				const T* szDrive = NULL;
				uint32 nDriverLen = 0;
				const T* szDir = NULL;
				uint32 nDirLen = 0;
				const T* szFileName = NULL;
				uint32 nNameLen = 0;
				const T* szFileExtension = NULL;
				uint32 nExtLen = 0;

				while (*szTemp != 0)
				{
					switch (*szTemp)
					{
					case (T)':':
					{
						szDrive = szPath;
					}
					break;
					case (T)'/':
					case (T)'\\':
					{
						szFileName = szTemp + 1;

						if (szDir == NULL)
						{
							if (szDrive != NULL) szDir = szTemp + 1;
							else szDir = szPath;

							if (szDrive != NULL) nDriverLen = (uint32)(szTemp - szDrive);
						}
						else
						{
							nDirLen = (uint32)(szFileName - szDir);
						}
					}
					break;
					case (T)'.':
					{
						szFileExtension = szTemp;
						if (szFileName != NULL) nNameLen = (uint32)(szFileExtension - szFileName);
					}
					break;
					}

					szTemp++;
				}

				if (szDrive != NULL && nDriverLen == 0) nDriverLen = (uint32)(szTemp - szDrive);
				if (szDir != NULL && nDirLen == 0) nDirLen = (uint32)(szTemp - szDir);
				if (szFileName != NULL && nNameLen == 0) nNameLen = (uint32)(szTemp - szFileName);
				if (szFileExtension != NULL && nExtLen == 0) nExtLen = (uint32)(szTemp - szFileExtension);
				if (szDrive == NULL && szDir == NULL && szFileName == NULL && szFileExtension == NULL)
				{
					szFileName = szPath;
					nNameLen = (uint32)(szTemp - szPath);
				}

				if (szDir == szFileName)
				{
					szDir = NULL;
					nDirLen = 0;
				}

				if (szFileName == szFileExtension)
				{
					szFileName = NULL;
					nNameLen = 0;
				}
				else if (szFileExtension != NULL && szFileExtension != szPath && szFileName == NULL)
				{
					szFileName = szPath;
					nNameLen = (uint32)(szFileExtension - szFileName);
				}

				T* szBuf = m_pBuffer;

				memcpy(szBuf, szDrive, sizeof(T) * nDriverLen);
				m_szDrive = szBuf;
				szBuf += nDriverLen;
				if (szDrive != NULL)
				{
					memcpy(szBuf, &slash, sizeof(T));
					szBuf += 2;
				}

				memcpy(szBuf, szDir, sizeof(T) * nDirLen);
				m_szDir = szBuf;
				szBuf += (nDirLen + 1);

				memcpy(szBuf, szFileName, sizeof(T) * nNameLen);
				m_szFileName = szBuf;
				szBuf += (nNameLen + 1);

				memcpy(szBuf, szFileExtension, sizeof(T) * nExtLen);
				m_szFileExtension = szBuf;
				szBuf += (nExtLen + 1);

				memcpy(szBuf, szDrive, sizeof(T) * nDriverLen);
				m_szFullDir = szBuf;
				szBuf += nDriverLen;
				if (szDrive != NULL)
				{
					memcpy(szBuf, &slash, sizeof(T));
					szBuf++;
				}
				memcpy(szBuf, szDir, sizeof(T) * nDirLen);
				szBuf += (nDirLen + 1);

				memcpy(szBuf, szFileName, sizeof(T) * nNameLen);
				m_szFileFullName = szBuf;
				szBuf += nNameLen;

				memcpy(szBuf, szFileExtension, sizeof(T) * nExtLen);
				szBuf += (nExtLen + 1);
			}

			virtual ~TPath()
			{
			}

		private:
			T	m_pBuffer[256];

			T*	m_szDrive;
			T*	m_szDir;
			T*	m_szFullDir;
			T*	m_szFileName;
			T*	m_szFileExtension;
			T*	m_szFileFullName;

		public:
			//获取指定路径的根目录信息。
			const T* GetPathRoot(){ return m_szDrive; }

			//返回指定路径字符串的目录信息。 
			const T* GetDirectoryName(){ return m_szDir; }

			//返回指定路径字符串的全路径。
			const T* GetFullPath(){ return m_szFullDir; }

			//返回指定路径字符串的文件名和扩展名。
			const T* GetFileName(){ return m_szFileFullName; }

			//返回不具有扩展名的指定路径字符串的文件名。
			const T* GetNameNoExt(){ return m_szFileName; }

			//返回指定的路径字符串的扩展名。 
			const T* GetExtension(){ return m_szFileExtension; }
		};

		typedef TPath<char8> Path;
	}
}