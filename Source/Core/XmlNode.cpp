#include "PreCompile.h"
#include "Pharos.h"

XmlNode::XmlNode(node_type type) : xml_node<char8>(type)
{
	m_child_num = 0;
	m_attr_num = 0;

	m_curr_node = nullptr;
	m_curr_node_index = 0;

	m_curr_attr = nullptr;
	m_curr_attr_index = 0;
}

XmlNode::~XmlNode()
{

}

XmlNode* XmlNode::AppendChild(XmlNode* childNode)
{
	XmlNode* node = (XmlNode*)childNode;
	this->append_node(node);
	return node;
}

XmlNode* XmlNode::AppendChild(const char8* szChildName)
{
	xml_node<char8>* node = document()->allocate_node(node_element);
	this->append_node(node);

	XmlNode* myNode = (XmlNode*)node;
	myNode->SetName(szChildName);
	return myNode;
}

XmlNode* XmlNode::PrependChild(const char8* szChildName)
{
	xml_node<char8>* node = document()->allocate_node(node_element);
	this->prepend_node(node);

	XmlNode* myNode = (XmlNode*)node;
	myNode->SetName(szChildName);
	return myNode;
}

XmlNode* XmlNode::InsertBeforeNode(uint32 nIndex, const char8* szChildName)
{
	xml_node<char8>* pWhereNode = this->FindChild(nIndex);
	if (!pWhereNode) return nullptr;

	xml_node<char8>* node = document()->allocate_node(node_element);
	this->insert_before_node(pWhereNode, node);

	XmlNode* myNode = (XmlNode*)node;
	myNode->SetName(szChildName);
	return myNode;
}

XmlNode* XmlNode::InsertBeforeNode(XmlNode* pWhereNode, const char8* szChildName)
{
	if (!pWhereNode || pWhereNode->GetParentNode() != this) return nullptr;

	for (xml_node<char8>* node = first_node(); node; node = node->next_sibling())
	{
		if (node == (XmlNode*)pWhereNode)
		{
			xml_node<char8>* new_node = document()->allocate_node(node_element);
			this->insert_before_node(node, new_node);

			XmlNode* myNode = (XmlNode*)new_node;
			myNode->SetName(szChildName);
			return myNode;
		}
	}

	return nullptr;
}

XmlNode* XmlNode::InsertAfterNode(uint32 nIndex, const char8* szChildName)
{
	xml_node<char8>* pWhereNode = this->FindChild(nIndex);
	if (!pWhereNode) return nullptr;

	xml_node<char8>* node = document()->allocate_node(node_element);
	this->insert_after_node(pWhereNode, node);

	XmlNode* myNode = (XmlNode*)node;
	myNode->SetName(szChildName);
	return myNode;
}

XmlNode* XmlNode::InsertAfterNode(XmlNode* pWhereNode, const char8* szChildName)
{
	if (!pWhereNode || pWhereNode->GetParentNode() != this) return nullptr;

	for (xml_node<char8>* node = first_node(); node; node = node->next_sibling())
	{
		if (node == (XmlNode*)pWhereNode)
		{
			xml_node<char8>* new_node = document()->allocate_node(node_element);
			this->insert_after_node(node, new_node);

			XmlNode* myNode = (XmlNode*)new_node;
			myNode->SetName(szChildName);
			return myNode;
		}
	}

	return nullptr;
}

bool XmlNode::RemoveChild(uint32 nIndex)
{
	xml_node<char8>* child = this->FindChild(nIndex);
	if (!child) return false;

	this->remove_node(child);
	return true;
}

bool XmlNode::RemoveChild(const char8* szChildName)
{
	for (xml_node<char8>* node = first_node(); node; node = node->next_sibling())
	{
		if (internal::compare(node->name(), (int32)node->name_size(), szChildName, internal::measure(szChildName), true))
		{
			this->remove_node(node);
			return true;
		}
	}

	return false;
}

