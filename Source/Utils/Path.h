#pragma once

namespace Pharos
{
	namespace Utils
	{
		class Path
		{
		public:
			Path(const char8* path)
			{
				char8 buf[MAX_PATH] = { 0 };
				strcpy(buf, path);

				char8* tmp = buf;
				while (*tmp != '\0') { tmp++; }
				char8* end = tmp;

				//分解盘符和其他部分
				//////////////////////////////////////////////////////////////////////////
				tmp = buf;
				char8* driver = "";
				char8* full_path = tmp;		//防止出现没有盘符的情况

				while (*tmp != '\0')
				{
					if (*tmp == ':')
					{
						*tmp = '\0';
						driver = buf;
						full_path = tmp + 1;
						break;
					}

					tmp++;
				}
				//////////////////////////////////////////////////////////////////////////

				//分解文件名和目录名
				//////////////////////////////////////////////////////////////////////////
				tmp = end;
				char8* dir = "";
				char8* full_name = full_path;	//防止出现没有目录的时候

				while (tmp != full_path - 1)
				{
					if (*tmp == '/' || *tmp == '\\')
					{
						*tmp = '\0';
						dir = full_path;
						full_name = tmp + 1;
						break;
					}

					tmp--;
				}
				//////////////////////////////////////////////////////////////////////////

				//分解文件名和扩展名
				//////////////////////////////////////////////////////////////////////////
				tmp = end;
				char8* name = full_name;
				char8* ext = "";

				while (tmp != full_name - 1)
				{
					if (*tmp == '.')
					{
						*tmp = '\0';
						name = full_name;
						ext = tmp + 1;
						break;
					}

					tmp--;
				}
				//////////////////////////////////////////////////////////////////////////

				//将替换的分割符重新加上
				//////////////////////////////////////////////////////////////////////////
				m_driver = string(driver);
				if (!m_driver.empty()) m_driver += ":";

				//如果目录名为空，盘符不为空，说明曾经替换过一个分隔符，要重新加上
				m_dir = string(dir);
				if ((m_dir.empty() && !m_driver.empty()) || !m_dir.empty()) m_dir += "/";

				m_fileName = string(name);
				m_fileExt = string(ext);
				if (!m_fileExt.empty()) m_fileExt = "." + m_fileExt;

				m_fullPath = m_driver + m_dir;
				m_fileFullName = m_fileName + m_fileExt;
				//////////////////////////////////////////////////////////////////////////
			}

			virtual ~Path()
			{
			}

		private:
			string	m_driver;
			string	m_dir;
			string	m_fullPath;
			string	m_fileName;
			string	m_fileExt;
			string	m_fileFullName;

		public:
			//获取指定路径的根目录信息。
			const char8* GetPathDriver(){ return m_driver.c_str(); }

			//返回指定路径字符串的目录信息。 
			const char8* GetPathDirectory(){ return m_dir.c_str(); }
			
			//返回指定路径字符串的全路径。
			const char8* GetFullPath() { return m_fullPath.c_str(); }

			//返回指定路径字符串的文件名和扩展名。
			const char8* GetFileFullName() { return m_fileFullName.c_str(); }

			//返回不具有扩展名的指定路径字符串的文件名。
			const char8* GetFileName() { return m_fileName.c_str(); }

			//返回指定的路径字符串的扩展名。 
			const char8* GetFileExtension(){ return m_fileExt.c_str(); }
		};

	}
}