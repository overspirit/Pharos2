#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class VarNode
		{
		public:
			VarNode(void);
			virtual ~VarNode(void);

			typedef std::shared_ptr<VarNode>	VarNodePtr;

		private:
			string										m_name;
			map<string, string>							m_valueMap;
			map<string, vector<VarNodePtr>>				m_childList;
			map<string, vector<VarNodePtr>>::iterator	m_childListIter;
			uint32										m_nodeListIndex;

		public:
			//从XML节点中读取
			void LoadXml(XmlNode* pElement);

			bool GetBoolValue(const char8* szName) const;		//bool类型
			int32 GetIntValue(const char8* szName) const;	//INT或者枚举
			Color4 GetColorValue(const char8* szName) const;	//Color类型
			float32 GetFloatValue(const char8* szName) const;	//Float类型
			const char8* GetStringValue(const char8* szName) const;	//Stirng类型

			const char8* GetVarName() const { return m_name.c_str(); } //获得该节点数据名称

			VarNodePtr GetFirstChildNode();
			VarNodePtr GetNextChildNode();

			uint32 GetChildNodeNum(const char8* name);
			VarNodePtr GetChildNode(const char8* name, uint32 index = 0);
		};
	}	

	typedef std::shared_ptr<Desktop::VarNode>	VarNodePtr;
}