bool XmlNode::RemoveChild(XmlNode* pChildNode)
{
	for (xml_node<char8>* node = first_node(); node; node = node->next_sibling())
	{
		if (node == (XmlNode*)pChildNode)
		{
			this->remove_node(node);
			return true;
		}
	}

	return false;
}

bool XmlNode::RemoveAllChild(const char8* szChildName)
{
	for (xml_node<char8>* node = first_node(); node; node = node->next_sibling())
	{
		if (internal::compare(node->name(), (int32)node->name_size(), szChildName, internal::measure(szChildName), true))
		{
			this->remove_node(node);
			return true;
		}
	}

	return false;
}

void XmlNode::RemoveAllChild()
{
	this->remove_all_nodes();
}

uint32 XmlNode::GetChildNum()
{
	return m_child_num;
}

XmlNode* XmlNode::GetFirstNode(const char8* szChildName)
{
	return (XmlNode*)this->first_node(szChildName, internal::measure(szChildName));
}

XmlNode* XmlNode::GetLastNode(const char8* szChildName)
{
	return (XmlNode*)this->last_node(szChildName, internal::measure(szChildName));
}

XmlNode* XmlNode::GetChildNode(uint32 index)
{
	return (XmlNode*)this->FindChild(index);
}

XmlNode* XmlNode::GetNextSibling(const char8* szName)
{
	return (XmlNode*)this->next_sibling(szName, internal::measure(szName));
}

XmlNode* XmlNode::GetPrevSibling(const char8* szName)
{
	return (XmlNode*)this->previous_sibling(szName, internal::measure(szName));
}

XmlAttribute* XmlNode::AppendAttribute(const char8* szAttrName)
{
	xml_attribute<char8>* attr = document()->allocate_attribute();
	this->append_attribute(attr);

	XmlAttribute* myAttr = (XmlAttribute*)attr;
	myAttr->SetName(szAttrName);
	return myAttr;
}

XmlAttribute* XmlNode::PrependAttribute(const char8* szAttrName)
{
	xml_attribute<char8>* attr = document()->allocate_attribute();
	this->prepend_attribute(attr);

	XmlAttribute* myAttr = (XmlAttribute*)attr;
	myAttr->SetName(szAttrName);
	return myAttr;
}

XmlAttribute* XmlNode::InsertBeforeAttr(uint32 nIndex, const char8* szAttrName)
{
	xml_attribute<char8>* pWhereAttr = this->FindAttribute(nIndex);
	if (!pWhereAttr) return nullptr;

	xml_attribute<char8>* attr = document()->allocate_attribute();
	this->insert_before_attribute(pWhereAttr, attr);

	XmlAttribute* myAttr = (XmlAttribute*)attr;
	myAttr->SetName(szAttrName);
	return myAttr;
}

XmlAttribute* XmlNode::InsertBeforeAttr(const char8* szWhereName, const char8* szAttrName)
{
	xml_attribute<char8>* pWhereAttr = (XmlAttribute*)this->GetAttribute(szWhereName);
	if (!pWhereAttr) return nullptr;

	xml_attribute<char8>* attr = document()->allocate_attribute();
	this->insert_before_attribute(pWhereAttr, attr);

	XmlAttribute* myAttr = (XmlAttribute*)attr;
	myAttr->SetName(szAttrName);
	return myAttr;
}

XmlAttribute* XmlNode::InsertBeforeAttr(XmlAttribute* pWhereAttr, const char8* szAttrName)
{
	if (!pWhereAttr || pWhereAttr->GetOwnerNode() != this) return nullptr;

	for (xml_attribute<char8>* attribute = first_attribute(); attribute; attribute = attribute->next_attribute())
	{
		if ((XmlAttribute*)pWhereAttr == attribute)
		{
			xml_attribute<char8>* new_attr = document()->allocate_attribute();
			this->insert_before_attribute(attribute, new_attr);

			XmlAttribute* myAttr = (XmlAttribute*)new_attr;
			myAttr->SetName(szAttrName);
			return myAttr;
		}
	}

	return nullptr;
}

