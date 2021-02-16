#include "PreCompile.h"
#include "Pharos.h"

#undef new

#define DEFAULT_DECL "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"

const static int32 parse_flag = parse_doctype_node | parse_pi_nodes | parse_validate_closing_tags;

XmlDocument::XmlDocument()
{
	m_resType = ERT_XML;

	m_loaded = false;
}

XmlDocument::~XmlDocument()
{
	clear();
}

bool XmlDocument::Create(File* file)
{
    if (file == NULL) return false;
    
    m_strFilePath = file->GetPath();
    
    return true;
}

bool XmlDocument::Open(File* file)
{
	if (m_first_node != nullptr) return false;

	if (m_loaded) return true;

	uint32 dwFileSize = file->GetSize();

	char8* str = this->allocate_string(nullptr, dwFileSize + 1);
	str[dwFileSize] = 0;
	file->Read(str, dwFileSize);
	
	this->parse<parse_flag>(str);

	m_strFilePath = file->GetPath();

	m_loaded = true;

	return true;
}

bool XmlDocument::Save(const char8* path)
{
    if (path == nullptr && m_strFilePath.empty()) return false;
    
    const char8* savePath = m_strFilePath.c_str();;
    if (path != nullptr) savePath = path;

	string text;
	text += DEFAULT_DECL;

	if (m_first_node != nullptr)
	{
		internal::print_node(back_inserter(text), m_first_node, 0, 0);
	}

	File file;
	if (!file.Create(savePath, true)) return false;
	file.Write(text.c_str(), (uint32)text.length());

	return true;
}

XmlNode* XmlDocument::AppendRootNode(const char8* name)
{
    if (m_first_node != nullptr) return NULL;

    xml_node<char8>* node = this->allocate_node(node_element);
    this->append_node(node);

    XmlNode* myNode = (XmlNode*)node;
    myNode->SetName(name);
    
	return (XmlNode*)this->first_node();
}

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
