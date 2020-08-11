﻿#pragma once

namespace Pharos
{
	namespace Core
	{
		//???资源的异步机制需要在这里实现
		//???资源要引用，不要每个资源都建立一个实例

		class ResourceManager : public Utils::Singleton<ResourceManager>
		{
		public:
			ResourceManager(void);
			virtual ~ResourceManager(void);

		private:
			FT_Library			m_fontLib;

			map<string, ResBase*>	m_storageResList;

			vector<ResBase*>		m_pendingResList;

			string			m_currWorkPath;

		private:			
			ResBase* FindResource(const char8* path, ResType type);

		public:
			virtual bool Init();			
			virtual void Destroy();

		public:
			Font* GenerateFont(const char8* path);
			Image* GenerateImage(const char8* path);
			XmlDocument* GenerateXmlDocument(const char8* path);
			Package* GeneratePackage(const char8* path);

			ResBase* GenerateResource(ResType resType, const char8* resKey = NULL, File* file = nullptr);

			File* CreateResourceFile(const char8* path);
		};
	}
}