XmlAttribute* XmlNode::InsertAfterAttr(uint32 nIndex, const char8* szAttrName)
{
	xml_attribute<char8>* pWhereAttr = this->FindAttribute(nIndex);
	if (!pWhereAttr) return nullptr;

	xml_attribute<char8>* attr = document()->allocate_attribute();
	this->insert_after_attribute(pWhereAttr, attr);

	XmlAttribute* myAttr = (XmlAttribute*)attr;
	myAttr->SetName(szAttrName);
	return myAttr;
}

XmlAttribute* XmlNode::InsertAfterAttr(const char8* szWhereName, const char8* szAttrName)
{
	xml_attribute<char8>* pWhereAttr = (XmlAttribute*)this->GetAttribute(szWhereName);
	if (!pWhereAttr) return nullptr;

	xml_attribute<char8>* attr = document()->allocate_attribute();
	this->insert_after_attribute(pWhereAttr, attr);

	XmlAttribute* myAttr = (XmlAttribute*)attr;
	myAttr->SetName(szAttrName);
	return myAttr;
}

XmlAttribute* XmlNode::InsertAfterAttr(XmlAttribute* pWhereAttr, const char8* szAttrName)
{
 	if (!pWhereAttr || pWhereAttr->GetOwnerNode() != this) return nullptr;
 
	for (xml_attribute<char8>* attribute = first_attribute(); attribute; attribute = attribute->next_attribute())
 	{
		if ((XmlAttribute*)pWhereAttr == attribute)
 		{
			xml_attribute<char8>* new_attr = document()->allocate_attribute();
 			this->insert_after_attribute(attribute, new_attr);

			XmlAttribute* myAttr = (XmlAttribute*)new_attr;
			myAttr->SetName(szAttrName);
			return myAttr;
 		}
 	}

	return nullptr;
}

bool XmlNode::RemoveAttribute(uint32 nIndex)
{
	xml_attribute<char8>* attr = this->FindAttribute(nIndex);
	if (!attr) return false;

	this->remove_attribute(attr);
	return true;
}

bool XmlNode::RemoveAttribute(const char8* szAttrName)
{
	for (xml_attribute<char8>* attribute = first_attribute(); attribute; attribute = attribute->next_attribute())
	{
		if (internal::compare(attribute->name(), (int32)attribute->name_size(), szAttrName, internal::measure(szAttrName), true))
		{
			this->remove_attribute(attribute);
			return true;
		}
	}

	return false;
}

bool XmlNode::RemoveAttribute(XmlAttribute* pAttr)
{
	for (xml_attribute<char8>* attribute = first_attribute(); attribute; attribute = attribute->next_attribute())
	{
		if ((XmlAttribute*)pAttr == attribute)
		{
			this->remove_attribute(attribute);
			return true;
		}
	}

	return false;
}

void XmlNode::RemoveAllAttribute()
{
	this->remove_all_attributes();
}

uint32 XmlNode::GetAttributeNum()
{
	return m_attr_num;
}

XmlAttribute* XmlNode::GetAttribute(const char8* szAttrName)
{
	return (XmlAttribute*)this->first_attribute(szAttrName, internal::measure(szAttrName));
}

XmlAttribute* XmlNode::GetAttribute(uint32 nIndex)
{
	return (XmlAttribute*)this->FindAttribute(nIndex);
}

void XmlNode::SetName(const char8* szName)
{
	uint32 len = (uint32)Utils::strlen_utf8(szName);
	char* pName = document()->allocate_string(nullptr, len + 1);
	strcpy(pName, szName);
	pName[len] = 0;
	name(pName, len);
}

const char8* XmlNode::GetName()
{
	return name();
}

const char8* XmlNode::GetInnerText()
{
	return value();
}

void XmlNode::SetInnerText(const char8* szInnerText)
{
	uint32 len = (uint32)Utils::strlen_utf8(szInnerText);
	char8* pValue = document()->allocate_string(nullptr, len + 1);
	strcpy(pValue, szInnerText);
	pValue[len] = 0;
	value(pValue, len);
}

