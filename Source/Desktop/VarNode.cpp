#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "DesktopGlobal.h"

VarNode::VarNode(void)
{
	m_nodeListIndex = 0;
}

VarNode::~VarNode(void)
{
}

void VarNode::LoadXml(XmlNode* pElement)
{
	m_name = pElement->GetName();

	for(uint32 i = 0; i < pElement->GetAttributeNum(); i++)
	{
		XmlAttribute* pAttr = pElement->GetAttribute(i);
		const char8* key = pAttr->GetName();
		const char8* value = pAttr->GetStringValue();
		m_valueMap[key] = value;
	}

	for(uint32 i = 0; i < pElement->GetChildNum(); i++)
	{		
		XmlNode* childXmlNode = pElement->GetChildNode(i);

		VarNodePtr childNode = MakeSharedPtr<VarNode>();
		childNode->LoadXml(childXmlNode);
		
		const char8* childName = childXmlNode->GetName();

		vector<VarNodePtr>& childNodeList = m_childList[childName];
		uint32 childNodeSize = (uint32)childNodeList.size();
		childNodeList.resize(childNodeSize + 1);
		childNodeList[childNodeSize] = childNode;
	}
}

bool VarNode::GetBoolValue(const char8* szName) const
{
	map<string, string>::const_iterator iter = m_valueMap.find(szName);
	if(iter != m_valueMap.end())
	{
		const char8* pValue = iter->second.c_str();
		switch(*(uint32*)pValue)
		{
		case 0x65757274: return true;	//字符串true
		case 0x736c6166: return false;	//字符串false
		}
	}

	return false;
}

int32 VarNode::GetIntValue(const char8* szName) const
{	
	map<string, string>::const_iterator iter = m_valueMap.find(szName);
	if(iter != m_valueMap.end())
	{
		const char8* pValue = iter->second.c_str();
		return (int32)strtol(pValue, nullptr, 10);
	}
	return 0;
}

Color4 VarNode::GetColorValue(const char8* szName) const
{
	map<string, string>::const_iterator iter = m_valueMap.find(szName);
	if(iter != m_valueMap.end())
	{
		const char8* pValue = iter->second.c_str();
		return (Color4)strtoul(pValue, nullptr, 16);		
	}
	return 0xFFFFFFFF;
}

float32 VarNode::GetFloatValue(const char8* szName) const
{
	map<string, string>::const_iterator iter = m_valueMap.find(szName);
	if(iter != m_valueMap.end())
	{
		const char8* pValue = iter->second.c_str();
		return (float32)strtod(pValue, nullptr);
	}
	return 0.0f;
}

const char8* VarNode::GetStringValue(const char8* szName) const
{	
	map<string, string>::const_iterator iter = m_valueMap.find(szName);
	if(iter != m_valueMap.end())
	{
		return iter->second.c_str();
	}
	return "";
}

VarNodePtr VarNode::GetFirstChildNode()
{
	m_childListIter = m_childList.begin();
	m_nodeListIndex = 0;

	vector<VarNodePtr>& nodeList = m_childListIter->second;
	if (nodeList.size() <= 0) return nullptr;

	return nodeList[m_nodeListIndex];
}

VarNodePtr VarNode::GetNextChildNode()
{
	m_nodeListIndex++;

	if (m_nodeListIndex >= m_childListIter->second.size())
	{
		m_childListIter++;
		m_nodeListIndex = 0;
	}

	if (m_childListIter == m_childList.end()) return nullptr;

	vector<VarNodePtr>& nodeList = m_childListIter->second;
	if (nodeList.size() <= 0) return nullptr;

	return nodeList[m_nodeListIndex];
}

uint32 VarNode::GetChildNodeNum(const char8* name)
{
	auto iter = m_childList.find(name);
	return (iter == m_childList.end()) ? 0 : (uint32)iter->second.size();
}

VarNodePtr VarNode::GetChildNode(const char8* name, uint32 index)
{
	auto iter = m_childList.find(name);
	if (iter == m_childList.end()) return nullptr;

	vector<VarNodePtr>& nodeList = iter->second;

	uint32 nodeListSize = (uint32)nodeList.size();
	if (index >= nodeListSize) return nullptr;

	return nodeList[index];
}