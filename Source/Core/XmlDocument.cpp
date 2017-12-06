#include "PreCompile.h"
#include "CoreGlobal.h"

#undef new

#define DEFAULT_DECL "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"

const static int32 parse_flag = parse_doctype_node | parse_pi_nodes | parse_validate_closing_tags;

XmlDocument::XmlDocument()
{
	m_strResType = "XML";
}

XmlDocument::~XmlDocument()
{
	clear();
}

bool XmlDocument::CreateRootNode(const char8* name)
{
	if (m_first_node != nullptr) return false;

	xml_node<char8>* node = this->allocate_node(node_element);
	this->append_node(node);

	XmlNode* myNode = (XmlNode*)node;
	myNode->SetName(name);
	
	return true;
}

bool XmlDocument::Open(const char8* path)
{
	if (m_first_node != nullptr) return false;

	m_file = sKernel->OpenFileStream(path);
	if (m_file == nullptr) return false;

	return true;
}

bool XmlDocument::Save(const char8* path)
{
	if (path == nullptr && m_strFilePath.empty()) return false;

	string text;
	text += DEFAULT_DECL;

	if (m_first_node != nullptr)
	{
		internal::print_node(back_inserter(text), m_first_node, 0, 0);
	}

	const char8* filePath = m_strFilePath.c_str();
	if (path != nullptr) filePath = path;

	File* file = sKernel->CreateFileStream(filePath, true);
	if (!file->Create(filePath, true)) return false;

	file->Write(text.c_str(), (uint32)text.length());

	m_file = file;

	return true;
}

bool XmlDocument::Load()
{
	uint32 dwFileSize = m_file->GetSize();

	char8* str = this->allocate_string(nullptr, dwFileSize + 1);
	str[dwFileSize] = 0;
	m_file->Read(str, dwFileSize);
	
	this->parse<parse_flag>(str);

	m_strFilePath = m_file->GetPath();

	return true;
}

// bool XmlDocument::LoadXml(const char8* szXmlStr)
// {
// 	if (m_first_node != nullptr) return false;
// 
// 	uint32 len = (uint32)Utils::strlen_utf8(szXmlStr);
// 	char8* buf = this->allocate_string(nullptr, len + 1);
// 	buf[len] = 0;
// 	strcpy(buf, szXmlStr);
// 
// 	try
// 	{
// 		this->parse<parse_flag>(buf);
// 	}
// 	catch (Utils::Exception&)
// 	{
// 		return false;
// 	}
// 
// 	return true;
// }

XmlNode* XmlDocument::GetRootNode()
{
	return (XmlNode*)this->first_node();
}

xml_node<char8>* XmlDocument::allocate_node(node_type type, const char8* name, const char8* value, size_t name_size, size_t value_size)
{
	void* memory = allocate_aligned(sizeof(XmlNode));
	XmlNode *node = new(memory) XmlNode(type);
	if (name)
	{
		if (name_size > 0)
			node->name(name, name_size);
		else
			node->name(name);
	}
	if (value)
	{
		if (value_size > 0)
			node->value(value, value_size);
		else
			node->value(value);
	}
	return node;
}

xml_attribute<char8>* XmlDocument::allocate_attribute(const char8* name, const char8* value, size_t name_size, size_t value_size)
{
	void* memory = allocate_aligned(sizeof(XmlAttribute));
	XmlAttribute* attribute = new(memory) XmlAttribute();
	if (name)
	{
		if (name_size > 0)
			attribute->name(name, name_size);
		else
			attribute->name(name);
	}
	if (value)
	{
		if (value_size > 0)
			attribute->value(value, value_size);
		else
			attribute->value(value);
	}
	return attribute;
}