XmlDocument* XmlNode::GetOwnerDocument()
{
	return (XmlDocument*)document();
}

XmlNode* XmlNode::GetParentNode()
{
	return (XmlNode*)m_parent;
}

xml_node<char8>* XmlNode::FindChild(uint32 index)
{
	if (index >= m_child_num || !m_first_node) return nullptr;

	if (index == m_child_num - 1)
	{
		m_curr_node = m_first_node;
		m_curr_node_index = 0;
		return m_last_node;
	}
	else if (index == 0)
	{
		m_curr_node = m_first_node->next_sibling();
		m_curr_node_index++;
		return m_first_node;
	}
	else if (index == m_curr_node_index && m_curr_node_index != -1)
	{
		xml_node<char8>* curr_node = m_curr_node;
		m_curr_node = m_curr_node->next_sibling();
		m_curr_node_index++;
		return curr_node;
	}

	xml_node<char8>* begin_node = m_first_node;
	xml_node<char8>* end_node = m_last_node;
	int begin_index = 0, end_index = m_child_num - 1;
	if (m_curr_node_index != -1)
	{
		if ((int)index < m_curr_node_index)
		{
			end_node = m_curr_node;
			end_index = m_curr_node_index;
		}
		else
		{
			begin_node = m_curr_node;
			begin_index = m_curr_node_index;
		}
	}

	int begin_step = index - begin_index;
	int end_step = end_index - index;

	if (begin_step < end_step)
	{
		xml_node<char8>* child = begin_node;
		for (int i = 0; i < begin_step; i++)
		{
			child = child->next_sibling();
		}
		m_curr_node = child->next_sibling();
		m_curr_node_index = index + 1;
		return child;
	}
	else
	{
		xml_node<char8>* child = end_node;
		for (int i = 0; i < end_step; i++)
		{
			child = child->previous_sibling();
		}
		m_curr_node = child->next_sibling();
		m_curr_node_index = index + 1;
		return child;
	}

	return nullptr;
}

xml_attribute<char8>* XmlNode::FindAttribute(uint32 index)
{
	if (index >= m_attr_num || !m_first_attribute) return nullptr;

	if (index == m_attr_num - 1)
	{
		m_curr_attr = m_first_attribute;
		m_curr_attr_index = 0;
		return m_last_attribute;
	}
	else if (index == 0)
	{
		m_curr_attr = m_first_attribute->next_attribute();
		m_curr_attr_index++;
		return m_first_attribute;
	}
	else if (index == m_curr_attr_index && m_curr_attr_index != -1)
	{
		xml_attribute<char8>* curr_attr = m_curr_attr;
		m_curr_attr = m_curr_attr->next_attribute();
		m_curr_attr_index++;
		return curr_attr;
	}

	xml_attribute<char8>* begin_attr = m_first_attribute;
	xml_attribute<char8>* end_attr = m_last_attribute;
	int begin_index = 0, end_index = m_attr_num - 1;
	if (m_curr_attr_index != -1)
	{
		if ((int)index < m_curr_attr_index)
		{
			end_attr = m_curr_attr;
			end_index = m_curr_attr_index;
		}
		else
		{
			begin_attr = m_curr_attr;
			begin_index = m_curr_attr_index;
		}
	}

	int begin_step = index - begin_index;
	int end_step = end_index - index;

	if (begin_step < end_step)
	{
		xml_attribute<char8>* attr = begin_attr;
		for (int i = 0; i < begin_step; i++)
		{
			attr = attr->next_attribute();
		}
		m_curr_attr = attr->next_attribute();
		m_curr_attr_index = index + 1;
		return attr;
	}
	else
	{
		xml_attribute<char8>* attr = end_attr;
		for (int i = 0; i < end_step; i++)
		{
			attr = attr->previous_attribute();
		}
		m_curr_attr = attr->next_attribute();
		m_curr_attr_index = index + 1;
		return attr;
	}

	return nullptr;
}

