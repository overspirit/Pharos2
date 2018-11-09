#pragma once

namespace Pharos
{
	namespace Core
	{
		class XmlDocument : public xml_document<char8>, public ResBase
		{
		protected:
			XmlDocument();
			virtual ~XmlDocument();
			
			friend class ResourceManager;

		private:
			bool	m_loaded;

		public:
			virtual xml_node<char8>* allocate_node(node_type type, const char8* name = 0, const char8* value = 0, size_t name_size = 0, size_t value_size = 0);

			virtual xml_attribute<char8>* allocate_attribute(const char8* name = 0, const char8* value = 0, size_t name_size = 0, size_t value_size = 0);

		public:
            virtual bool CreateRootNode(const char8* name);
			virtual bool Open(const char8* path);
			virtual bool Save(const char8* path);

			virtual bool Load();
			//virtual bool LoadXml(const char8* szXmlStr);

			virtual XmlNode* GetRootNode();
		};
	}
}
