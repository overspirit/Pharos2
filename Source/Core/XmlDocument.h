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
            virtual bool Create(File* file);
			virtual bool Open(File* file);

		public:
            virtual XmlNode* AppendRootNode(const char8* name);
            virtual XmlNode* GetRootNode();
            
			virtual bool Save(const char8* path = NULL);
		};
	}
}