void XmlNode::prepend_node(xml_node<char8>* child)
{
	m_child_num++;
	m_curr_node = m_first_node;
	m_curr_node_index = 0;

	xml_node<char8>::prepend_node(child);
}

void XmlNode::append_node(xml_node<char8>* child)
{
	m_child_num++;
	m_curr_node = m_first_node;
	m_curr_node_index = 0;

	xml_node<char8>::append_node(child);
}

void XmlNode::insert_before_node(xml_node<char8>* where, xml_node<char8>* child)
{
	if (where != m_first_node && where != 0)
	{
		m_child_num++;
		m_curr_node = m_first_node;
		m_curr_node_index = 0;
	}

	xml_node<char8>::insert_before_node(where, child);
}

void XmlNode::insert_after_node(xml_node<char8>* where, xml_node<char8>* child)
{
	if (where != m_last_node && where != 0)
	{
		m_child_num++;
		m_curr_node = m_first_node;
		m_curr_node_index = 0;
	}

	xml_node<char8>::insert_after_node(where, child);
}

void XmlNode::remove_first_node()
{
	m_child_num--;
	m_curr_node = m_first_node;
	m_curr_node_index = 0;

	xml_node<char8>::remove_first_node();
}

void XmlNode::remove_last_node()
{
	m_child_num--;
	m_curr_node = m_first_node;
	m_curr_node_index = 0;

	xml_node<char8>::remove_last_node();
}

void XmlNode::remove_node(xml_node<char8>* where)
{
	if (where != m_first_node && where != m_last_node)
	{
		m_child_num--;
		m_curr_node = m_first_node;
		m_curr_node_index = 0;
	}

	xml_node<char8>::remove_node(where);
}

void XmlNode::remove_all_nodes()
{
	m_child_num = 0;
	m_curr_node = nullptr;
	m_curr_node_index = -1;

	xml_node<char8>::remove_all_nodes();
}

void XmlNode::prepend_attribute(xml_attribute<char8>* attribute)
{
	m_attr_num++;
	m_curr_attr = m_first_attribute;
	m_curr_attr_index = 0;

	xml_node<char8>::prepend_attribute(attribute);
}

void XmlNode::append_attribute(xml_attribute<char8>* attribute)
{
	m_attr_num++;
	m_curr_attr = m_first_attribute;
	m_curr_attr_index = 0;

	xml_node<char8>::append_attribute(attribute);
}

void XmlNode::insert_before_attribute(xml_attribute<char8>* where, xml_attribute<char8>* attribute)
{
	if (where != m_first_attribute && where != 0)
	{
		m_attr_num++;
		m_curr_attr = m_first_attribute;
		m_curr_attr_index = 0;
	}

	xml_node<char8>::insert_before_attribute(where, attribute);
}

void XmlNode::insert_after_attribute(xml_attribute<char8>* where, xml_attribute<char8>* attribute)
{
	if (where != m_last_attribute && where != 0)
	{
		m_attr_num++;
		m_curr_attr = m_first_attribute;
		m_curr_attr_index = 0;
	}

	xml_node<char8>::insert_after_attribute(where, attribute);
}

void XmlNode::remove_first_attribute()
{
	m_attr_num--;
	m_curr_attr = m_first_attribute;
	m_curr_attr_index = 0;

	xml_node<char8>::remove_first_attribute();
}

void XmlNode::remove_last_attribute()
{
	m_attr_num--;
	m_curr_attr = m_first_attribute;
	m_curr_attr_index = 0;

	xml_node<char8>::remove_last_attribute();
}

void XmlNode::remove_attribute(xml_attribute<char8>* where)
{
	if (where != m_first_attribute && where != m_last_attribute)
	{
		m_attr_num--;
		m_curr_attr = m_first_attribute;
		m_curr_attr_index = 0;
	}

	xml_node<char8>::remove_attribute(where);
}

void XmlNode::remove_all_attributes()
{
	m_attr_num = 0;
	m_curr_attr = nullptr;
	m_curr_attr_index = -1;

	xml_node<char8>::remove_all_attributes();
}
