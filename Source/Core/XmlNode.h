#pragma once

namespace Pharos
{
	namespace Core
	{
		class XmlNode : public xml_node<char8>
		{
		public:
			XmlNode(node_type type);
			virtual ~XmlNode();

			friend class XmlDocument;

		private:
			uint32			m_child_num;
			uint32			m_attr_num;

			xml_node<char8>*		m_curr_node;
			int						m_curr_node_index;

			xml_attribute<char8>*	m_curr_attr;
			int						m_curr_attr_index;

		private:
			xml_node<char8>* FindChild(uint32 index);
			xml_attribute<char8>* FindAttribute(uint32 index);

		public:;
			   virtual void prepend_node(xml_node<char8>* child);
			   virtual void append_node(xml_node<char8>* child);
			   virtual void insert_before_node(xml_node<char8>* where, xml_node<char8>* child);
			   virtual void insert_after_node(xml_node<char8>* where, xml_node<char8>* child);
			   virtual void remove_first_node();
			   virtual void remove_last_node();
			   virtual void remove_node(xml_node<char8>* where);
			   virtual void remove_all_nodes();

			   virtual void prepend_attribute(xml_attribute<char8>* attribute);
			   virtual void append_attribute(xml_attribute<char8>* attribute);
			   virtual void insert_before_attribute(xml_attribute<char8>* where, xml_attribute<char8>* attribute);
			   virtual void insert_after_attribute(xml_attribute<char8>* where, xml_attribute<char8>* attribute);
			   virtual void remove_first_attribute();
			   virtual void remove_last_attribute();
			   virtual void remove_attribute(xml_attribute<char8>* where);
			   virtual void remove_all_attributes();

		public:
			//子节点操作
			//////////////////////////////////////////////////////////////////////////
			virtual XmlNode* AppendChild(XmlNode* childNode);
			virtual XmlNode* AppendChild(const char8* szChildName);		//添加一个指定名的子节点到该节点子节点列表末尾
			virtual XmlNode* PrependChild(const char8* szChildName);		//添加一个指定名的子节点到该节点子节点列表开头

			virtual XmlNode* InsertBeforeNode(uint32 nIndex, const char8* szChildName);					//在指定位置之前添加一个指定名的子节点
			virtual XmlNode* InsertBeforeNode(XmlNode* pWhereNode, const char8* szChildName);		//在指定节点之前添加一个指定名的子节点

			virtual XmlNode* InsertAfterNode(uint32 nIndex, const char8* szChildName);					//在指定位置之后添加一个指定名的子节点
			virtual XmlNode* InsertAfterNode(XmlNode* pWhereNode, const char8* szChildName);		//在指定节点之后添加一个指定名的子节点

			virtual bool RemoveChild(uint32 nIndex);				//移除指定位置的子节点	
			virtual bool RemoveChild(const char8* szChildName);		//移除具有指定名的首个子节点
			virtual bool RemoveChild(XmlNode* pChildNode);	//移除指定子节点
			virtual bool RemoveAllChild(const char8* szChildName);	//移除所有具有指定名的子节点
			virtual void RemoveAllChild();						//移除当前节点的所有子节点 

			virtual uint32 GetChildNum();										//获取子节点的个数		
			virtual XmlNode* GetFirstNode(const char8* szChildName = nullptr);		//获取第一个子节点
			virtual XmlNode* GetLastNode(const char8* szChildName = nullptr);		//获取最后一个子节点
			virtual XmlNode* GetChildNode(uint32 nIndex);						//获取指定位置的子节点。 
			//////////////////////////////////////////////////////////////////////////

			//属性操作
			//////////////////////////////////////////////////////////////////////////
			virtual XmlAttribute* AppendAttribute(const char8* szAttrName);			//添加一个具有指定名的属性到该节点属性列表的开头
			virtual XmlAttribute* PrependAttribute(const char8* szAttrName);			//添加一个具有指定名的属性到该节点属性列表的开头

			virtual XmlAttribute* InsertBeforeAttr(uint32 nIndex, const char8* szAttrName);					//在指定位置之前添加一个具有指定名的属性
			virtual XmlAttribute* InsertBeforeAttr(const char8* szWhereName, const char8* szAttrName);			//在具有指定名的属性之前添加一个具有指定名的属性
			virtual XmlAttribute* InsertBeforeAttr(XmlAttribute* pWhereAttr, const char8* szAttrName);		//在指定属性之前添加一个具有指定名的属性

			virtual XmlAttribute* InsertAfterAttr(uint32 nIndex, const char8* szAttrName);						//在指定位置之后添加一个具有指定名的属性
			virtual XmlAttribute* InsertAfterAttr(const char8* szWhereName, const char8* szAttrName);				//在具有指定名的属性之后添加一个具有指定名的属性
			virtual XmlAttribute* InsertAfterAttr(XmlAttribute* pWhereAttr, const char8* szAttrName);		//在指定属性之后添加一个具有指定名的属性

			virtual bool RemoveAttribute(uint32 nIndex);				//移除指定位置的属性
			virtual bool RemoveAttribute(const char8* szAttrName);		//移除指定名的属性
			virtual bool RemoveAttribute(XmlAttribute* pAttr);		//移除指定属性
			virtual void RemoveAllAttribute();						//移除所有属性

			virtual uint32 GetAttributeNum();												//获取节点属性个数
			virtual XmlAttribute* GetAttribute(const char8* szAttrName);			//获取属性列表的第一个属性
			virtual XmlAttribute* GetAttribute(uint32 nIndex);								//获取指定位置的属性 
			//////////////////////////////////////////////////////////////////////////

			//其他操作
			//////////////////////////////////////////////////////////////////////////
			virtual XmlNode* GetNextSibling(const char8* szName = nullptr);	//获取下一个具有name名字的兄弟节点
			virtual XmlNode* GetPrevSibling(const char8* szName = nullptr);	//获取上一个具有name名字的兄弟节点

			virtual void SetName(const char8* szName);			//设置节点名
			virtual const char8* GetName();						//获取节点名

			virtual const char8* GetInnerText();					//获取节点文本值
			virtual void SetInnerText(const char8* szInnerText);	//设置文本节点值

			virtual XmlDocument* GetOwnerDocument();		//获取该节点所属的Xml文档
			virtual XmlNode* GetParentNode();			//获取该节点的父级。
			//////////////////////////////////////////////////////////////////////////
		};
	}
}